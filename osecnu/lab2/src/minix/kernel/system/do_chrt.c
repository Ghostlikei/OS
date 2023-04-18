#include "kernel/system.h"
#include <minix/endpoint.h>
#include "kernel/vm.h"
#include <signal.h>
#include <string.h>
#include <assert.h>
#include <minix/u64.h>


#if USE_CHRT

/*===========================================================================*
  *                                do_chrt                                   *
  *===========================================================================*/

int do_chrt(struct proc *caller, message *m_ptr){
    struct proc *rp;

    rp = proc_addr(m_ptr->m2_i1);
    rp->p_ddl = m_ptr->m2_l1;
    return (OK);
}
#endif /* USE_CHRT */