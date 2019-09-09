
https://sysplay.github.io/books/LinuxDrivers/book/Content/Part18.html

https://www.tldp.org/LDP/khg/HyperNews/get/fs/vfstour.html

Filesystems in Linux can be implemented as kernel modules. But there is also the FUSE (Filesystem in USErspace) interface, which can allow a regular user-space process to act as a filesystem driver. If you're prototyping a new filesystem, implementing it first using the FUSE interface could make the testing and development easier. Once you have the internals of the filesystem worked out in FUSE form, you might then start implementing a performance-optimized kernel module version of it.

Basically, the initialization function of your filesystem driver module needs just to call a register_filesystem() function, and give it as a parameter a structure that includes a function pointer that identifies the function in your filesystem driver that will be used as the first step in identifying your filesystem type and mounting it.

Unlike character or block drivers, the file system drivers involve not just one structure of function pointers, but instead 5 structures of function pointers, for the various interfaces, provided by a file system. These are:

- struct file_system_type – contains functions to operate on the super block
- struct super_operations – contains functions to operate on the inodes
- struct inode_operations – contains functions to operate on the directory entries
- struct file_operations – contains functions to operate on the file data (through page cache)
- struct address_space_operations – contains page cache operations for the file data

See:

```
struct inode_operations {
           struct file_operations * default_file_ops;
           int (*create) (struct inode *,const char *,int,int,struct inode **);
           int (*lookup) (struct inode *,const char *,int,struct inode **);
           int (*link) (struct inode *,struct inode *,const char *,int);
           int (*unlink) (struct inode *,const char *,int);
           int (*symlink) (struct inode *,const char *,int,const char *);
           int (*mkdir) (struct inode *,const char *,int,int);
           int (*rmdir) (struct inode *,const char *,int);
           int (*mknod) (struct inode *,const char *,int,int,int);
           int (*rename) (struct inode *,const char *,int,struct inode *,const char *,int);
           int (*readlink) (struct inode *,char *,int);
           int (*follow_link) (struct inode *,struct inode *,int,int,struct inode **);
           int (*readpage) (struct inode *, struct page *);
           int (*writepage) (struct inode *, struct page *);
           int (*bmap) (struct inode *,int);
           void (*truncate) (struct inode *);
           int (*permission) (struct inode *, int);
           int (*smap) (struct inode *,int);
   };
```

and:

```
struct file_operations {
           int (*lseek) (struct inode *, struct file *, off_t, int);
           int (*read) (struct inode *, struct file *, char *, int);
           int (*write) (struct inode *, struct file *, const char *, int);
           int (*readdir) (struct inode *, struct file *, void *, filldir_t);
           int (*select) (struct inode *, struct file *, int, select_table *);
           int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
           int (*mmap) (struct inode *, struct file *, struct vm_area_struct *);
           int (*open) (struct inode *, struct file *);
           void (*release) (struct inode *, struct file *);
           int (*fsync) (struct inode *, struct file *);
           int (*fasync) (struct inode *, struct file *, int);
           int (*check_media_change) (kdev_t dev);
           int (*revalidate) (kdev_t dev);
   };
```
