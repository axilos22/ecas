/*
* Spec : Max sampling rate: 100KHz
sampling every 10µs
*/
/* required includes */
#ifndef MAIN
	 #include "../lib/rtai.h"
#endif

/* defines for handling PCI CARTE DAC PCM-3718 */
#define 	BASE_3718		    (0x320)

/*READ - WRITE REGISTERS */
#define     SOFTWARE_TRIGGER    (BASE_3718)
#define		RANGE_CONTROL	    (BASE_3718+1)
#define		MUX_SCAN		    (BASE_3718+2)
#define		STATUS			    (BASE_3718+8)
#define		CONTROL			    (BASE_3718+9)

/*READ ONLY REGISTERS */
#define		IO_LOWB			    SOFTWARE_TRIGGER
#define		IO_HIGHB		    RANGE_CONTROL

/*WRITE ONLY REGISTERS*/

/* Useful definitions */
#define     CR1                     0x01
#define     CR2                     0xE1
#define     MAX_CHANNEL             16
#define     PHYSICAL_CHANNEL        4
#define 	TEN_VOLT_BIPOLAR		0x08
#define 	TEN_VOLT_UNIPOLAR		0x04
#define 	FIVE_VOLT_BIPOLAR		0x00
#define 	FIVE_VOLT_UNIPOLAR		0x05

#ifndef VERBOSE
    #define VERBOSE 0
#endif // VERBOSE

/*structures*/
struct convertion {
	u16 value;
	u8 channel;
}; typedef struct convertion Conversion;

struct acq {
    u16 position;
    u16 angle;
}; typedef struct acq Acq;

/* Prototypes */
Acq	 	doubleAcq(void);
int	    init3718(void);
void    setChannel(int channel);
void    setChannelScan(int startChan, int stopChan);
void    ADRangeSelect(int channel, int range);
void    startConv(void);
int		testOneChannel(int channel);
void 	checkSR(u8 sr);
u16     readAD(void);
Conversion readConv(void);
void    wait_EOC(void);
