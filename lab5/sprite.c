/** Creates a new sprite with pixmap "pic", with specified position (within the screen limits) and speed;
* Does not draw the sprite on the screen
* Returns NULL on invalid pixmap.
*/
#include "sprite.h"
#include "video_test.h"


Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed) {
        //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
    if( sp == NULL ) return NULL;

    sp->x = x;
    sp->y = y;

    sp->xspeed = xspeed;
    sp->yspeed = yspeed;

    // read the sprite pixmap
    sp->map = read_xpm(pic, &(sp->width), &(sp->height));
    if( sp->map == NULL ) {
        free(sp);
        return NULL;
    }
    return sp;
}


void destroy_sprite(Sprite *sp) {
    if( sp == NULL ) return;
    if( sp ->map ) free(sp->map);
    free(sp);
    sp = NULL;     // XXX: pointer is passed by value
                    //           should do this @ the caller
}
int animate_sprite(Sprite *sp) {
        return 0;
}

    /* Some useful non-visible functions */
int draw_sprite(Sprite *sp, char *base) {

     long i, j;

    for (i = 0; i < sp->height; i++){
        for (j = 0; j< sp->width; j++){
            fillPixel(sp->x+j, sp->y+i, sp->map[i*sp->width + j]);
        }
    }

    video_dump_fb();

    return 0;

}

int erase_sprite(Sprite *sp) {

    unsigned long x, y;
    x = sp->x;
    y = sp->y;

    unsigned short i, j;
    for (i = x; i < x + sp->width; i++) {
      for (j = y; j < y + sp->height; j++) {
        fillPixel(i, j, 0);
      }
    }

    return 0;
}

int sprite_move(Sprite * sprite, int xf, int yf) {

  char * video_mem = getVideoMem();

  erase_sprite(sprite);
  sprite->x = sprite->x + sprite->xspeed;

  sprite->y = sprite->y + sprite->yspeed;

  if ((sprite->xspeed > 0) && (sprite->x > xf)) sprite->x = xf;
  else if ((sprite->xspeed < 0) && (sprite->x < xf)) sprite->x = xf;

  if ((sprite->yspeed > 0) && (sprite->y > yf)) sprite->y = yf;
  else if ((sprite->yspeed < 0) && (sprite->y < yf)) sprite->y = yf;

  draw_sprite(sprite, video_mem);
  return 0;
}
