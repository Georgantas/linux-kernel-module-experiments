
https://sysplay.github.io/books/LinuxDrivers/book/

In an x86 architecture, the initial 3GB (0x00000000 to 0xBFFFFFFF) is typically for RAM and the later 1GB (0xC0000000 to 0xFFFFFFFF) for device maps. However, if the RAM is less, say 2GB, device maps could start from 2GB (0x80000000).

The corresponding APIs for mapping & unmapping the device bus addresses to virtual addresses are:
```#include <asm/io.h>
void *ioremap(unsigned long device_bus_address, unsigned long device_region_size);
void iounmap(void *virt_addr);```
