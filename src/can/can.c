#include	"can.h"

void init_can(void) {
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
}

void send_CAN(msg_CAN msg) {
	int i = 0; u8 id_byte, rtr_byte;

    #if VERBOSE > 2
		printk("######################  Emmission  ######################\n");
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
		printk("\n");
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
    	outb(0x04,CAN_COMMAND);
    }
    return msg;
}

void sendU16(u16 data, int id) {
    msg_CAN msg;
	msg.id = id;
	msg.size = 2;
	msg.data[0] = ((data&0x00ff));
    msg.data[1] = ((data&0xff00)>>8);
    send_CAN(msg);
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

unsigned int toInt(u8 c1,u8 c2) {
unsigned int i = 0;
	i = (int) (c1<<8)+c2;
	return i;
}

int start(void) {
	init_can();
	printk("CAN module loaded.\n");
	return 0;
}

void stop(void) {
	printk("CAN module stopped.\n");
}

/* Send aquisition over a CAN msg format :
H_position	: data 3
L_position	: data 2
H_angle		: data 1
L_angle		: data 0
*/
void sendAcq(Acq aq) {
	msg_CAN msg;

	msg.id = ID_ACQ;
	msg.size = 4;
	msg.data[3] = aq.position>>8;
	msg.data[2] = aq.position&0x00FF;
	msg.data[1] = aq.angle>>8;
	msg.data[0] = aq.angle&0x00FF;
	send_CAN(msg);
}

void sendCmd(u16 cmd) {
	sendU16(cmd,ID_CMD);
}

Acq canToAcq(msg_CAN msg) {
	Acq aq;
	aq.position = msg.data[3];
	aq.position = aq.position<<8;
	aq.position = aq.position+msg.data[2];

	aq.angle = msg.data[1];
	aq.angle = aq.angle<<8;
	aq.angle = aq.angle+msg.data[0];
	return aq;
}

u16 canToCmd(msg_CAN msg) {
	u16 out;
	out = msg.data[1];
	out = out<<8;
	out = out+msg.data[0];
	return out;
}
/* Gives back a boolean according if it is an acquisition message or not */
int isAcq(msg_CAN msg) {
	if(msg.id == ID_ACQ) {
		return 1;
	} else {
		return 0;
	}
}

int isCmd(msg_CAN msg) {
	if(msg.id == ID_CMD) {
		return 1;
	} else {
		return 0;
	}
}

void print(msg_CAN msg) {
	int i = 0;
	printk("--------- Message CAN ---------\nid = %d \t size= %d\n",msg.id,msg.size);
	for(i=0; i<msg.size;i++) {
		printk("data[%d]= $%x\t",i,msg.data[i]);
	}
	printk("\n");
}

module_init(start);
module_exit(stop);

EXPORT_SYMBOL(send_CAN);
EXPORT_SYMBOL(receive_CAN);
EXPORT_SYMBOL(sendAcq);
EXPORT_SYMBOL(sendCmd);
EXPORT_SYMBOL(canToAcq);
EXPORT_SYMBOL(canToCmd);
EXPORT_SYMBOL(isAcq);
EXPORT_SYMBOL(isCmd);
EXPORT_SYMBOL(print);

