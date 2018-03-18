#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"
#include "video_test.h"

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

    mmap_t buf;
    struct reg86u r;

    if (lm_init() == NULL){
        print_dbg("lm_init() failed\n");
        print_location();
        return 1;
    }

    if(lm_alloc(sizeof(vbe_mode_info_t), &buf) == NULL){
        print_dbg("lm_alloc() failed\n");
        print_location();
        return 1;
    }

    r.u.w.ax = VBE_MODE_INFO_FUNC;         /* VBE get mode info */
    /* translate the buffer linear address to a far pointer */
    r.u.w.es = PB2BASE(buf.phys);    /* set a segment base */
    r.u.w.di = PB2OFF(buf.phys);     /* set the offset accordingly */
    r.u.w.cx = mode;
    r.u.b.intno = VBE_INT_NO;
    if( sys_int86(&r) != OK ){ /* call BIOS  */
        panic("get_vbe_mode_info: sys_int86() failed \n");
    }

    *vmi_p = *(vbe_mode_info_t*) buf.virtual;

    lm_free(&buf);

  return 0;
}

int vbe_get_controller_info(vbe_info_block_t * vib_p){

    struct reg86u r;
    mmap_t buf;

    if (lm_init() == NULL){
        print_dbg("lm_init() failed\n");
        print_location();
        return 1;
    }

    if(lm_alloc(sizeof(vbe_mode_info_t), &buf) == NULL){
        print_dbg("lm_alloc() failed\n");
        print_location();
        return 1;
    }

    r.u.w.ax = VBE_CNTR_INFO_FUNC;
    r.u.w.es = PB2BASE(buf.phys);    /* set a segment base */
    r.u.w.di = PB2OFF(buf.phys);     /* set the offset accordingly */
    r.u.b.intno = VBE_INT_NO;
    if( sys_int86(&r) != OK ){ /* call BIOS  */
        panic("get_vbe_controller_info: sys_int86() failed \n");
    }

    *vib_p = *(vbe_info_block_t*) buf.virtual;

    lm_free(&buf);

    return 0;

}
