#include "commande.h"

static double S[5][6] = {
		{   0.6300,    0.1206,   -0.0008,    0.0086,    0.3658,   0.1200    },
	    {	-.0953,    .6935,    .0107,    .0012,   .0993,    .3070    },
	    {   -.2896,   -1.9184,    1.1306,    .2351,   1.0887,    2.0141   },
	    {	-3.9680,    -1.7733,   -0.1546,   0.7222,    3.1377,   1.6599  },
	    {  -80.3092,   -9.6237,  -14.1215,  -23.6260,         0,         0	}
	};

static double y1_pos_m = 0.0;
static double y1_pos_V = 0.0;
static double y2_theta_rad = 0.0;
static double y2_theta_V = 0.0;

static double commande= 0.0;
static double x1chap= 0.0;
static double x2chap= 0.0;
static double x3chap= 0.0;
static double x4chap= 0.0;
static double x5chap= 0.0;

static double _x1chap= 0.0;
static double _x2chap= 0.0;
static double _x3chap= 0.0;
static double _x4chap= 0.0;
static double _x5chap= 0.0;  

static int commande_ajile = 0;

int calcul(int pos_ajile, int theta_ajile) {		
	y1_pos_V = (((pos_ajile-1.0)*10.0)/2048.0);
	y1_pos_m = ((y1_pos_V*0.46/10.0));

	y2_theta_V = ((theta_ajile-1.0)*5.0)/2048.0; /* +5/-5 */
	y2_theta_rad = ((y2_theta_V*0.3)/5.0);

	/* ATTENTION: SECURITE A NE PAS SUPPRIMER */
	if (((y2_theta_V > 4.5)||(y2_theta_V < -4.5))) {
		commande_ajile = 2048;
		return commande_ajile;
	}
		
	_x1chap = S[0][0]*x1chap + S[0][1]* x2chap + S[0][2]*x3chap + S[0][3]*x4chap +  S[0][4]*y2_theta_rad + S[0][5]*y1_pos_m ;
	_x2chap = S[1][0]*x1chap + S[1][1]* x2chap + S[1][2]*x3chap + S[1][3]*x4chap +  S[1][4]*y2_theta_rad + S[1][5]*y1_pos_m ;
	_x3chap = S[2][0]*x1chap + S[2][1]* x2chap + S[2][2]*x3chap + S[2][3]*x4chap +  S[2][4]*y2_theta_rad + S[2][5]*y1_pos_m ;
	_x4chap = S[3][0]*x1chap + S[3][1]* x2chap + S[3][2]*x3chap + S[3][3]*x4chap +  S[3][4]*y2_theta_rad + S[3][5]*y1_pos_m ;

	commande = S[4][0]*(x1chap)+ S[4][1]*x2chap + S[4][2]*x3chap + S[4][3]*x4chap;
		
	x1chap=_x1chap;
	x2chap=_x2chap;
	x3chap=_x3chap;
	x4chap=_x4chap;
	x5chap=_x5chap;

	if(commande > VOLT_MAX) {commande=VOLT_MAX;}
	if(commande < VOLT_MIN) {commande=VOLT_MIN;} 
	
	/* Format fonctionnel */
	commande_ajile = (int) ((commande + 10) * 4096) / 20;		
	return commande_ajile;
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
