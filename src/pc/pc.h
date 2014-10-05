#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/pci.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>


MODULE_LICENSE("GPL");

/* define pour gestion tache periodique */
#define STACK_SIZE  2000
#define TICK_PERIOD 1000000    //  1 ms
#define PERIODE 10000000 //10ms
#define N_BOUCLE 10000000
#define NUMERO 1
#define PRIORITE 1


/* define pour gestion PCI CARTE CAN 7841 */
#define CAN_VENDOR_ID 0x144A
#define CAN_DEVICE_ID 0x7841

/* declaration variables pour addresse et irq */
/*  de la carte CAN PCI 7841                  */
static u32 base;  //adresse de base de la carte CAN
static u32 base2;
static unsigned int irq_7841;


/* define pour gestion registres CAN 7841 */
#define CAN_CONTROL 	base
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

#define SYN 			0
#ifndef VERBOSE
    #define	VERBOSE			3
#endif // VERBOSE

//structures message_CAN
typedef struct {
	int id;
	int size;
	u8 data[8];
 } msg_CAN;

void send_CAN(msg_CAN msg);
msg_CAN receive_CAN(void);
void sendI(int i);
void takeAndDisplay(void);
int init_7841(void);
int isValid(msg_CAN msg);
unsigned int toInt(u8 c1,u8 c2);
void sendU16(u16 data, int id);

/*declaration de taches */
static RT_TASK tache1;
