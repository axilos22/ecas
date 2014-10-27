/**********************************************************/
/* Code for stabilizing the pendulum             		  */
/* version  : RTAI 3.4 -                                  */
/* Authors  : ARRACHIDI - JEANNE - TRAN                   */
/* Date		: 24/10/2014				                  */
/**********************************************************/
#include "commande.h"

/* arcom 22 
gauche max : 2872
droite max : 1275
 * */

double S[5][6] = {
	{0.6196,    0.0968,   -0.0008,    0.0086,    0.3762,    -0.0973},
	{0.0971,    0.7038,   0.0107,     0.0012,    -0.0931,   0.2966},
   	{1.8124,    -1.7997,  1.1306,     0.2351,    -1.0133,    1.8954},
   	{-3.8837,   0.8724,   -0.1546,    0.7222,    3.0534,    -0.9858},
   	{-80.3092,  -9.6237,  -14.1215,   -23.6260,         0,         0}
};

double y1_pos_m = 0.0;
double y1_pos_V = 0.0;
double y2_theta_rad = 0.0;
double y2_theta_V = 0.0;
double y3_consigne_m = 0.0;
double y3_consigne_V = 0.0;

double x1chap= 0.0;
double x2chap= 0.0;
double x3chap= 0.0;
double x4chap= 0.0;

double _x1chap= 0.0;
double _x2chap= 0.0;
double _x3chap= 0.0;
double _x4chap= 0.0;

double commande= 0.0;
u16 commande_ajile = 2048;
int coeff_precision = 5;
int coeff_reactivite = 2;

u16 calcul(u16 pos_ajile, u16 theta_ajile, u16 consigne) {

	y1_pos_V = ((pos_ajile-2048)/2048.0);
	y1_pos_m = (y1_pos_V*.46*coeff_reactivite);

	y2_theta_V = (theta_ajile-2048)/2048.0;
	y2_theta_rad = ((y2_theta_V*1.613)/coeff_precision);
	
	y3_consigne_V = ((consigne-2048)/2048.0);
	y3_consigne_m =  y3_consigne_V*0.25;
	
	y1_pos_m = y1_pos_m + y3_consigne_m; 

	/* ATTENTION: SECURITE A NE PAS SUPPRIMER */
	if (((y2_theta_rad > 0.15)||(y2_theta_rad < -0.15))) {return 2048;}
	if (((y1_pos_m > 0.40)||(y1_pos_m < -0.40))) {return 2048;}

	_x1chap = S[0][0]*x1chap + S[0][1]*x2chap + S[0][2]*x3chap + S[0][3]*x4chap +  S[0][4]*y2_theta_rad + S[0][5]*y1_pos_m ;
	_x2chap = S[1][0]*x1chap + S[1][1]*x2chap + S[1][2]*x3chap + S[1][3]*x4chap +  S[1][4]*y2_theta_rad + S[1][5]*y1_pos_m ;
	_x3chap = S[2][0]*x1chap + S[2][1]*x2chap + S[2][2]*x3chap + S[2][3]*x4chap +  S[2][4]*y2_theta_rad + S[2][5]*y1_pos_m ;
	_x4chap = S[3][0]*x1chap + S[3][1]*x2chap + S[3][2]*x3chap + S[3][3]*x4chap +  S[3][4]*y2_theta_rad + S[3][5]*y1_pos_m ;

	commande = S[4][0]*x1chap + S[4][1]*x2chap + S[4][2]*x3chap + S[4][3]*x4chap;

	x1chap=_x1chap;
	x2chap=_x2chap;
	x3chap=_x3chap;
	x4chap=_x4chap;

	if(commande > VOLT_MAX) {commande=VOLT_MAX;}
	if(commande < VOLT_MIN) {commande=VOLT_MIN;}

	/* Format fonctionnel */
	commande_ajile = (u16) ((commande + 10) * 4096) / 20;
	
	return commande_ajile;
}

int start_com(void) {
	printk("Module COMMANDE started.\n");
	return 0;
}

void stop_com(void) {
	printk("Module COMMANDE stoped.\n");
}

module_init(start_com);
module_exit(stop_com);

EXPORT_SYMBOL(calcul);
