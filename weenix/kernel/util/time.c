/******************************************************************************/
/* Important Spring 2017 CSCI 402 usage information:                          */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5f2e8d450c0c5851acd538befe33744efca0f1c4f9fb5f       */
/*         3c8feabc561a99e53d4d21951738da923cd1c7bbd11b30a1afb11172f80b       */
/*         984b1acfbbf8fae6ea57e0583d2610a618379293cb1de8e1e9d07e6287e8       */
/*         de7e82f3d48866aa2009b599e92c852f7dbf7a6e573f1c7228ca34b9f368       */
/*         faaef0c0fcf294cb                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

#include "globals.h"

#include "main/interrupt.h"
#include "main/apic.h"
#include "main/pit.h"

#include "util/debug.h"
#include "util/init.h"

#include "proc/sched.h"
#include "proc/kthread.h"

#define APIC_TIMER_IRQ 32 /* Map interrupt 32 */

#ifdef __UPREEMPT__
static unsigned int ms = 0;

static void pit_handler(regs_t *regs)
{
  dbg(DBG_CORE, "PIT HANDLER FIRED\n");
/* UPREEMPT BLANK {{{ */
        if (0 == ++ms % 10) {
                /* If we're in userland */
                if (0x3 == (regs->r_cs & 0x3)) {
                        /* Check for cancellation */
                        if (curthr->kt_cancelled) {
                                dbg(DBG_CORE, ("Preempt: In userland and cancelled, exiting\n"));
                                apic_eoi();
                                kthread_exit(curthr->kt_retval);
                        }

                        dbg(DBG_CORE, ("Preempt: In userland, switching\n"));
                        sched_make_runnable(curthr);
                        sched_switch();
                } else {
                        dbg(DBG_CORE, ("Preempt: in kernel, skipping\n"));
                }
        }
/* UPREEMPT BLANK }}} */
}

/* Uncomment this to enable the apic timer to 
 * call the pit_handler for userland preemption
 */

static __attribute__((unused)) void time_init(void)
{
  intr_map(APIC_TIMER_IRQ, APIC_TIMER_IRQ);
  intr_register(APIC_TIMER_IRQ, pit_handler);
  /* TODO: figure out how this argument converts to hertz */
  apic_enable_periodic_timer(8);
}
init_func(time_init);

#endif
