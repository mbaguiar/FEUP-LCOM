#include "video_gr.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES             1024
#define V_RES		       768
#define BITS_PER_PIXEL	    8

/* Private global variables */

static short *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static short *dBuffer;

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned int vram_size;

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = VBE_INT_NO; /* BIOS video services */

  reg86.u.b.ah = SET_VIDEO_MODE;    /* Set Video Mode function */
  reg86.u.b.al = TEXT_MODE_80x25;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

void* vg_init(unsigned short mode){

vbe_mode_info_t vmi;

struct reg86u re;
    re.u.w.ax = VBE_SET_MODE_FUNC; // VBE call, function 02 -- set VBE mode
    re.u.w.bx = BIT(LINEAR_MODEL_BIT)|mode; // set bit 14: linear framebuffer
    re.u.b.intno = VBE_INT_NO;

    if( sys_int86(&re) != 0) {
      panic("set_vbe_mode: sys_int86() failed \n");
    }

    if (vbe_get_mode_info(mode, &vmi) != 0){
        print_dbg("vbe_get_mode_info failed\n");
        print_location();
        return NULL;
    }

  h_res = vmi.XResolution;
  v_res = vmi.YResolution;
  bits_per_pixel = vmi.BitsPerPixel;

  int r;
  struct mem_range mr;
  unsigned int vram_base = (phys_bytes) vmi.PhysBasePtr;  /* VRAM's physical addresss */
  vram_size = h_res * v_res * (bits_per_pixel/8);  /* VRAM's size, but you can use
                              the frame-buffer size, instead */

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
     panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
   panic("couldn't map video memory");

  dBuffer = (short *) malloc(vram_size);

   return video_mem;
}

short * getVideoMem() {
  return video_mem;
}

int getHRES(){
    return h_res;
}

int getVRES(){
    return v_res;
}

short* getDoubleBuffer() {
	return dBuffer;
}

int getVramSize() {
	return vram_size;
}
