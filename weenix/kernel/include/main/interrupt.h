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

#pragma once

#include "kernel.h"
#include "types.h"

#include "main/apic.h"

#define INTR_DIVIDE_BY_ZERO 0x00
#define INTR_INVALID_OPCODE 0x06
#define INTR_GPF 0x0d
#define INTR_PAGE_FAULT 0x0e

#define INTR_PIT 0xf1
#define INTR_APICTIMER 0xf0
#define INTR_KEYBOARD 0xe0
#define INTR_DISK_PRIMARY 0xd0
#define INTR_DISK_SECONDARY 0xd1

/* NOTE: INTR_SYSCALL is not defined here, but is in syscall.h (it must be
 * in a userland-accessible header) */

#define IPL_LOW 0
#define IPL_HIGH (0xff)

typedef struct regs {
        uint32_t r_es, r_ds; /* pushed manually */
        uint32_t r_edi, r_esi, r_ebp, r_esp, r_ebx, r_edx, r_ecx, r_eax; /* pushed by pusha */
        uint32_t r_intr, r_err; /* intr number and error code */
        uint32_t r_eip, r_cs, r_eflags, r_useresp, r_ss; /* pushed by the processor automatically */
} regs_t;

void intr_init();

/* The function pointer which should be implemented by functions
 * which will handle interrupts. These handlers should be registered
 * with the interrupt subsystem via the intr_register function.
 * The regs structure contains the state of the registers saved when
 * the interrupt occured. */
typedef void (*intr_handler_t)(regs_t *regs);

/* Registers an interrupt handler for the given interrupt handler.
 * If another handler had been previously registered for this interrupt
 * number it is returned, otherwise this function returns NULL. It
 * is good practice to assert that this function returns NULL unless
 * it is known that this will not be the case. */
intr_handler_t intr_register(uint8_t intr, intr_handler_t handler);
int32_t intr_map(uint16_t irq, uint8_t intr);

static inline void intr_enable()
{
        __asm__ volatile("sti");
}

static inline void intr_disable()
{
        __asm__ volatile("cli");
}

/* Atomically enables interrupts using the sti
 * instruction and puts the processor into a halted
 * state, this function returns once an interrupt
 * occurs. */
static inline void intr_wait()
{
        /* the sti instruction enables interrupts, however
         * interrupts are not checked for until the next
         * instruction is executed, this means that the following
         * code will not be succeptible to a bug where an
         * interrupt occurs between the sti and hlt commands
         * and does not wake us up from the hlt. */
        __asm__ volatile("sti\n\t"
                         "hlt");
}

/* Sets the interrupt priority level for hardware interrupts.
 * At initialization time devices should detect their individual
 * IPLs and save them for use with this function. IPL_LOW allows
 * all hardware interrupts. IPL_HIGH blocks all hardware interrupts */
static inline void intr_setipl(uint8_t ipl)
{
        apic_setipl(ipl);
}

/* Retreives the current interrupt priority level. */
static inline uint8_t intr_getipl()
{
        return apic_getipl();
}
