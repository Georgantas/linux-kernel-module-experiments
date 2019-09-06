
Create symbol that is global across the kernel with:
- EXPORT_SYMBOL(sym)
- EXPORT_SYMBOL_GPL(sym)
- EXPORT_SYMBOL_GPL_FUTURE(sym)

The symbol could be either a variable name or a function name.

Each exported symbol have a corresponding structure placed into each of the kernel symbol table (__ksymtab), kernel string table (__kstrtab), and kernel CRC table (__kcrctab) sections, marking it to be globally accessible. Check `cat /proc/kallsyms | grep our_global_syms`, where "our_global_syms" is the name of the kernel module. Check `man nm` to see the format of this output.

To read in parameters when the function is being loaded use `module_param(name, type, perm)`, where name is the parameter name, type is the type of the parameter, and perm is the permissions of the sysfs file corresponding to this parameter. Supported type values are: byte, short, ushort, int, uint, long, ulong, charp (character pointer), bool or invbool (inverted boolean). You can see the value of this parameters under `/sys/module/module_name/parameters`. If the write access was granted to users for this parameter, they can also write to it with `echo 2 > /sys/module/module_name/parameters/cfg_value`. To pass parameter, load the module like `insmod module_param.ko cfg_value=10`.
