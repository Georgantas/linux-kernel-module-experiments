
## Find the major number of the driver handling a device (5th column)
ls -al .

## Load module with parameter myvariable passed in
insmod lkm_example.ko myvariable=5

## See kernel log
dmesg

## List modules
lsmod

## Remove modules
rmmod lkm_example

## Create a device file to communicate with a kernel module from userspace
sudo mknod /dev/lkm_example c MAJOR 0
notes:
- replace MAJOR with the value of the char device
- the "c" indicates that we need a character device file created

## Read from the char device
- cat /dev/lkm_example
- dd if=/dev/lkm_example of=test bs=14 count=100

## Delete a character device file
sudo rm /dev/lkm_example

## Read info of a module
modinfo serio
