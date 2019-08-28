
https://sysplay.github.io/books/LinuxDrivers/book/Content/Part11.html

http://www.linux-usb.org/USB-guide/book1.html

https://coreos.com/os/docs/latest/using-systemd-and-udev-rules.html

https://www.freedesktop.org/software/systemd/man/udev.html

https://wiki.archlinux.org/index.php/Udev

Whether a driver of a USB device is there or not on a Linux system, a valid USB device would always get detected at the hardware and kernel spaces of a USB-enabled Linux system. A valid USB device is a device designed and detected as per USB protocol specifications. Hardware space detection is done by the USB host controller – typically a native bus device, e.g. a PCI device on x86 systems. The corresponding host controller driver would pick and translate the low-level physical layer information into higher level USB protocol specific information. The USB protocol formatted information about the USB device is then populated into the generic USB core layer (usbcore driver) in the kernel space, thus enabling the detection of a USB device in the kernel space, even without having its specific driver.

So, say an MFD (multi-function device) USB printer can do printing, scanning, and faxing, then it most likely would have at least three interfaces – one for each of the functionalities. So, unlike other device drivers, a USB device driver is typically associated/written per interface, rather than the device as a whole – meaning one USB device may have multiple device drivers. Though definitely one interface can have a maximum of one driver only.

For every interface, there would be one or more end points. An endpoint is like a pipe for transferring information either into or from the interface of the device, depending on the functionality. Depending on the type of information, the endpoints have four types:

- Control
- Interrupt
- Bulk
- Isochronous

As per USB protocol specification, all valid USB devices have an implicit special control endpoint zero, the only bi-directional endpoint. (see fig 20 of sysplay link)

Instead of registering with & unregistering from VFS, here we would do that with the corresponding protocol layer – the USB core in this case. The headers are found in <linux/usb.h> for the constructor and destructor:

```int usb_register(struct usb_driver *driver);
void usb_deregister(struct usb_driver *);```

