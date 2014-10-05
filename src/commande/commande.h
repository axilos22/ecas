 #define VOLT_MAX 9.5
 #define VOLT_MIN -9.5

#ifndef MAIN
	#include "../lib/rtai.h" 
#endif

/* functions */
int calcul(int pos_ajile, int theta_ajile);
int test_comm(int x,int theta);
