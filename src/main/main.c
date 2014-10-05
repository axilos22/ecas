#include "main.h" 

int main_start(void) {
	int value = 1024, valueOut = 0;
	
	printk("main start\n");
	init3712();
	init3718();
	
	enableOutput();
	printk("Value in : %d",value);
	setValue(value,0);
	
	
	startConv();
	valueOut = readAD();
	disableOutput();
	printk("Value back : %d\n",valueOut);
	
	return(0);	
}

void main_stop(void) {
	printk("main stop\n\n");
}

module_init(main_start);
module_exit(main_stop);
