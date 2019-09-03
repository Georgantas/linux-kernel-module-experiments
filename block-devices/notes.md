
https://sysplay.github.io/books/LinuxDrivers/book/Content/Part15.html (see commands section)

The key differences between block and character devices could be listed out as follows:

- Abstraction for block-oriented versus byte-oriented devices
- Block drivers are designed to be used by I/O schedulers, for optimal performance. Compare that with character drivers to be used by VFS.
- Block drivers are designed to be integrated with the Linux’ buffer cache mechanism for efficient data access. Character drivers are pass-through drivers, accessing the hardware directly.

To elaborate, there are no operations even to read and write a block device. As we already know that the block drivers need to integrate with the I/O schedulers, the read-write implementation is achieved through something called request queues. So, along with providing the device file operations, the following needs to provided:

- Request queue for queuing the read/write requests
- Spin lock associated with the request queue for its concurrent access protection
- Request function to process the requests queued in the request queue


