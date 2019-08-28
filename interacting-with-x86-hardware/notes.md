
https://sysplay.github.io/books/LinuxDrivers/book/Content/Part08.html

Data transfer to and from the peripherals may be done in any of the three possible ways:

- Programmed I/O.
- Interrupt- initiated I/O.
- Direct memory access (DMA).

Source: https://www.geeksforgeeks.org/io-interface-interrupt-dma-mode/

Unlike most other architectures, x86 has an additional hardware accessing mechanism through a direct I/O mapping. It is a direct 16-bit addressing scheme and doesn’t need a mapping to virtual address for its accessing (like in the DMA method used in Part 07 of this book). These addresses are referred to as port addresses, or in short – ports. As x86 has this as an additional accessing mechanism, it calls for additional set of x86 (assembly/machine code) instructions. And yes, there are the input instructions inb, inw, inl for reading an 8-bit byte, a 16-bit word, and a 32-bit long word respectively, from the I/O mapped devices through the ports. And the corresponding output instructions are outb, outw, outl, respectively. And the equivalent C functions/macros are as follows (available through the header <asm/io.h>):

```
u8 inb(unsigned long port);
u16 inw(unsigned long port);
u32 inl(unsigned long port);
void outb(u8 value, unsigned long port);
void outw(u16 value, unsigned long port);
void outl(u32 value, unsigned long port);
```

This is the "Programmed/IO" method described in the geeksforgeeks article.

/proc/ioports has all the port mappings! More precisely, it's the list of I/O ports regions that have been claimed by kernel drivers using the request_region kernel function. So it's not the complete list of I/O ports or devices available, only the ones that have been claimed by various kernel drivers. The request_region mechanism allows the kernel to prevent multiple drivers from talking to the same device. ( https://stackoverflow.com/questions/7778486/what-is-the-meaning-of-the-content-of-proc-ioports )



