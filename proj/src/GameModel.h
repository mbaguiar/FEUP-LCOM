#ifndef __GAMEMODEL_H
#define __GAMEMODEL_H
#include "bitmap.h"

/** @defgroup GameModel GameModel
 * @{
 * Functions that load, draw and init game objects
 */

#define imgPath(img) "/home/proj/res/img/" img ".bmp"

#define themedImgPath(img, theme) "/home/proj/res/img/" img "_" theme ".bmp"

#define MAX_LIVES 10

#define EASY_MAX_XSPEED 3
#define EASY_MIN_XSPEED 2

#define EASY_MAX_YSPEED 6
#define EASY_MIN_YSPEED 4

#define MEDIUM_MAX_XSPEED 3
#define MEDIUM_MIN_XSPEED 3

#define MEDIUM_MAX_YSPEED 7
#define MEDIUM_MIN_YSPEED 5

#define HARD_MAX_XSPEED 4
#define HARD_MIN_XSPEED 3

#define HARD_MAX_YSPEED 8
#define HARD_MIN_YSPEED 6

#define EASY_BONUS_CLICKS 10
#define MEDIUM_BONUS_CLICKS 20
#define HARD_BONUS_CLICKS 30

#define EASY_INITIAL_SPAWN_RATE 60
#define MEDIUM_INITIAL_SPAWN_RATE 50
#define HARD_INITIAL_SPAWN_RATE 40
#define MAX_SPAWN_RATE 20

#define EASY_SPAWN_RATE_EV 5
#define MEDIUM_SPAWN_RATE_EV 6
#define HARD_SPAWN_RATE_EV 5

#define EASY_DEATH_PILL_RATE 0
#define MEDIUM_DEATH_PILL_RATE 5
#define HARD_DEATH_PILL_RATE 10

#define EASY_WRONG_CLICK 0
#define MEDIUM_WRONG_CLICK 0
#define HARD_WRONG_CLICK 1

#define BONUS "bonus"


typedef enum { TYPE_1, TYPE_2, TYPE_3 } pill_type_t;
typedef enum { SET_UP, MAIN_MENU, OPTIONS_MENU, INIT_GAME, GAME, RESUME_GAME, INIT_BONUS, BONUS_LEVEL, GAME_PAUSE, BONUS_PAUSE, MULTI_PLAYER_SYNC, GAME_OVER, GO_WAIT, LEAVE } game_state_t;
typedef enum { EASY, MEDIUM, HARD } game_difficulty_t;
typedef enum { NO_THEME, DAY, NIGHT, CHRISTMAS } game_theme_t;
typedef enum { MULTI_PLAYER, SINGLE_PLAYER, NO_MODE } game_mode_t;

typedef struct {
        int x;
        int y;
        pill_type_t color;
        int height;
        int width;
        Bitmap * view;
        int didDestroyPill;
        void (*draw)();
} Crosshair;

typedef struct {
        Bitmap * background;
        Bitmap * statBar;
} View;

typedef struct {
        int x;
        int y;
        int xspeed;
        int yspeed;
        pill_type_t color;
        int height;
        int width;
        Bitmap * view;
} Pill;

typedef struct {
        int x;
        int y;
        int xSpeed;
        int height;
        int width;
        Bitmap * view;
} Monster;

typedef struct {
        int score;
        int lives;
        int bonus;
        int bonusScore;
        int bonusTimer;
        int autoTheme;
        game_mode_t mode;
        game_state_t state;
        game_difficulty_t difficulty;
        game_theme_t theme;
        unsigned int seed;
        int multiplayerScore;
        int win;
} Status;

typedef struct {
        int x;
        int y;
        int width;
        int height;
        Bitmap * view;
        Bitmap * view2;
} Button;

typedef struct {
        int x;
        int y;
        Bitmap * view;
        Button *singleplayer;
        Button *multiplayer;
        Button *options;
        Button *exit;
} MainMenu;

typedef struct {
        int x;
        int y;
        Bitmap * view;
        Button *resume;
        Button *restart;
        Button *exit;
} PauseMenu;

typedef struct {
        int x;
        int y;
        Bitmap * view;
        Button *autoTheme;
        Button *day;
        Button *night;
        Button *xmas;
        Button *easy;
        Button *medium;
        Button *hard;
        Button *back;
} OptionsMenu;

typedef struct {
        int x;
        int y;
        Bitmap * view;
        Button *restart;
        Button *exit;
} GameOverMenu;

typedef struct {
        int x;
        int y;
        Bitmap *viewWin;
        Bitmap * viewLose;
        Button * exit;
} GameOverMenuMP;

typedef struct {
        int x;
        int y;
        int width;
        int height;
        Bitmap * view;
} Cursor;

// Game variables
Crosshair crosshair;
View view;
Status status;
Monster monster;
PauseMenu pauseMenu;
MainMenu mainMenu;
OptionsMenu optionsMenu;
GameOverMenu gameOverMenu;
GameOverMenuMP gameOverMenuMP;
Cursor cursor;

Pill* pillArr[50];

unsigned int pillCount;

int maxXSpeed;
int minXSpeed;
int maxYSpeed;
int minYSpeed;
int bonusClicks;
int initialSpawnRate;
int deathPillRate;
int wrongColorClick;
int maxSpawnRate;
int spawnRateEvolution;

/**
 * @brief initializes crosshair object
 */
void initCrosshair();
/**
 * @brief initializes game view, background and stat bar
 */
void initView();
/**
 * @brief initializes game status
 */
void initStatus();
/**
 * @brief initializes monster object
 */
void initMonster();
/**
 * @brief initializes pause menu
 */
void initPauseMenu();
/**
 * @brief initializes main menu
 */
void initMainMenu();
/**
 * @brief initializes options menu
 */
void initOptionsMenu();
/**
 * @brief initializes game over menu singleplayer mode
 */
void initGameOverMenu();
/**
 * @brief initializes game over menu multiplayer mode
 */
void initGameOverMenuMP();
/**
 * @brief initializes cursor object
 */
void initCursor();
/**
 * @brief changes crosshair color
 */
void changeColor();
/**
 * @brief changes game theme
 */
void changeTheme();
/**
 * @brief draws crosshair
 */
void drawCrosshair();
/**
 * @brief draws background
 */
void drawBackground();
/**
 * @brief initializes pill object
 * @return pointer to pill that was initialized
 */
Pill * initPill();
/**
 * @brief initializes game bonus pill object
 * @return pointer to pill that was initialized
 */
Pill * initPillBonus();
/**
 * @brief draws pill passed as argument
 * @param p pill to be drawn
 */
void drawPill(Pill * p);
/**
 * @brief draws all pills in the pills array
 */
void drawAllPills();
/**
 * @brief loads all game bitmaps
 */
void loadBitmaps();
/**
 * @brief loads sugarpunch font regular version
 */
void loadFont();
/**
 * @brief loads sugarpunch font small version
 */
void loadFontSmall();
/**
 * @brief loads game bonus pills bitmaps
 */
void loadPillBonus();
/**
 * @brief loads multiplayer buffer bitmaps
 */
void loadMPBuffer();
/**
 * @brief draws current game score
 * @param score Game score
 */
void drawScore(int score);
/**
 * @brief draws game lives on stat bar
 * @param lives Game lives
 */
void drawLives(int lives);
/**
 * @brief draws game status counter on stat bar
 * @param bonusCounter Game bonus counter
 */
void drawBonusCounter(int bonusCounter);
/**
 * @brief draws pause menu
 */
void drawPauseMenu();
/**
 * @brief draws main menu
 */
void drawMainMenu();
/**
 * @brief draws options menu
 */
void drawOptionsMenu();
/**
 * @brief draws game over menu singleplayer mode
 */
void drawGameOverMenu();
/**
 * @brief draws score game over menu
 */
void drawScoreGameOver();
/**
 * @brief draws stat bar
 */
void drawStatBar();
/**
 * @brief draws monster object
 */
void drawMonster();
/**
 * @brief draws cursor object
 */
void drawCursor();
/**
 * @brief loads all themed bitmaps for game theme
 */
void loadThemedBitmaps();
/**
 * @brief frees all themed bitmaps
 */
void freeThemedBitmaps();
/**
 * @brief frees all bitmaps
 */
void freeBitmaps();
/**
 * @brief loads game difficulty variables
 */
void loadGameVariables();
/**
 * @brief draws score in sugarpunch font
 * @param score game score
 * @param x     x position
 * @param y     y position
 */
void drawAerolite(int score, int x, int y);
/**
 * @brief draws current date
 * @param day     current day
 * @param month   current month
 * @param year    current year
 * @param hours   current hour
 * @param minutes current minute
 * @param seconds current second
 */
void drawDate(int day, int month, int year, int hours, int minutes, int seconds);
/**
 * @brief draws game over menu multiplayer mode
 */
void drawGameOverMenuMP();
/**
 * @brief draws multiplayer buffer
 */
void drawMPBuffer();
/**
 * @brief initializes menu buttonn passed as argument
 * @param button button to be initialized
 * @param view1  bitmap corresponding to view1
 * @param view2  bitmap corresponding to view2 (pressed button)
 * @param x      x position
 * @param y      y position
 */
void initButton(Button * button, Bitmap * view1, Bitmap * view2, int x, int y);
/**
 * @brief draws button passed as argument
 * @param button button to be drawn
 */
void drawButton(Button * button);
/**
 * @brief draws view2 of button passed as argument (pressed button)
 * @param button button to be drawn
 */
void drawPressedButton(Button * button);
/**
 * @brief frees menu buttons
 */
void freeMenuButtons();
/**@}*/
int unsubscribe();
#endif
