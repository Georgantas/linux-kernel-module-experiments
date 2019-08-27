
// sleep.c - creates a /proc file and if several processes try to open it at once, put all but one to sleep

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
// for get_user and put_user to access userspace
#include <linux/uaccess.h>

// keep the last message received to prove that we can process input
#define MESSAGE_LENGTH 80
static char Message[MESSAGE_LENGTH];

static struct proc_dir_entry *Our_Proc_File;
#define PROC_ENTRY_FILENAME "sleep"

// called when the /proc/sleep file is read
static ssize_t module_output(struct file *file, char *buf, size_t len, loff_t *offset) {
    static int finished = 0;
    int i;
    char message[MESSAGE_LENGTH + 30];

    // return 0 to signify EOF
    if(finished){
        finished = 0;
        return 0;
    }

    sprintf(message, "Last input:%s\n", Message);
    for (i = 0; i < len && message[i]; i++)
        put_user(message[i], buf + i);
        
    finished = 1;
    // return the number of bytes read
    return i;
}

// this receives the input when the user writes to the /proc/sleep file
static ssize_t module_input(struct file *file, const char *buf, size_t length, loff_t *offset) {
    int i;

    // put the input into Message
    for (i = 0; i < MESSAGE_LENGTH − 1 && i < length; i++)
        get_user(Message[i], buf + i);
    Message[i] = '\0';
    // return the number of input characters used
    return i;
}

// set to 1 if file is currently open by somebody
int Already_Open = 0;

// queue of processes that want the file
// we name it WaitQ
DECLARE_WAIT_QUEUE_HEAD(WaitQ);

// called when the /proc/sleep file is opened
static int module_open(struct inode *inode, struct file *file){
        // if O_NONBLOCK is included in the flags, it means the process doesn't want to wait for the file.
        // in this case, if the file is already open, we should fail with -EAGAIN, meaning "try again"
        // instead of blocking a process that wants to stay awake
        if ((file−>f_flags & O_NONBLOCK) && Already_Open)
            return −EAGAIN;

        // this prevents unloading of the module by incrementing the usage count by 1
        try_module_get(THIS_MODULE);

        // if the file is already open, wait for it to be closed
        while (Already_Open) {
            int i, is_sig = 0;

            // puts the current process to sleep (TASK_INTERRUPTIBLE) until "!Already_Open"
            // evaluates to true or a signal is received like Ctrl-C
            // condition is checked every time WaitQ is woken up
            is_sig = wait_event_interruptible(WaitQ, !Already_Open) == -ERESTARTSYS;

            if (is_sig) {
                // It's important to put module_put(THIS_MODULE) here,
                // because for processes where the open is interrupted
                // there will never be a corresponding close. If we 
                // don't decrement the usage count here, we will be 
                // left with a positive usage count which we'll have no
                // way to bring down to zero, giving us an immortal 
                // module, which can only be removed by rebooting 
                // the machine.
                module_put(THIS_MODULE);
                return -EINTR;
            }
	    }

	//open the file
	Already_Open = 1;
	// allow the access
    return 0;
}

// called when the /proc/sleep file is closed
int module_close(struct inode *inode, struct file *file)
{
	/* 
	 * Set Already_Open to zero, so one of the processes in the WaitQ will
	 * be able to set Already_Open back to one and to open the file. All 
	 * the other processes will be called when Already_Open is back to one,
	 * so they'll go back to sleep.
	 */
	Already_Open = 0;

    // Wake up all the processes in WaitQ, so if anybody is waiting for the
    // file, they can have it.
	wake_up(&WaitQ);

	module_put(THIS_MODULE);

	return 0;
}

/*
 * This function decides whether to allow an operation (return zero) or not
 * allow it (return a non-zero which indicates why it is not allowed).
 *
 * The operation can be one of the following values:
 * 0 - Execute (run the "file" - meaningless in our case)
 * 2 - Write (input to the kernel module)
 * 4 - Read (output from the kernel module)
 *
 * This is the real function that checks file permissions. The permissions
 * returned by ls -l are for reference only, and can be overridden here.
 */
static int module_permission(struct inode *inode, int op, struct nameidata *nd)
{
	/* 
	 * We allow everybody to read from our module, but only root (uid 0)
	 * may write to it
	 */
	if (op == 4 || (op == 2 && current->euid == 0))
		return 0;

	/* 
	 * If it's anything else, access is denied 
	 */
	return -EACCES;
}

static struct file_operations File_Ops_4_Our_Proc_File = {
	.read = module_output,	/* "read" from the file */
	.write = module_input,	/* "write" to the file */
	.open = module_open,	/* called when the /proc file is opened */
	.release = module_close,	/* called when it's closed */
};

static struct inode_operations Inode_Ops_4_Our_Proc_File = {
	.permission = module_permission,	/* check for permissions */
};

// runs on module load
int init_module()
{

	Our_Proc_File = create_proc_entry(PROC_ENTRY_FILENAME, 0644, NULL);
	
	if (Our_Proc_File == NULL) {
		remove_proc_entry(PROC_ENTRY_FILENAME, &proc_root);
		printk(KERN_ALERT "Error: Could not initialize /proc/test\n");
		return -ENOMEM;
	}
	
	Our_Proc_File->owner = THIS_MODULE;
	Our_Proc_File->proc_iops = &Inode_Ops_4_Our_Proc_File;
	Our_Proc_File->proc_fops = &File_Ops_4_Our_Proc_File;
	Our_Proc_File->mode = S_IFREG | S_IRUGO | S_IWUSR;
	Our_Proc_File->uid = 0;
	Our_Proc_File->gid = 0;
	Our_Proc_File->size = 80;
	
	printk(KERN_INFO "/proc/test created\n");
	
	return 0;
}

// Cleanup - unregister our file from /proc.  This could get dangerous if
// there are still processes waiting in WaitQ, because they are inside our
// open function, which will get unloaded.
void cleanup_module()
{
	remove_proc_entry(PROC_ENTRY_FILENAME, &proc_root);
	
	printk(KERN_INFO "/proc/test removed\n");
}
