#include <linux/init.h>
#include <linux/module.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <linux/pci.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>

#include <rtai_sem.h>   /* RTAI semaphores */

MODULE_LICENSE("GPL");


