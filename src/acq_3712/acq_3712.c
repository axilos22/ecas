/**********************************************************/
/* Code for handling the DAC converter	        		  */
/* version  : RTAI 3.4	                                  */
/* Authors  : ARRACHIDI - JEANNE - TRAN                   */
/* Date		: 24/10/2014				                  */
/**********************************************************/
#include "acq_3712.h"


int acq_start(void) {
	printk("ACQ3712 module started.\n");
	init3712();
	enableOutput();
	return(0);
}

void acq_stop(void) {
	disableOutput();
	printk("ACQ3712 module stopped.\n");
}

int init3712(void) {	
//TODO sequence d initialisation du 3712
  printk("3712 initialized - Ok\n");  
  return(0);
}
/* getValue use one of the DAC function which can read outputs registers
 * @param: tells the channel from the 
 */
u16 getValue(int channel) {
	u16 result;
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

void enableOutput(void) {
	outb(0x80,OUTPUT_CTRL);
}

void disableOutput(void) {
	outb(0x00,OUTPUT_CTRL);
}

void triggerConv(void) {
	outb(0,SYNC_TP);
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

void test(void) {
	int i = 0;
	enableOutput();
	for(i = 0; i< 1000000; i++) {
		/*triggerConv();*/
		setValue(1024,0);
	}
	disableOutput();
}

module_init(acq_start);
module_exit(acq_stop);

EXPORT_SYMBOL(setValue);
EXPORT_SYMBOL(disableOutput);
EXPORT_SYMBOL(getValue);
EXPORT_SYMBOL(triggerConv);
