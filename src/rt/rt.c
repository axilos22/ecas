#include "rt.h"

static u16 x_acquis, theta_acquis;

void cmd_pendule(long arg) {
	int boucle = 0, commande = 1800;
	/*while(boucle < 10) {*/
	while(1) {
            /*printk("[%d]\tcmd\tcmd = %u\n",boucle,commande);*/
            commande = calcul(x_acquis,theta_acquis);
            printk("[%d]cmd\tcmd=%d\n",boucle,commande);
            setValue(commande,0);           
            boucle++;
            rt_task_wait_period();
		}
}
void acquisition(long arg) {
	int boucle = 0;
	Acq ak;
	/*while(boucle < 20) {*/
	while(1) {
		printk("[%d]acq\tx=%d\ttheta=%d\n",boucle,x_acquis,theta_acquis);
		/*
		startConv();
		wait_EOC();
		setGrandeur(readConv());
		rt_task_wait_period();
		startConv();
		wait_EOC();
		setGrandeur(readConv());
		*/
		ak = doubleAcq();
		x_acquis = ak.position;
		theta_acquis = ak.angle;
		boucle++;
		rt_task_wait_period();
	}
}

int rt_start(void) {

  int ierr;
  RTIME now;
    
   /* creation taches periodiques */
	rt_set_oneshot_mode();
	ierr = rt_task_init(&tache1,cmd_pendule,0,STACK_SIZE, PRIORITE, USE_FLOAT, 0);
	ierr = rt_task_init(&tache2,acquisition,0,STACK_SIZE, PRIORITE, USE_FLOAT, 0);

	start_rt_timer(nano2count(TICK_PERIOD));
	now = rt_get_time();
	rt_task_make_periodic(&tache1, now+nano2count(5000000), nano2count(PERIODE));	/* 5ms de retard */
    rt_task_make_periodic(&tache2, now, nano2count(PERIODE));
	return(0);

}
/*	theta a gauche : 3521
*	theta a droite : 549
*/
void setGrandeur(Conversion conv) {
        if(conv.channel == 1){
            x_acquis = conv.value;
        }else{
            theta_acquis = conv.value;
        }
}

void rt_stop(void) {
    printk("uninstall 7841 CAN PCI driver RT\n");
    stop_rt_timer();    
    rt_task_delete(&tache1);
    rt_task_delete(&tache2);
}

module_init(rt_start);
module_exit(rt_stop);
