#include "rt.h"

static u16 x_acquis, theta_acquis;

void tacheP1(long arg) {
	int commande, boucle=0;
	Conversion conv;
	printk("Debut de la sequence ------------------------------------------------\n");
	while(boucle<10000) {
			setChannelScan(0,1);
			startConv();
			rt_busy_sleep(100000);
			conv = readConv();
			printk("[%d]chan%d = %d\n",boucle,conv.channel,conv.value);
			setGrandeur(conv);
			startConv();
			rt_busy_sleep(100000);
			conv = readConv();
			printk("[%d]chan%d = %d\n",boucle,conv.channel,conv.value);
			setGrandeur(conv);
            commande = calcul(x_acquis,theta_acquis);
            /*printk("[%d]Commande : %d\n",boucle,commande);*/
            setValue(commande,0);
            boucle++;
            rt_task_wait_period();
		}
}

void setGrandeur(Conversion conv) {
    #if (CONF == 1)
        if(conv.channel == 0){
            x_acquis = conv.value;
        }
        if(conv.channel == 1) {
            theta_acquis = conv.value;
        }
    #else
        if(conv.channel == 1){
            x_acquis = conv.value;
        }
        if(conv.channel == 0) {
            theta_acquis = conv.value;
        }
    #endif
}

/*
 * tache d'acquisistion periodique sur les channel 0 et 1 
 */
void acquisition(long arg) {
	Conversion conv;
	while(1) {
		setChannel(0);
		startConv();
		conv = readConv();
		setGrandeur(conv);
		rt_task_wait_period();
		setChannel(1);
		startConv();
		conv = readConv();
		setGrandeur(conv);
		rt_task_wait_period();
	}
}

int rt_start(void) {

  int ierr;
  RTIME now;
   /* creation taches periodiques */
	rt_set_oneshot_mode();
	ierr = rt_task_init(&tache1,tacheP1,0,STACK_SIZE, PRIORITE, USE_FLOAT, 0);
	/*ierr = rt_task_init(&tache2,tacheP2,0,STACK_SIZE, PRIORITE, 0, 0);*/

	start_rt_timer(nano2count(TICK_PERIOD));
	now = rt_get_time();

	rt_task_make_periodic(&tache1, now, nano2count(PERIODE));
    /*rt_task_make_periodic(&tache2, now+nano2count(6000000), nano2count(PERIODE));*/
	enableOutput();
 return(0);

}

void rt_stop(void) {
    printk("uninstall 7841 CAN PCI driver RT\n");
    stop_rt_timer();
    disableOutput();
    rt_task_delete(&tache1);
    /*rt_task_delete(&tache2);*/
}

module_init(rt_start);
module_exit(rt_stop);
