obj-m	   := commande.o

KIT = arcom21.esiee.fr
CURRENT = 2.6.16.14-arcom1
KDIR = /usr/lib/arcom/lib/modules/${CURRENT}/build
#KDIR = /opt/arcom/usr/lib/arcom/lib/modules/${CURRENT}/build
PWD = $(shell pwd)
EXTRA_CFLAGS += -I/usr/lib/arcom/realtime/include -Wall -ffast-math -mhard-float
#CC = i386-linux-gcc
#LD = i386-linux-ld
default:
	@echo --------------------------------------
	@echo PWD = $(PWD)
	@echo CURRENT = $(CURRENT)
	@echo KDIR = $(KDIR)
	make CROSS_COMPILE=i386-linux- -C $(KDIR) SUBDIRS=$(PWD) modules


clean:
	-rm -f *.o *.ko .*.cmd .*.flags *.mod.c *~
 
