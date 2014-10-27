obj-m	   := rt.o
KIT17 = arcom17.esiee.fr
KIT10 = arcom10.esiee.fr
KIT1 = arcom1.esiee.fr
KIT21= arcom21.esiee.fr
KIT22= arcom22.esiee.fr
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
send10:
	scp *.ko ../can/can.ko ../pcm_3718/pcm_3718.ko ../acq_3712/acq_3712.ko ../commande/commande.ko runarcom arcom@$(KIT10):/home/arcom
send17:
	scp *.ko ../can/can.ko ../pcm_3718/pcm_3718.ko ../acq_3712/acq_3712.ko ../commande/commande.ko runarcom arcom@$(KIT17):/home/arcom
send1:
	scp *.ko ../can/can.ko ../pcm_3718/pcm_3718.ko ../acq_3712/acq_3712.ko ../commande/commande.ko runarcom arcom@$(KIT1):/home/arcom
send21:
	scp *.ko ../can/can.ko ../pcm_3718/pcm_3718.ko ../acq_3712/acq_3712.ko ../commande/commande.ko runarcom arcom@$(KIT21):/home/arcom
send22:
	scp *.ko ../can/can.ko ../pcm_3718/pcm_3718.ko ../acq_3712/acq_3712.ko ../commande/commande.ko runarcom arcom@$(KIT22):/home/arcom
