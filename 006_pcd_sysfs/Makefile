obj-m := pcd_sysfs.o
pcd_sysfs-objs += pcd_platform_driver_dt_sysfs.o pcd_syscalls.o

ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-
KERN_DIR=/home/rickfeng/code/workspace/ldd/source/linux-5.10-rt/
HOST_KERN_DIR=/lib/modules/$(shell uname -r)/build/

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) clean
	
help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) help
	
host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

copy-dtb-to-BBB:
	sudo scp /home/rickfeng/code/workspace/ldd/source/linux-5.10-rt/arch/arm/boot/dts/am335x-boneblack.dtb debian@192.168.7.2:/home/debian/drivers

copy-drv-to-BBB:
	sudo scp *.ko debian@192.168.7.2:/home/debian/drivers
