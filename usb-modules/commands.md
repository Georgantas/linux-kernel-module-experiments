
## Find all usb devices and corresponding drivers (sudo required)
cat /sys/kernel/debug/usb/devices

Note: If you do not see any file under /sys/kernel/debug (even as root), then you may have to first mount the debug filesystem, as follows: mount -t debugfs none /sys/kernel/debug.