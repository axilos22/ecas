#include "rt.h"

Acq aquisition;
u16 commande;

void acq(long arg) {
	int boucle = 0; 
	while (1){
		aquisition = doubleAcq();
		boucle++;
		rt_task_wait_period();
	}
}

void emission(long args) {
	while(1) {
		sendAcq(aquisition);
		rt_sleep(nano2count(1000000)); /*1ms d attente passive*/
		sendCmd(commande);
		rt_task_wait_period();
	}
}

void commande(long args) {	
	while(1) {
		commande = calcul(aquisition.position, aquisition.angle);
		setValue(commande,0);
		rt_task_wait_period();
	}
}

int rt_start(void) {

	int ierr;
	RTIME now;
	aquisition.position = 2048;
	aquisition.angle = 2048;
	commande = 2048;
    
	/* creation taches periodiques */
	rt_set_oneshot_mode();
	ierr = rt_task_init(&t_aq,acq,0,STACK_SIZE, PRIORITE_MAX, USE_FLOAT, 0);
	ierr = rt_task_init(&t_em,emission,0,STACK_SIZE, 2, USE_FLOAT, 0);
	ierr = rt_task_init(&t_cmd,commande,0,STACK_SIZE, PRIORITE, USE_FLOAT, 0);

	start_rt_timer(nano2count(TICK_PERIOD));
	now = rt_get_time();
	rt_task_make_periodic(&t_aq, now, nano2count(PERIODE));
	rt_task_make_periodic(&t_em, now+nano2count(2500000), nano2count(PERIODE));	/* 2,5ms de retard */
	rt_task_make_periodic(&t_cmd, now+nano2count(5000000), nano2count(PERIODE)); /* 5ms de retard */
	return(0);
}

void rt_stop(void) {
    printk("uninstall 7841 CAN PCI driver RT\n");
    stop_rt_timer();    
    rt_task_delete(&tache1);
    //rt_task_delete(&tache2);
}

module_init(rt_start);
module_exit(rt_stop);
