
The location in the kernel a process can jump to is called system_call. The procedure at that location checks the system call number, which tells the kernel what service the process requested. Then, it looks at the table of system calls (sys_call_table) to see the address of the kernel function to call. Then it calls the function, and after it returns, does a few system checks and then return back to the process (or to a different process, if the process time ran out). **If you want to read this code, it's at the source file arch/$<$architecture$>$/kernel/entry.S, after the line ENTRY(system_call).**

We can override a system call, by writing our own function, and then altering the address in the sys_call_table.
