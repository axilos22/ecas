#include	"can.h"

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

void init_can(void) {
	printk("Initialisation du CAN en cours\n");
	outb(0x01,CAN_CONTROL);
	outb(0x04,CAN_COMMAND);
	outb(0xff,CAN_ACR);
	outb(0xff,CAN_AMR);
	outb(0x03,CAN_BTR0);
	outb(0x1c,CAN_BTR1);
	outb(0xFA,CAN_OCR);
	/*#if SYN == 1
		outb(0x05,CAN_IR);
		inb(CAN_IR); // ack interruption
		outb(0x02,CAN_CONTROL);
		printk("Initialisation terminee, interruptions activees.\n");
	#else*/
	outb(0x00,CAN_CONTROL);
	/*#endif*/
	printk("Initialisation terminee.\n");
}

int isValid(msg_CAN msg) {
    if((msg.size<0||msg.size>8)&&(msg.id<0||msg.id>1023) ) {
        printk("Error, id or size is out of bound.\n");
        printk("id provided: %d, size provided: %d",msg.id,msg.size);
        return 0;
    } else {
    	return 1;
    }
}

int start(void) {
	init_can();
	printk("module CAN chargé\n");
	return 0;
}

void stop(void) {
	printk("module CAN stopé\n");
}

void sendCmd(u16 commande) {
	u8 h_byte, l_byte;
	out msg_CAN;
	h_byte = commande>>8;
	l_byte = commande&0x00FF;

	out.id = 16;
	out.size = 2;
	out.data[1] = l_byte;
	out.data[2] = h_byte;
	send_CAN(out);
}

module_init(start);
module_exit(stop);

EXPORT_SYMBOL(send_CAN);
EXPORT_SYMBOL(receive_CAN);
EXPORT_SYMBOL(init_can);
EXPORT_SYMBOL(isValid);
