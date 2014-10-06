#include "commande.h"

/*static int NbBitsADC=12;*/
/*static int NbBitsDAC=12;*/
/*static int NON_DEFINI=1;*/

static float S[5][6] = {
		{   0.6300,    0.1206,   -0.0008,    0.0086,    0.3658,   0.1200    },
	    {	-.0953,    .6935,    .0107,    .0012,   .0993,    .3070    },
	    {   -.2896,   -1.9184,    1.1306,    .2351,   1.0887,    2.0141   },
	    {	-3.9680,    -1.7733,   -0.1546,   0.7222,    3.1377,   1.6599  },
	    {  -80.3092,   -9.6237,  -14.1215,  -23.6260,         0,         0	}
	};

static float y1_pos_m = 0.0;
static float y2_theta_rad = 0.0;

static float pos_metre= 0.0;
static float theta_angle= 0.0;

static float commande= 0.0;
static float x1chap= 0.0;
static float x2chap= 0.0;
static float x3chap= 0.0;
static float x4chap= 0.0;
static float x5chap= 0.0;

static float _x1chap= 0.0;
static float _x2chap= 0.0;
static float _x3chap= 0.0;
static float _x4chap= 0.0;
static float _x5chap= 0.0;  

static int commande_ajile = 0.0;

int calcul(int pos_ajile, int theta_ajile) {		
	y1_pos_m  = (float) ((pos_ajile-2048.0)*.5)/2048.0;
	y2_theta_rad =(float) ((theta_ajile-2048.0)*3)/2048.0;

	// ATTENTION: SECURITE A NE PAS SUPPRIMER
	/*if (((y2_theta_rad>2.8)||(y2_theta_rad<-2.8))) {
		commande_ajile = 2048; // 0 Volt
		printk("securite verouille\n");
		return commande_ajile;			
	}*/
		
	_x1chap = S[0][0]*x1chap + S[0][1]* x2chap + S[0][2]*x3chap + S[0][3]*x4chap +  S[0][4]*theta_angle + S[0][5]*pos_metre ;
	_x2chap = S[1][0]*x1chap + S[1][1]* x2chap + S[1][2]*x3chap + S[1][3]*x4chap +  S[1][4]*theta_angle + S[1][5]*pos_metre ;
	_x3chap = S[2][0]*x1chap + S[2][1]* x2chap + S[2][2]*x3chap + S[2][3]*x4chap +  S[2][4]*theta_angle + S[2][5]*pos_metre ;
	_x4chap = S[3][0]*x1chap + S[3][1]* x2chap + S[3][2]*x3chap + S[3][3]*x4chap +  S[3][4]*theta_angle + S[3][5]*pos_metre ;

	/*commande = -(S[4][0]*(x1chap)+ S[4][1]*x2chap + S[4][2]*x3chap + S[4][3]*x4chap);*/
	commande = S[4][0]*(x1chap)+ S[4][1]*x2chap + S[4][2]*x3chap + S[4][3]*x4chap;
	printk("Cmd = %d",(int)commande);
	
	printk("xChap1 = %d , Xchap2 = %d",(int)x1chap,(int)x2chap);
		
	x1chap=_x1chap;
	x2chap=_x2chap;
	x3chap=_x3chap;
	x4chap=_x4chap;
	x5chap=_x5chap;

	if(commande > VOLT_MAX) {
		/*printk("Maximum atteint\n");*/
		commande=VOLT_MAX;}
	if(commande < VOLT_MIN) {
		/*printk("Minimum atteint\n");*/
		commande=VOLT_MIN;} 
	
	/* Format fonctionnel */
	commande_ajile = (int) ((commande + 10) * 4096.0) / 20.0;
	//commande_ajile = (int) (((commande)/10)+1)*2048;
		
	return commande_ajile;
}

int test_comm(int x,int theta) {
	return calcul(x,theta);
}

int start_com(void) {
	printk("In module COMMANDE\n");
	return 0;
}

void exit(void) {
	printk("out of module COMMANDE\n");
}

module_init(start_com);
module_exit(exit);

EXPORT_SYMBOL(calcul);
