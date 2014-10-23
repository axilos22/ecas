#include "rt.h"

void aq(long arg) {
	Acq aq;
	int boucle = 0; 	
	while (1){
		aq =doubleAcq();
		//printk("[%d]aqLocales\tpos=%d\tangle=%d\n",boucle,aq.position,aq.angle);
		sendAcq(aq);
		boucle++;
		rt_task_wait_period();	
	}
}

void cmd(long arg) {
	msg_CAN msg; Acq aq;
	int boucle = 0;
		
	while(1) {
		msg = receive_CAN();
		if(isAcq(msg)) {
			aq = canToAcq(msg);		
			//printk("[%d]aqCan\tpos=%d\tangle=%d\n",boucle,aq.position,aq.angle);
			sendCmd(calcul(aq.position, aq.angle));
		}		
		boucle++;
		rt_task_wait_period();
	}
	
}

void can(long arg) {
	msg_CAN msg;
	u16 commande;
	int boucle = 0;
	
	while(1) {
		msg = receive_CAN();
		if(isCmd(msg)) {
			commande = canToCmd(msg);
			//printk("[%d]cmd = %d\n",boucle,commande);	
			setValue(commande,0);			
		}		
		boucle++;
		rt_task_wait_period();			
	}
}

int rt_start(void) {
	int ierr;
	RTIME now;

	/* creation taches periodiques */
	rt_set_oneshot_mode();
	ierr = rt_task_init(&t_aq,aq  ,0,STACK_SIZE, PRIORITE, USE_FLOAT, 0);
	ierr = rt_task_init(&t_cmd,cmd,0,STACK_SIZE, PRIORITE, USE_FLOAT, 0);
	ierr = rt_task_init(&t_can,can,0,STACK_SIZE, PRIORITE, USE_FLOAT, 0);

	start_rt_timer(nano2count(TICK_PERIOD));
	now = rt_get_time();
	rt_task_make_periodic(&t_aq, now+nano2count(5000000), nano2count(PERIODE));
	rt_task_make_periodic(&t_cmd, now+nano2count(2500000), nano2count(PERIODE));
	rt_task_make_periodic(&t_can, now, nano2count(PERIODE));
	//rt_task_make_periodic(&t_aq, now, nano2count(PERIODE));
	//rt_task_make_periodic(&t_cmd, now+nano2count(2500000), nano2count(PERIODE));	/* 2,5ms de retard */
	//rt_task_make_periodic(&t_can, now+nano2count(5000000), nano2count(PERIODE)); /* 5ms de retard */
	return(0);
}

void rt_stop(void) {
    printk("uninstall 7841 CAN PCI driver RT\n");
    stop_rt_timer();    
    rt_task_delete(&t_aq);
    rt_task_delete(&t_cmd);
    rt_task_delete(&t_can);
}

module_init(rt_start);
module_exit(rt_stop);
