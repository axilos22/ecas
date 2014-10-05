obj-m	   	:= main.o

KIT = arcom16.esiee.fr
CURRENT = 2.6.16.14-arcom1
KDIR = /usr/lib/arcom/lib/modules/${CURRENT}/build
PWD = $(shell pwd)
EXTRA_CFLAGS += -I/usr/lib/arcom/realtime/include -Wall -ffast-math -mhard-float

default:
	@echo --------------------------------------
	@echo PWD = $(PWD)
	@echo CURRENT = $(CURRENT)
	@echo KDIR = $(KDIR)
	make CROSS_COMPILE=i386-linux- -C $(KDIR) SUBDIRS=$(PWD) modules


clean:
	-rm -f *.o *.ko .*.cmd .*.flags *.mod.c *~
send:
	scp *.ko runarcom arcom@$(KIT):/home/arcom
gather:
	cp ../pcm_3718/pcm_3718.ko ./main
	cp ../acq_3712/acq_3712.ko ./main
sons:
	cd  ../pcm_3718
	make
	cd	../acq_3712
	make
	cd	../main

