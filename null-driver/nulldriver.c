
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

static char c;
static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class

static int my_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: open()\n");
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: close()\n");
    return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver: read()\n");
    if (raw_copy_to_user(buf, &c, 1) != 0)
        return -EFAULT;
    else
        return 1;
}
static ssize_t my_write(
        struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver: write()\n");
    if (raw_copy_from_user(&c, buf + len - 1, 1) != 0)
        return -EFAULT;
    else
        return len;
}

static struct file_operations pugs_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .read = my_read,
    .write = my_write
};

static int __init ofcd_init(void) /* Constructor */
{
    int ret;
    struct device *dev_ret;

    printk(KERN_INFO "Namaskar: ofcd registered");
    if ((ret = alloc_chrdev_region(&first, 0, 1, "Shweta")) < 0)
    {
        return ret;
    }

    // set the class this device falls under
    // this will show up under /sys/devices/virtual/chardrv
    if (IS_ERR(cl = class_create(THIS_MODULE, "chardrv")))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }

    // this will create a devices under the class "cl" declared above called mynull
    // the new device will show up under /sys/devices/virtual/chardrv/mynull
    // if we do "ls" on this folder, we find files: "power, subsystem, dev, uevent"
    // furthermore, if we do "cat" on the "dev" file, we get "237:0", which is the
    // major and minor numbers used by this driver, defined in the "first" variable
    // in user space, udev will detect this new device and do a "mknod" to create
    // /dev/mynull with MAJOR/MINOR defined in the "dev" file, which was taken from
    // the "first" variable in the following call to device_create()
    // note: alternatively, we could have skipped class_create() and device_create() calls
    // and created the file ourselves from the command line using "mknod", instead of
    // relying on udev to create /dev/mynull
    if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, "mynull")))
    {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }

    cdev_init(&c_dev, &pugs_fops);
    if ((ret = cdev_add(&c_dev, first, 1)) < 0)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return ret;
    }
    return 0;
}

static void __exit ofcd_exit(void) /* Destructor */
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "Alvida: ofcd unregistered");
}

module_init(ofcd_init);
module_exit(ofcd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia <email@sarika-pugs.com>");
MODULE_DESCRIPTION("Our First Character Driver");
