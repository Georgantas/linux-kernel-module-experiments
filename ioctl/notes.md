https://sysplay.github.io/books/LinuxDrivers/book/Content/Part09.html

The prototype of the ioctl() function is the following: 

```long ioctl(struct file *f, unsigned int cmd, unsigned long arg);```

To invoke ioctl() from user space, use <sys/ioctl.h>:

```int ioctl(int fd, int cmd, ...);```

From http://www.circlemud.org/jelson/software/fusd/docs/node31.html:

The Linux header file /usr/include/asm/ioctl.h defines macros that must be used to create the ioctl command number. These macros take various combinations of three arguments:

    type, an 8-bit integer selected to be specific to the device driver. type should be chosen so as not to conflict with other drivers that might be ``listening'' to the same file descriptor. (Inside the kernel, for example, the TCP and IP stacks use distinct numbers since an ioctl sent to a socket file descriptor might be examined by both stacks.)
    number, an 8-bit integer command number. Within a driver, distinct numbers should be chosen for each different kind of ioctl command that the driver services
    data_type, the name of a type used to compute how many bytes are exchanged between the client and the driver. This argument is, for example, the name of a structure.

The macros used to generate command numbers are:

    _IO(int type, int number), used for a simple ioctl that sends nothing but the type and number, and receives back nothing but an (integer) retval
    _IOR(int type, int number, data_type), used for an ioctl that reads data from the device driver. The driver will be allowed to return sizeof(data_type) bytes to the user
    _IOW(int type, int number, data_type), similar to _IOR, but used to write data to the driver
    _IORW(int type, int number, data_type), a combination of _IOR and _IOW. That is, data is both written to the driver and then read back from the driver by the client


