#include "video_gr.h"
#include "video_test.h"

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

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

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
  unsigned int vram_size = h_res * v_res * (bits_per_pixel/8);  /* VRAM's size, but you can use
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

   return video_mem;
}

unsigned long convertX(unsigned short x){
    return x+h_res/2;
}

unsigned long convertY(unsigned short y){
    return y+v_res/2;
}

unsigned long getArrayCords(unsigned short x, unsigned short y){

    return (y * h_res) + x;
}

int fillPixel(long x, long y, unsigned color) {
    if (x < h_res && y < v_res && x >= 0 && y >= 0)
    video_mem[getArrayCords(x, y)] = color;
    return 0;

}

int vg_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color){

    long i, j, realX, realY;

    realX = convertX(x);
    realY = convertY(y);

    for (i = realY-size/2; i < realY+size/2; i++){
      for (j = realX-size/2; j < realX+size/2; j++){
          fillPixel(j, i, color);
        }
    }
    video_dump_fb();
    return 0;
}

int vg_line(int xi, int yi, int xf, int yf, int color){

    //adapted from digital differencial analyzer algorithm
    //https://www.tutorialspoint.com/computer_graphics/line_generation_algorithm.htm

    int dx, dy, iterations, i;
    float xin, yin;
    float x, y;

    //calculate the differnce between two end points
    dx = xf - xi;
    dy = yf - yi;

    if (dx == 0 && dy == 0){
        fillPixel(xi, yi, color);
        return 0;
    } 

    if (dx == 0){
        iterations = abs(dy);
        xin = 0;
        yin = iterations/dy; // == 1 || -1
    } else {

        if (abs(dy)>abs(dx)){
            iterations = abs(dy);
            xin = dx/(float) iterations; 
            yin = iterations/dy; // == 1 || -1
        } else {
            iterations = abs(dx);
            xin = iterations/dx; // == 1 || -1
            yin = dy/(float) iterations;
        }
    }

    x = xi;
    y = yi;

    for (i = 0; i < iterations; i++) {

      x = x + xin;
      y = y + yin;
      fillPixel((long) x, (long) y, color);
    }

    video_dump_fb();

    return 0;
}

int vg_xpm(char *xpm[], unsigned short xi, unsigned short yi){
    Sprite * sp = create_sprite(xpm, xi, yi, 0, 0);

    return draw_sprite(sp, video_mem);

}

char * getVideoMem() {
  return video_mem;
}

int getHRES(){
    return h_res;
}

int getVRES(){
    return v_res;
}