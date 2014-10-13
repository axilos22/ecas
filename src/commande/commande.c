#include "commande.h"
/* arcom 22 
gauche max : 2872
droite max : 1275
 * */

double S[5][6] = {
    {   0.6300,    0.1206,   -0.0008,    0.0086,    0.3658,   0.1200    },
    {	-.0953,    .6935,    .0107,    .0012,   .0993,    .3070    },
    {   -.2896,   -1.9184,    1.1306,    .2351,   1.0887,    2.0141   },
    {	-3.9680,    -1.7733,   -0.1546,   0.7222,    3.1377,   1.6599  },
    {  -80.3092,   -9.6237,  -14.1215,  -23.6260,         0,         0	}
};

double y1_pos_m = 0.0;
double y1_pos_V = 0.0;
double y2_theta_rad = 0.0;
double y2_theta_V = 0.0;

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

u16 calcul(u16 pos_ajile, u16 theta_ajile) {
	/*printk("pos_ajile = %d\t theta_ajile = %d\n", pos_ajile, theta_ajile);*/
	/*probleme :  si theta > 2.2V erreur de commande*/
	//y1_pos_V = ((pos_ajile)/4096.0);
	//y1_pos_m = ((y1_pos_V*.92 ))-.46;

	//y2_theta_V = (theta_ajile)/4096.0;
	//y2_theta_rad = ((y2_theta_V*.698)-.349);
	
	y1_pos_V = ((pos_ajile - 2048.0)*10)/2048.0;
	y1_pos_m = (y1_pos_V*0.46*2)/10; 
	
	y2_theta_V = ((theta_ajile - 2048.0)*5)/2048.0;
	y2_theta_rad = (y2_theta_V * 1.54)/20;
	
	/*y1_pos_m = ((pos_ajile*10)/2048)*(.297/4);*/
	/*y2_theta_rad = ((theta_ajile*10)/2048)*(.054);*/

	/* ATTENTION: SECURITE A NE PAS SUPPRIMER */
	/*if (((y2_theta_rad > 2.8)||(y2_theta_rad < -2.8))) {return 2048;}*/

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
	//commande_ajile = (u16) ((commande + 10) * 4096) / 20;
	commande_ajile = (u16) ((commande*2048)/10)+2048; 
	/*printk("cmd ajile= %d \n", commande_ajile);*/
	return commande_ajile;
}

int start_com(void) {
	printk("In module COMMANDE\n");
	return 0;
}

void stop_com(void) {
	printk("out of module COMMANDE\n");
}

module_init(start_com);
module_exit(stop_com);

EXPORT_SYMBOL(calcul);
