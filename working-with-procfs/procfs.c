
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

#define procfs_name "helloworld"

struct proc_dir_entry *Our_Proc_File;

// populate the proc file
int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data){
    int ret;

    printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);

    // give information in one go because
    // the standard read function continues
    // to read system calls until the kernel
    // has no more info
    if(offset > 0) {
        ret = 0;
    } else {
        ret = sprintf(buffer, "Hello World!\n");
    }

    return ret;
}

int init_module() {
    // NOTE!: create_proc_entry was refactored in kernel 3.10
    // This module only compiles pre-3.10
    Our_Proc_File = create_proc_entry(procfs_name, 0644, NULL);

    if(Our_Proc_File == NULL) {
        remove_proc_entry(procfs_name, &proc_root);
        printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", procfs_name);
        return -ENOMEM;
    }

    Our_Proc_File->read_proc = procfile_read;
    Our_Proc_File->owner = THIS_MODULE;
    Our_Proc_File->mode = S_IFREG | S_IRUGO;
    Our_Proc_File->uid = 0;
    Our_Proc_File->gid = 0;
    Our_Proc_File->size = 37;

    printk(KERN_INFO "/proc/%s created\n", procfs_name);
    return 0;
}

void cleanup_module() {
    remove_proc_entry(procfs_name, &proc_root);
    printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}

// note: when "init_module" and "cleanup_module" are used, __init and __exit are not needed
