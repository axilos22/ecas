/**********************************************************/
/* Code de gestion du DAC  ACQ_3712	            		  */
/* version  : RTAI 3.4                                    */
/* Authors  : ARRACHIDI - JEANNE - TRAN                   */
/**********************************************************/

#include "acq_3712.h"

int init3712(void) {	
//TODO sequence d initialisation du 3712
  printk("3712 initialized - Ok\n");
  enableOutput();
  return(0);
}

void test(void) {
	int i = 0;
	enableOutput();
	for(i = 0; i< 1000000; i++) {
		/*triggerConv();*/
		setValue(1024,0);
	}
	disableOutput();
}

u16 getValue(int channel) {
	u16 result;
	/* sequence for getting value */	
	if(channel == 0) {
		result = (inb(HIGH_CH0)<<8)+(inb(LOW_CH0));
	} else {
		result = (inb(HIGH_CH1)<<8)+(inb(LOW_CH1));
	}	
	printk("Received value: %d / $%x",result,result);
	return result;
}

void setValue(u16 value, int channel) {
	u8 byte1, byte2;
	byte1 = value>>8;
	/*printk("byte1 value $%x\n",byte1);*/
	byte2 = (0xFF)&value;
	/*printk("byte2 value $%x\n",byte2);*/
	if(channel == 0) {
		outb(byte1,HIGH_CH0);
		outb(byte2,LOW_CH0);
	} else {
		outb(byte1,HIGH_CH1);
		outb(byte2,LOW_CH1);
	}	
}

int acq_start(void) {
	printk("3712 started.\n");
	init3712();
	return(0);
}

void enableOutput(void) {
	outb(0x80,OUTPUT_CTRL);
}

void disableOutput(void) {
	outb(0x00,OUTPUT_CTRL);
}

void triggerConv(void) {
	outb(0,SYNC_TP);
}

void acq_stop(void) {
	disableOutput();
	printk("3712 stopped.\n");
}

/* dessine des creneaux sur l'oscillo */
void test_adc(void) {
	int i=0;
	enableOutput();
	while(1) {
		setValue(i,0);
		i++;
		if(i>4094){i=0;}
	}
	
}

module_init(acq_start);
module_exit(acq_stop);

EXPORT_SYMBOL(init3712);
EXPORT_SYMBOL(enableOutput);
EXPORT_SYMBOL(setValue);
EXPORT_SYMBOL(disableOutput);
EXPORT_SYMBOL(getValue);
EXPORT_SYMBOL(triggerConv);
