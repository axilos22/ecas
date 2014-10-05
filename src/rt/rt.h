/* Includes for external lib */
#ifndef MAIN
    #define MAIN 0
    #include "../lib/rtai.h"
#endif

/*Config - 1 thetha gauche / x droite
 * 		 - 2 thetha droite / x gauche */
#define CONF 2

/*Include for CAN */
/*#include	"../can/can.h"*/

/*Include for ADC */
#include	"../pcm_3718/pcm_3718.h"

/* Include for DAC */
#include	"../acq_3712/acq_3712.h"

/*Include for pendule */
#include	"../commande/commande.h"

/* define pour gestion tache periodique */
#define STACK_SIZE  2000
#define TICK_PERIOD 1000000    //  1 ms
#define PERIODE 10000000 //10ms
#define N_BOUCLE 10000000
#define NUMERO 1
#define PRIORITE 1
#define USE_FLOAT 1

/*declaration de taches */
/*static RT_TASK tache1, tache2;*/
static RT_TASK tache1;

static u16 x_acquis, theta_acquis;

int setGrandeur(Conversion conv);

/* Utiliser les float RT TAST INIT*/