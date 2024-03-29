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

#include "proc/sched.h"

#include "mm/mmobj.h"

#include "util/list.h"
#include "util/init.h"

struct mmobj;

#define PF_BUSY                 0x01
#define PF_DIRTY                0x02

#define pframe_is_busy(pf)          ((pf)->pf_flags & PF_BUSY)
#define pframe_set_busy(pf)         do { (pf)->pf_flags |= PF_BUSY; } while (0)
#define pframe_clear_busy(pf)       do { (pf)->pf_flags &= ~PF_BUSY; } while (0)

#define pframe_is_dirty(pf)         ((pf)->pf_flags & PF_DIRTY)
#define pframe_set_dirty(pf)        do { (pf)->pf_flags |= PF_DIRTY; } while (0)
#define pframe_clear_dirty(pf)      do { (pf)->pf_flags &= ~PF_DIRTY; } while (0)

#define pframe_is_pinned(pf)        ((pf)->pf_pincount)
#define pframe_is_free(pf)          (!(pf)->pf_obj)

/* A pframe structure represents a page frame in physical memory available to the
 * kernel. pframes are managed by mmobjs */
typedef struct pframe {
        /* Public read: (do not modify outside pframe.c) */

        /* Object and page number, which together uniquely identify the page */
        struct mmobj       *pf_obj;
        uint32_t            pf_pagenum;

        /*   The address of the page frame. Note that this is NOT a
         *   physical address, but is a virtual address in the kernel's memory
         *   map (i.e., it will be higher than 0xc0000000) */
        void               *pf_addr;

        /* Private: */
        uint8_t             pf_flags;    /* PF_DIRTY, PF_BUSY */
        ktqueue_t           pf_waitq;    /* wait on this if page is busy */
        int                 pf_pincount;
        list_link_t         pf_link;     /* link on {free,allocated,pinned}_list */
        list_link_t         pf_hlink;    /* link on hash chain of resident page hash */
        list_link_t         pf_olink;    /* link on object's list of resident pages */
} pframe_t;

void pframe_init(void);
void pframe_add_range(uint32_t startpfn, uint32_t endpfn);
void pframe_pageoutd_init(void);

void pframe_shutdown(void);

pframe_t *pframe_get_resident(struct mmobj *o, uint32_t pagenum);

int pframe_lookup(struct mmobj *o, uint32_t pagenum, int forwrite, pframe_t **result);
void pframe_migrate(pframe_t *pf, mmobj_t *dest);
int pframe_get(struct mmobj *o, uint32_t pagenum, pframe_t **result);

void pframe_pin(pframe_t *pf);
void pframe_unpin(pframe_t *pf);

int  pframe_dirty(pframe_t *pf);
int  pframe_clean(pframe_t *pf);
void pframe_free(pframe_t *pf);

void pframe_clean_all(void);

void pframe_remove_from_pts(pframe_t *pf);
