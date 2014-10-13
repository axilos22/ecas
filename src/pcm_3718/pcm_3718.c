/**********************************************************/
/* Code for handling the PCM 3718 ADC             		  */
/* version  : RTAI 3.4                                     */
/* Authors  : ARRACHIDI - JEANNE - TRAN                      */
/**********************************************************/

#include "pcm_3718.h"

int pcm_start(void) {
	printk("3718 started.\n");
    init3718();
    setChannelScan(0,1);
    return(0);
}

void pcm_stop(void) {
	printk("3718 - stopped.\n");
}
/*
Initialization function of the 3718.
This function set the 3718 in a ready state in order to do a A/D Conversion.
Channel 0 : -5v/+5V
Others : -10/+10
@return 0 if all clear.
*/
int init3718(void) {	
    ADRangeSelect(0,55);/* Met le channel 0 sur +10/-10 */
    ADRangeSelect(1,1010);/* Met le channel 1 sur +10/-10 */
	/* CR1 = no interrupt, no DMAE, Software trigger
       CR2 = interrupt lvl 6, no DMAE, ST
    */
	outb(CR1,CONTROL);
	outb(0xFF,STATUS); /*reset the int */
	printk("3718 Initialization.... - Ok\n");
	return(0);
}

/*
This function select the range of a provided channel.
@parameter  channel : the channel to set range.
            range   : the range to set.
*/
void ADRangeSelect(int channel, int range) {
    /* Selecting channel */
    outb(channel,MUX_SCAN);
    /* Selecting range */
    switch(range) {
    case 1010:
        outb(TEN_VOLT_BIPOLAR,RANGE_CONTROL);
        break;
    case 010:
        outb(TEN_VOLT_UNIPOLAR,RANGE_CONTROL);
        break;
    case 05:
        outb(FIVE_VOLT_UNIPOLAR,RANGE_CONTROL);
        break;
    case 55:
        outb(FIVE_VOLT_BIPOLAR,RANGE_CONTROL);
        break;
    default:
        printk("Invalid range argument.\n");
        break;
    }
}
/*
Function which choose the bounds of channel to scan.
@parameters startChan: the starting channel to scan.
            stopChan: the last channel to scan before looping.
*/
void setChannelScan(int startChan, int stopChan) {
    u8 scan;
    if( (startChan > MAX_CHANNEL) || (stopChan > MAX_CHANNEL)) {
        printk("ERROR, provided channels are out of bounds.\n");
    } else {
        scan = stopChan;
        scan = scan << 4;
        scan = scan+startChan;
        outb(scan,MUX_SCAN);
    }
}

void setChannel(int channel) {
    setChannelScan(channel,channel);
}
/*
Function made for reading a converted value.
@return value of the converted value.
*/
u16 readAD(void) {
    u8 byte1, SR;
    u16 result = 0xFFFF;
    SR = inb(STATUS);
    checkSR(SR);
    byte1 = 0x0F&SR;
	#if VERBOSE>0
		printk("Channel is %d.\n",byte1);
	#endif // VERBOSE
	result = (inb(IO_HIGHB)<<4)+(inb(IO_LOWB)>>4);
	#if VERBOSE>0
		printk("converted value = %d\n",result);
    #endif // VERBOSE
    return result;
}

Conversion readConv(void) {
	Conversion conv;
	conv.channel = 0x0F&inb(IO_LOWB);
	conv.value = (inb(IO_HIGHB)<<4)+(inb(IO_LOWB)>>4);
	return conv;
}

/*
Function made for starting a conversion.
(purpose : readability only)
*/
void startConv(void) {
    outb(1,SOFTWARE_TRIGGER);
}

void checkSR(u8 sr) {
	#if VERBOSE > 1
		printk("SR = %x\n",sr);
	#endif // VERBOSE
	if((sr&0x80) == 0x80) {
		printk("WARNING ! The PCM3718 is busy converting. Unexpected value output.\n");
	}
	if((sr&0x10) == 0x00) {
		printk("WARNING ! The converted data is not valid. Or the conv is finished.\n");
	}
}

int testOneChannel(int channel) {
	setChannel(channel);
	startConv();
	return readAD();
}

void wait_EOC(void) {
	u8 sr = inb(STATUS);
	/*printk("wait eoc.sr=%x\n",sr);*/
	while((sr&0x10)==0x00) {
		sr = inb(STATUS);
		/*printk("busy sr=%x\n",sr);*/
	}
	outb(0xFF,STATUS); /*reset the int */
}

Acq doubleAcq(void) {
    Conversion conv;
    Acq ak;
    startConv();
    wait_EOC();
    conv = readConv();
    if(conv.channel == 0) {ak.angle = conv.value;}
    if(conv.channel == 1) {ak.position = conv.value;}
    startConv();
    wait_EOC();
    conv = readConv();
    if(conv.channel == 0) {ak.angle = conv.value;}
    if(conv.channel == 1) {ak.position = conv.value;}
    return ak;
}


module_init(pcm_start);
module_exit(pcm_stop);

/* Example of exporting a function */
EXPORT_SYMBOL(doubleAcq);

/*EXPORT_SYMBOL(readConv);
EXPORT_SYMBOL(wait_EOC);
EXPORT_SYMBOL(checkSR);
EXPORT_SYMBOL(testOneChannel);
EXPORT_SYMBOL(startConv);
EXPORT_SYMBOL(init3718);
EXPORT_SYMBOL(ADRangeSelect);
EXPORT_SYMBOL(readAD);
EXPORT_SYMBOL(setChannel);
EXPORT_SYMBOL(setChannelScan);*/

