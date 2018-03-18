#ifndef __GAMECTRL_H
#define __GAMECTRL_H
#include  <stdio.h>
#include "GameModel.h"
#include "MultiplayerCtrl.h"
#include "mouse.h"
#include "rtc.h"
#include "i8042.h"

/** @defgroup GameCtrl GameCtrl
 * @{
 * Functions for the game logic and that make the game evolve
 */

typedef enum {MOUSE_COLLISION, REFLECTION_COLLISION, BOUNDARIES_COLLISION, NO_COLLISION, MONSTER_COLLISION} collision_evt_t;
typedef enum {PRESSED, NOT_PRESSED} k_pressed_t;

typedef struct {
int x;
int y;
int lb;
int rb;
int didPressButton;
} mouse_t;

typedef struct {
  k_pressed_t a;
  k_pressed_t d;
  k_pressed_t q;
} keyboard_t;

/**
 * @brief initializes game controller variables and sets interrupts listeners
 */
void gameCtrl_init();
/**
 * @brief initializes mouse object
 */
void mouse_init();
/**
 * @brief initializes keyboard object
 */
void keyboard_init();
/**
 * @brief updates game theme based on current date
 */
void updateTheme();
/**
 * @brief process information received by moouse's interrupts
 * @param dx value of mouse deslocation in x
 * @param dy value of mouse deslocation in y
 * @param rb 1 if mouse right button is pressed and 0 otherwise
 * @param lb 1 if mouse left button is pressed and 0 otherwise
 */
void mouseProcess(int dx, int dy, int rb, int lb);
/**
 * @breif called in each time's interrupts, calls all the functions that make the game evolve
 */
void timerIH();
/**
 * @brief process information received by keyboard's interrupts
 * @param scancode key scancode
 */
void keyboardProcess(unsigned long scancode);
/**
 * @brief process information received by rtc's interrupts
 * @param date current date
 */
void rtcProcess(Date date);
/**
 * @brief creates game pill
 */
void createPill();
/**
 * @brief creates bonus game pill
 */
void createBonusPill();
/**
 * @brief cleans pill array
 */
void resetPills();
/**
 * @brief manages all pills
 */
void pillManager();
/**
 * @brief manages monsters object
 */
void monsterManager();
/**
 * @brief updates monster position
 */
void updateMonster();
/**
 * @brief resets monster position
 */
void resetMonster();
/**
 * @brief removes pill from pill array
 * @param idx index of pill to be removed
 */
void removePill(unsigned int idx);
/**
 * @brief checks collision for pill passed as argument
 * @param  p pill to check collision
 * @return returns type of collision
 */
collision_evt_t testCollision(Pill* p);
/**
 * @brief tests collisions for all pills in pill array
 */
void testCollisionAllPills();
/**
 * @brief updates postion of pill passed as argument
 * @param p pill
 */
void updatePill(Pill * p);
/**
 * @brief updates all pills in pill array
 */
void updateAllPills();
/**
 * @brief updates game state
 */
void statusManager();
/**
 * @brief leaves game
 */
void gameLeave();
/**
 * @brief singleplayer mode event manager
 */
void singleplayerEventManager();
/**
 * @brief singleplayer mode status manager - updates game state
 */
void singleplayerStatusManager();
/**
 * @brief multiplayer mode status manager - update game state
 */
void multiplayerEventManager();
/**
 * @brief multiplayer mode event manager
 */
void multiplayerStatusManager();
/**
 * @brief game event manager
 */
void eventManager();
/**
 * @brief spawn pill manager
 */
void spawnManager();
/**
 * @brief pause conditions manager
 */
void pauseManager();
/**
 * @brief checks if any pause menu button is selected
 */
void pauseMenuOptions();
/**
 * @brief checks if any main menu button is selected
 */
void mainMenuOptions();
/**
 * @brief checks if any options menu button is selected
 */
void optionsMenuOptions();
/**
 * @brief checks if any options menu button is selected
 */
void gameOverMenuOptions();
/**
 * checks if the button passed as argument is selected
 * @param  b button to be checked
 * @return returns 1 if button is selected and 0 otherwise
 */
int buttonCollision(Button *b);
/**
 * @brief generates game seed based on current date
 * @return returns seed generated
 */
unsigned int generateSeed();
/**
 * @brief checks serial port messages and connection
 * @return returns 1 if received a message and 0 otherwise
 */
int checkSerialPort();
/**
 * @brief pause menu mouse hover draw
 */
void pauseMenuMouseHover();
/**
 * @brief main menu mouse hover draw
 */
void mainMenuMouseHover();
/**
 * @brief options menu mouse hover draw
 */
void optionsMenuMouseHover();
/**
 * @brief game over menu - singleplayer mode mouse hover draw
 */
void gameOverMenuMouseHover();
/**
 * @brief game over menu - multiplayer mode mouse hover draw
 */
void gameOverMenuMPMouseHover();
/**
 * @brief updates crosshair position
 * @param x x position
 * @param y y position
 */
void updateCrosshair(int x, int y);
/**
 * @brief updates cursor position
 * @param x x position
 * @param y y position
 */
void updateCursor(int x, int y);
/**
 * @brief checks if any game over menu - multiplayer mode is selected
 */
void gameOverMenuMPOptions();
/**@}*/
#endif
