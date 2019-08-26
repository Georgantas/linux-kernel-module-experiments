
#include <linux/kernel.h>
#include <linux/module.h>
// the module will take in params
#include <linux/moduleparam.h>
// the list of system calls
#include <linux/unistd.h>
// required to know who the current user is
#include <linux/sched.h>
#include <asm/uaccess.h>

// the system call table
// this is no longer exported after v2.6.x
// as it is unsafe
extern void *sys_call_table[];

// uid to spy on, taken as a parameter
static int uid;
module_param(uid, int, 0644);

// pointer to the original system call
asmlinkage int (*original_call) (const char *, int, int);

/* 
The function we'll replace sys_open (the function 
called when you call the open system call) with. To find
the exact prototype, with the number and type of arguments,
we find the original function first (it's at fs/open.c).
In theory, this means that we're tied to the current version
of the kernel. In practice, the * system calls almost never
change (it would wreck havoc * and require programs to be
recompiled, since the system * calls are the interface
between the kernel and the * processes).
*/
asmlinkage int our_sys_open(const char *filename, int flags, int mode){
    int i = 0;
    char ch;

    // check if this is the correct user to "spy" on
    if(uid == current->uid){
        // report the file if relevant
        printk("Opened file by %d", uid);
        do {
            get_user(ch, filename + i);
            i++;
            printk("%c", ch);
        } while(ch != 0);
        printk("\n");
    }

    //call the original sys_open
    return original_call(filename, flags, mode);
}

int init_module() {
    /*          
    * To get the address of the function for system
    * call foo, go to sys_call_table[__NR_foo].
    */
    original_call = sys_call_table[__NR_open];
    sys_call_table[__NR_open] = our_sys_open;

    printk(KERN_INFO "Spying on UID:%d\n", uid);

    return 0;
}

void cleanup_module() {
    if (sys_call_table[__NR_open] != our_sys_open) {
        printk(KERN_ALERT "Somebody else also played with the ");
        printk(KERN_ALERT "open system call\n");
        printk(KERN_ALERT "The system may be left in ");
        printk(KERN_ALERT "an unstable state.\n");
    }
    sys_call_table[__NR_open] = original_call;
}
