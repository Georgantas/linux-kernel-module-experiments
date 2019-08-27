

# if KERNELRELEASE is not defined, we've been called directly from the command line.
# Invoke the kernel build system.
ifeq (${KERNELRELEASE},)
    KERNEL_SOURCE := /usr/src/linux-5.2.9.arch1/
    PWD := $(shell pwd)
default:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} modules

clean:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} clean

# Otherwise KERNELRELEASE is defined; we've been invoked from the
# kernel build system and can use its language.
else
	obj-m := nulldriver.o
endif
