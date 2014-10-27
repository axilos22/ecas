/* Includes for external lib */
#ifndef MAIN
	#include "../lib/rtai.h" 
#endif

/* define pour gestion PCI CARTE DAC PCM-3712 */
#define 	BASE_3712	(0x300)
#define		LOW_CH0		(BASE_3712)
#define		HIGH_CH0	(BASE_3712+1)
#define		LOW_CH1		(BASE_3712+2)
#define		HIGH_CH1	(BASE_3712+3)
#define		SYNC_TP		(BASE_3712+4)
#define		OUTPUT_CTRL	(BASE_3712+5) 

//prototypes
int init3712(void);
u16 getValue(int);
void enableOutput(void);
void disableOutput(void);
void triggerConv(void);
void setValue(u16 value, int channel);
void test(void);
