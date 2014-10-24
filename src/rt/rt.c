#include "rt.h"

/*t aq = 32µs */
void aq(long arg) {
	//RTIME temps;
	Acq aq;
	int boucle = 0; 	
	
	while (boucle<100){
		//temps = rt_get_time_ns();
		//rt_sem_wait(&sema);
		aq =doubleAcq();
		printk("[%d]aqLocales\tpos=%d\tangle=%d\n",boucle,aq.position,aq.angle);
		sendAcq(aq);
		boucle++;
		//temps = rt_get_time_ns() - temps;
		//printk("tps exec: %llu",temps);
		rt_task_wait_period();	
	}
}

void interuption(void) {
	msg_CAN msg;
	u16 commande;
	Acq aq;
	msg = receive_CAN();
	if(isAcq(msg)) {
		aq = canToAcq(msg);		
		printk("INT\tpos=%d\tangle=%d\n",aq.position,aq.angle);
		sendCmd(calcul(aq.position, aq.angle));		
	}		
	if(isCmd(msg)) {
		commande = canToCmd(msg);
		printk("INT\tcmd = %d\n",commande);	
		setValue(commande,0);
		/*rt_sem_signal(&sema);*/
	}	
	rt_ack_irq(num_irq); /* acquittement de l'interruption */
}

int rt_start(void) {
	int ierr;
	RTIME now;

	rt_set_oneshot_mode();
	ierr = rt_task_init(&t_aq,aq,0,STACK_SIZE, PRIORITE, USE_FLOAT, 0);
	start_rt_timer(nano2count(TICK_PERIOD));
	now = rt_get_time();
	rt_task_make_periodic(&t_aq, now, nano2count(PERIODE));
	//rt_sem_init(&sema,1);
	
	rt_global_cli(); /* desactivation des IT */
	rt_request_global_irq(num_irq,interuption); /*installation du handler */
	
	rt_startup_irq(num_irq); /* activation de la ligne d'interruption */
	rt_global_sti(); /* re-activation des IT */
	return(0);
}

void rt_stop(void) {
    printk("uninstall 7841 CAN PCI driver RT\n");
    stop_rt_timer();    
    rt_shutdown_irq(num_irq);/* désactivation de l'IT num_irq */
	rt_free_global_irq(num_irq); /* désintallation du handler */
    rt_task_delete(&t_aq);
}

module_init(rt_start);
module_exit(rt_stop);
