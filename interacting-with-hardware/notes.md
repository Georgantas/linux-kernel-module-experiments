
https://sysplay.github.io/books/LinuxDrivers/book/Content/Part07.html

In an x86 architecture, the initial 3GB (0x00000000 to 0xBFFFFFFF) is typically for RAM and the later 1GB (0xC0000000 to 0xFFFFFFFF) for device maps. However, if the RAM is less, say 2GB, device maps could start from 2GB (0x80000000).

The corresponding APIs for mapping & unmapping the device bus addresses to virtual addresses are:
```#include <asm/io.h>
void *ioremap(unsigned long device_bus_address, unsigned long device_region_size);
void iounmap(void *virt_addr);```

And then, to read/write from these virtual addresses use:
```#include <asm/io.h>

unsigned int ioread8(void *virt_addr);
unsigned int ioread16(void *virt_addr);
unsigned int ioread32(void *virt_addr);
unsigned int iowrite8(u8 value, void *virt_addr);
unsigned int iowrite16(u16 value, void *virt_addr);
unsigned int iowrite32(u32 value, void *virt_addr);```
