#include "pc.h"

int init_7841(void)
{
  /* initialisation PCI */
   u16 vendor,device;   
  u8 revision;
 struct pci_dev *dev=NULL;
 
  // RECHERCHE DE L'ADRESSE DE LA CARTE SUR LE BUS PCI

 if(/*pci_present()*/1)
    {
      dev = pci_find_device(CAN_VENDOR_ID, CAN_DEVICE_ID,dev);
      pci_read_config_dword(dev,PCI_BASE_ADDRESS_2,&base);
      if(dev==NULL) 
	{
	  printk("cannot init PCI 7841 card\n");
	  return -1;
	}
    } 
  else 
    {
      printk("cannot init PCI 7841 card\n");
      return -1 ;
    }
  
  base &= PCI_BASE_ADDRESS_IO_MASK; //adresse de base du 1er SJA1000
  printk("CARTE 7841 : BASE  0x%4x\n",base);
  base2 =  base + 0x80;   //adresse de base du 2eme SJA1000
  
  irq_7841 = dev->irq;  //on r�cup�re l'irq de la carte CAN
  
  printk("CARTE 7841 Irq: %d\n", irq_7841);
  pci_read_config_byte(dev,PCI_REVISION_ID,&revision);
  printk("CARTE 7841 Revision : %u\n", revision);
  pci_read_config_word(dev,PCI_VENDOR_ID,&vendor);
  printk("CARTE 7841, VendorId : %x\n", vendor);
  pci_read_config_word(dev,PCI_DEVICE_ID,&device);
  printk("CARTE 7841 DeviceId : %x\n", device);
  printk("CARTE 7841 Base : %x\n", base);
  
  /**** initialisation SJA 1000 ****/
  printk("Initialisation du CAN en cours\n");
	outb(0x01,CAN_CONTROL);
	outb(0x04,CAN_COMMAND);
	outb(0xff,CAN_ACR);
	outb(0xff,CAN_AMR);
	outb(0x03,CAN_BTR0);
	outb(0x1c,CAN_BTR1);
	outb(0xFA,CAN_OCR);
	#if SYN == 1
	outb(0x05,CAN_IR);
	inb(CAN_IR); // ack interruption
	outb(0x02,CAN_CONTROL);
	printk("Initialisation terminee, interruptions activees.\n");
	#else
	outb(0x00,CAN_CONTROL);
	#endif
	printk("Initialisation terminee.\n");
	return 0;
}

void tacheP1(long arg) {
	int boucle = 0;
	msg_CAN msg;
	init_7841();
		msg.id = 16;
		msg.size = 5;
		msg.data[0] = 50;
		msg.data[1] = 40;
		msg.data[2] = 30;
		msg.data[3] = 20;
		msg.data[4] = 10;
		/*send_CAN(receive_CAN());*/
		send_CAN(msg);
		boucle++;
		rt_task_wait_period();
}

int pc_start(void) {
	int ierr;
	RTIME now;
	
	init_7841();
   /* creation taches periodiques */
	rt_set_oneshot_mode();
	ierr = rt_task_init(&tache1,tacheP1,0,STACK_SIZE, PRIORITE, 0, 0);	
	start_rt_timer(nano2count(TICK_PERIOD));
	now = rt_get_time();
	
	rt_task_make_periodic(&tache1, now, nano2count(PERIODE));
	return 0;
}

void pc_stop(void) {
    stop_rt_timer();
	rt_task_delete(&tache1);
}

void send_CAN(msg_CAN msg) {
	int i = 0; u8 id_byte, rtr_byte;

    #if VERBOSE > 2
		printk("######################  Emmission  ######################\n");
        //affichage test : affiche les donnees a envoyer
        printk("ID: %d\nSIZE: %d\n",msg.id,msg.size);
        printk("Tab : \n");
        for(i=0;i<msg.size;i++) {
            printk("msg[%d] = 0x%x \t %c\n",i,msg.data[i],msg.data[i]);
        }
        printk("\n\n");
    #endif // VERBOSE

    //traitement : passer des int et char aux octets
    //Identifier et RTR
    id_byte = (u8) msg.id>>3;
    rtr_byte = ((msg.id&(0x0007))<<5);
    rtr_byte =  rtr_byte|(u8)msg.size;

    #if VERBOSE > 2
    //affichage des valeurs traitées
        printk("Octet Identifier :\t");
        printk("%x\n",id_byte);
        printk("Octet RTR :\t");
        printk("%x\n",rtr_byte);
        /* buffer d envoi */
        for(i=0; i<msg.size;i++) {
			printk("$%x  ",msg.data[i]);
			printk("    -   %c",msg.data[i]);
			printk("\n");
        }
        printk("#####   Provided to CAN buffers:   #####\n");
	#endif // VERBOSE

	#if VERBOSE > 0
    for(i=0; i<msg.size;i++) {
		printk("out[%d] = $%x\n",i,msg.data[i]);
	}
    #endif // VERBOSE

    outb(id_byte,CAN_TRANSMIT);
    outb(rtr_byte,CAN_RTR);
    for(i=0; i<msg.size;i++) {
		outb(msg.data[i],CAN_CONTROL+12+i);
	}
	outb(0x01,CAN_COMMAND);
}

msg_CAN receive_CAN() {
msg_CAN msg;
u8 identifier, rtr; int i;
//condition : a msg have been stored in buffer
    if((inb(CAN_STATUS)&(0x01))==0x01) {
    	identifier = inb(CAN_RECEIVE);
    	rtr = inb(CAN_RECEIVE_RTR);

        #if VERBOSE > 2
			printk("######################  Reception  ######################\n");
            printk("identifier byte : $%x\n",identifier);
            printk("RTR byte : $%x\n",rtr);
        #endif // VERBOSE

        msg.size = (u8)rtr&(0x0F);
        msg.id =(int) (identifier<<3)+(rtr>>5);
        for(i=0;i<msg.size;i++) {
            msg.data[i]=inb(CAN_CONTROL+22+i);
        }

        #if VERBOSE > 1
            printk("size : %d\n",msg.size);
            printk("id : %d\n",msg.id);
        #endif // VERBOSE

        #if VERBOSE > 0
        for(i=0;i<msg.size;i++) {
			printk("in[%d] = $%x\n",i,msg.data[i]);
		}
		printk("Num : %d",toInt(msg.data[1],msg.data[0]));
		printk("\n\n");
		#endif
        outb(0x04,CAN_COMMAND);
    }
    else {
    	#if VERBOSE > 0
			printk("######################  Reception  ######################\n");
			printk("Rien a recevoir.\n");
    	#endif
    	msg.size = 0;
    	msg.id = 0;
    	msg.data[0] = 0;
    	msg.data[1] = 0;
    }
    return msg;
}

void sendI(int i) {
unsigned int temp; msg_CAN msg;
	msg.id = 10;
	msg.size = 2;
	temp = (i&0xff00)>>8;
    msg.data[1] = (unsigned char) temp;
    temp = (i&0x00ff);
    msg.data[0] = (unsigned char) temp;
    send_CAN(msg);
}

void sendU16(u16 data, int id) {
    msg_CAN msg;
	msg.id = id;
	msg.size = 2;
	msg.data[0] = ((data&0x00ff));
    msg.data[1] = ((data&0xff00)>>8);
    send_CAN(msg);
}

unsigned int toInt(u8 c1,u8 c2) {
unsigned int i = 0;
	i = (int) (c1<<8)+c2;
	return i;
}

module_init(pc_start);
module_exit(pc_stop);
