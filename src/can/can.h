/* required includes */
#ifndef MAIN
	 #include "../lib/rtai.h"
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
#define CAN_BYTE2 		(CAN_CONTROL+13)
#define CAN_BYTE3 		(CAN_CONTROL+14)
#define CAN_BYTE4 		(CAN_CONTROL+15)
#define CAN_BYTE5 		(CAN_CONTROL+16)
#define CAN_BYTE6 		(CAN_CONTROL+17)
#define CAN_BYTE7 		(CAN_CONTROL+18)
#define CAN_BYTE8 		(CAN_CONTROL+19)

#define CAN_RECEIVE (CAN_CONTROL+20)
#define CAN_RECEIVE_RTR (CAN_CONTROL+21)
#define CAN_RECEIVE_B1 (CAN_CONTROL+22)
#define CAN_RECEIVE_B2 (CAN_CONTROL+23)
#define CAN_RECEIVE_B3 (CAN_CONTROL+24)
#define CAN_RECEIVE_B4 (CAN_CONTROL+25)
#define CAN_RECEIVE_B5 (CAN_CONTROL+26)
#include	"../pcm_3718/pcm_3718.h"

#define CAN_RECEIVE_B6 (CAN_CONTROL+27)
#define CAN_RECEIVE_B7 (CAN_CONTROL+28)
#define CAN_RECEIVE_B8 (CAN_CONTROL+29)

#ifndef VERBOSE
    #define	VERBOSE			3
#endif // VERBOSE

//structures message_CAN
typedef struct {int id; int size;u8 data[8];} msg_CAN;

void	send_CAN(msg_CAN msg);
msg_CAN receive_CAN(void);
void	init_can(void);
int		isValid(msg_CAN msg);
void	sendCmd(u16 commande);
void	sendAcq(Dual_acq);
u16		receiveCmd(void);
u16		receiveAcq(void);