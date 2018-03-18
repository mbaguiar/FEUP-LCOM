#include <errno.h>
#include <minix/drivers.h>
#include "Game.h"



int main(int argc, char **argv){

								sef_startup();

								sys_enable_iop(SELF);

								gameStart();

								return 0;

}
