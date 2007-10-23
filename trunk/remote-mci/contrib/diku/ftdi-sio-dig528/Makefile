# USB FTDI SIO driver hacked to work with Freescale dig528-2 boards.

MODULESDIR      = /lib/modules/$(shell uname -r)
BUILDDIR        = $(MODULESDIR)/build
DRIVERNAME      = ftdi_sio_dig528
DRIVEROBJ       = $(MODULESDIR)/kernel/drivers/usb/serial/$(DRIVERNAME).ko

obj-m          += $(DRIVERNAME).o

all:
	$(MAKE) -C $(BUILDDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(BUILDDIR) M=$(PWD) clean
	$(RM) Module.symvers
	#$(RM) $(DRIVEROBJ)

install:
	cp $(DRIVERNAME).ko $(DRIVEROBJ)
	/sbin/depmod -a
