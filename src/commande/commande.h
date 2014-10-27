 #define VOLT_MAX 10
 #define VOLT_MIN -10

#ifndef MAIN
	#include "../lib/rtai.h"
#endif

/* functions */
u16 calcul(u16 pos_ajile, u16 theta_ajile, u16 consigne);
