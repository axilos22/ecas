/* required includes */
#ifndef MAIN
	#define MAIN 1
	#include "../lib/rtai.h"
	#include "../pcm_3718/pcm_3718.h"
#endif

/* define pour gestion registres CAN 7841 */
#define CAN_CONTROL 	0x180
#define CAN_COMMAND 	(CAN_CONTROL+1)
#define CAN_STATUS  	(CAN_CONTROL+2)
#define CAN_IR 			(CAN_CONTROL+3)
#define CAN_ACR 		(CAN_CONTROL+4)
#define CAN_AMR 		(CAN_CONTROL+5)
#define CAN_BTR0 		(CAN_CONTROL+6)
#define CAN_BTR1 		(CAN_CONTROL+7)
#define CAN_OCR 		(CAN_CONTROL+8)
#define CAN_TRANSMIT 	(CAN_CONTROL+10)
#define CAN_RTR 		(CAN_CONTROL+11)
#define CAN_BYTE1 		(CAN_CONTROL+12)

#define CAN_RECEIVE (CAN_CONTROL+20)
#define CAN_RECEIVE_RTR (CAN_CONTROL+21)
#define CAN_RECEIVE_B1 (CAN_CONTROL+22)

#define	ID_ACQ			16
#define	ID_CMD			17

#define SYN 			0
#ifndef VERBOSE
    #define	VERBOSE			1
#endif // VERBOSE

//structures message_CAN
typedef struct {int id; int size;u8 data[8];} msg_CAN;

void send_CAN(msg_CAN msg);
msg_CAN receive_CAN(void);
void init_can(void);
int isValid(msg_CAN msg);
unsigned int toInt(u8 c1,u8 c2);
void sendU16(u16 data, int id);
void sendAcq(Acq aq);
void sendCmd(u16 cmd);
Acq canToAcq(msg_CAN msg);
u16 canToCmd(msg_CAN msg);
int isAcq(msg_CAN msg);
void print(msg_CAN msg);
int isCmd(msg_CAN msg);
