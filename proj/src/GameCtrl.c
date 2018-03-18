#include "GameCtrl.h"

static mouse_t mouse;
static keyboard_t keyboard;
static Date date;
static unsigned long timerCounter = 0;

void gameCtrl_init(){
        initStatus();
        mouse_init();
        keyboard_init();
        date = getCurrentDate();
        setMouseListener(mouseProcess);
        setKeyboardListener(keyboardProcess);
        setRTCListener(rtcProcess);
        setTimerListener(timerIH);
}

void mouse_init() {
        mouse.x = getHRES()/2;
        mouse.y = getVRES()/2;
        mouse.lb = 0;
        mouse.rb = 0;
        mouse.didPressButton = 0;
}

void keyboard_init() {
        keyboard.a = NOT_PRESSED;
        keyboard.d = NOT_PRESSED;
        keyboard.q = NOT_PRESSED;
}

void createPill(){
        pillArr[pillCount] = (Pill*) initPill();
        pillCount++;
        //printf("Pill %u created\n", pillCount);
}

void createPillBonus() {
        pillArr[pillCount] = (Pill*) initPillBonus();
        pillCount++;
}

void updateCrosshair(int x, int y) {
        if (x >= getHRES() - crosshair.width)
                crosshair.x = getHRES() - crosshair.width;
        else crosshair.x = x;
        if (y >= getVRES() - cursor.height)
                crosshair.y = getVRES() - crosshair.height;
        else crosshair.y = y;
}

void updateCursor(int x, int y) {
        if (x >= getHRES() - cursor.width)
                cursor.x = getHRES() - cursor.width;
        else cursor.x = x;
        if (y >= getVRES() - cursor.height)
                cursor.y = getVRES() - cursor.height;
        else cursor.y = y;
}

void mouseProcess(int dx, int dy, int rb, int lb){

        if (!(status.mode == MULTI_PLAYER && status.state == BONUS_LEVEL)) {

                mouse.x += dx;
                mouse.y += (0-dy);
                if (mouse.x < 0) mouse.x = 0;
                else if (mouse.x > getHRES()) mouse.x = getHRES();
                if (mouse.y < 0) mouse.y = 0;
                else if (mouse.y > getVRES()) mouse.y = getVRES();

                updateCursor(mouse.x, mouse.y);
                updateCrosshair(mouse.x, mouse.y);
        }

        if (rb == 1 && mouse.rb == 0) changeColor();
        mouse.rb = rb;
        if (lb == 0 && crosshair.didDestroyPill == 1) crosshair.didDestroyPill = 0;
        if (lb == 0 && mouse.didPressButton == 1) mouse.didPressButton = 0;
        mouse.lb = lb;
}

void rtcProcess(Date d) {
        date = d;
        //printf("%lu-%lu-%lu  %lu:%lu:%lu\n", d.year, d.month, d.day, d.hour, d.minutes, d.seconds);
}

void timerIH() {
        timerCounter++;
        eventManager();
        updateFrameBuffer();
}

void keyboardProcess(unsigned long scancode) {
        if (scancode == Q_PRESSED) keyboard.q = PRESSED;
        if (scancode == Q_NOT_PRESSED) keyboard.q = NOT_PRESSED;
        if (scancode == A_NOT_PRESSED) keyboard.a = NOT_PRESSED;
        if (scancode == A_PRESSED) keyboard.a = PRESSED;
        if (scancode == D_PRESSED) keyboard.d = PRESSED;
        if (scancode == D_NOT_PRESSED) keyboard.d = NOT_PRESSED;
        if (scancode == ESC_PRESSED && status.mode == SINGLE_PLAYER) pauseManager();
}

void updateTheme() {
        game_theme_t theme;
        if (date.month == 12 || (date.month == 1 && date.day >= 1 && date.day <= 6)) {
                theme = CHRISTMAS;
        } else if (date.hour >= 19 || (date.hour >= 0 && date.hour <= 6)) {
                theme = NIGHT;
        } else {
                theme = DAY;
        }

        if (theme != status.theme) {
                status.theme = theme;
                changeTheme();
        }
}

void updatePill(Pill * p){
        if (p != NULL) {
                p->x += p->xspeed;
                p->y += p->yspeed;
        }
}

void pauseManager() {
        if (status.state == GAME) status.state = GAME_PAUSE;
        else if (status.state == BONUS_LEVEL) status.state = BONUS_PAUSE;
        else if (status.state == GAME_PAUSE) status.state = GAME;
        else if (status.state == BONUS_PAUSE) status.state = BONUS_LEVEL;
}

int buttonCollision(Button *b) {
        return (cursor.x < b->x + b->width && cursor.x > b->x && cursor.y < b->y + b->height && cursor.y > b->y);
}

void pauseMenuOptions() {
        if (mouse.lb && !mouse.didPressButton) {
                if (buttonCollision(pauseMenu.restart)) {
                        status.state = INIT_GAME;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(pauseMenu.resume)) {
                        crosshair.didDestroyPill = 1;
                        if (status.state == GAME_PAUSE)
                                status.state = GAME;
                        else if (status.state == BONUS_PAUSE)
                                status.state = BONUS_LEVEL;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(pauseMenu.exit)) {
                        status.mode = NO_MODE;
                        status.state = MAIN_MENU;
                        mouse.didPressButton = 1;
                        return;
                }
        }
}

void pauseMenuMouseHover() {
        if (buttonCollision(pauseMenu.restart)) drawPressedButton(pauseMenu.restart);
        if (buttonCollision(pauseMenu.resume)) drawPressedButton(pauseMenu.resume);
        if (buttonCollision(pauseMenu.exit)) drawPressedButton(pauseMenu.exit);
}

void mainMenuMouseHover() {
        if (buttonCollision(mainMenu.singleplayer)) drawPressedButton(mainMenu.singleplayer);
        if (buttonCollision(mainMenu.multiplayer)) drawPressedButton(mainMenu.multiplayer);
        if (buttonCollision(mainMenu.options)) drawPressedButton(mainMenu.options);
        if (buttonCollision(mainMenu.exit)) drawPressedButton(mainMenu.exit);
}

void mainMenuOptions() {
        if (mouse.lb && !mouse.didPressButton) {
                if (buttonCollision(mainMenu.singleplayer)) {
                        status.mode = SINGLE_PLAYER;
                        status.state = INIT_GAME;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(mainMenu.multiplayer)) {
                        status.mode = MULTI_PLAYER;
                        status.state = MULTI_PLAYER_SYNC;
                        enterLobby();
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(mainMenu.options)) {
                        status.state = OPTIONS_MENU;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(mainMenu.exit)) {
                        status.state = LEAVE;
                        mouse.didPressButton = 1;
                        return;
                }
        }
}

void optionsMenuMouseHover() {
        if (buttonCollision(optionsMenu.autoTheme)) drawPressedButton(optionsMenu.autoTheme);
        if (buttonCollision(optionsMenu.easy)) drawPressedButton(optionsMenu.easy);
        if (buttonCollision(optionsMenu.medium)) drawPressedButton(optionsMenu.medium);
        if (buttonCollision(optionsMenu.hard)) drawPressedButton(optionsMenu.hard);
        if (buttonCollision(optionsMenu.day)) drawPressedButton(optionsMenu.day);
        if (buttonCollision(optionsMenu.night)) drawPressedButton(optionsMenu.night);
        if (buttonCollision(optionsMenu.xmas)) drawPressedButton(optionsMenu.xmas);
        if (buttonCollision(optionsMenu.back)) drawPressedButton(optionsMenu.back);
}

void optionsMenuOptions() {
        if (mouse.lb && !mouse.didPressButton) {
                if (buttonCollision(optionsMenu.autoTheme)) {
                        updateTheme();
                        status.autoTheme = 1;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(optionsMenu.day)) {
                        status.theme = DAY;
                        changeTheme();
                        status.autoTheme = 0;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(optionsMenu.night)) {
                        status.theme = NIGHT;
                        changeTheme();
                        status.autoTheme = 0;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(optionsMenu.xmas)) {
                        status.theme = CHRISTMAS;
                        changeTheme();
                        status.autoTheme = 0;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(optionsMenu.easy)) {
                        status.difficulty = EASY;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(optionsMenu.medium)) {
                        status.difficulty = MEDIUM;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(optionsMenu.hard)) {
                        status.difficulty = HARD;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(optionsMenu.back)) {
                        status.state = MAIN_MENU;
                        mouse.didPressButton = 1;
                        return;
                }
        }
}

void gameOverMenuMouseHover() {
        if (buttonCollision(gameOverMenu.restart)) drawPressedButton(gameOverMenu.restart);
        if (buttonCollision(gameOverMenu.exit)) drawPressedButton(gameOverMenu.exit);
}

void gameOverMenuOptions() {
        if (mouse.lb && !mouse.didPressButton) {
                if (buttonCollision(gameOverMenu.restart)) {
                        status.state = INIT_GAME;
                        mouse.didPressButton = 1;
                        return;
                }
                if (buttonCollision(gameOverMenu.exit)) {
                        status.mode = NO_MODE;
                        status.state = MAIN_MENU;
                        mouse.didPressButton = 1;
                        return;
                }
        }
}

void gameOverMenuMPMouseHover() {
        if (buttonCollision(gameOverMenuMP.exit)) drawPressedButton(gameOverMenuMP.exit);
}

void gameOverMenuMPOptions() {
        if (mouse.lb && !mouse.didPressButton) {
                if (buttonCollision(gameOverMenuMP.exit)) {
                        status.mode = NO_MODE;
                        status.state = MAIN_MENU;
                        mouse.didPressButton = 1;
                        return;
                }
        }
}

void pillManager(){
        testCollisionAllPills();
        updateAllPills();
        drawAllPills();
}

void resetPills() {
        int i;
        for (i = 0; i < pillCount; i++) {
                free(pillArr[i]);
        }
        pillCount = 0;
}

void updateMonster() {
        if (keyboard.a == PRESSED && keyboard.d == PRESSED)
                return;
        if (keyboard.a == PRESSED && monster.x > monster.width/2) {
                monster.x = monster.x - monster.xSpeed;
                return;
        }
        if (keyboard.d == PRESSED && monster.x < getHRES() - monster.width/2) {
                monster.x = monster.x + monster.xSpeed;
                return;
        }
}

void resetMonster() {
        monster.x = getHRES()/2 - monster.width/2;
}

void monsterManager() {
        updateMonster();
        drawMonster();
}

void removePill(unsigned int idx){
        int i;
        free(pillArr[idx]);
        for(i = idx; i < pillCount-1; i++) pillArr[i] = pillArr[i + 1];
        pillCount--;
}

collision_evt_t testCollision(Pill* p){
        if (p->y >= getVRES()) return BOUNDARIES_COLLISION;
        if(status.state == GAME)
                if (mouse.lb && !crosshair.didDestroyPill) {
                        if (crosshair.x < p->x + p->width && crosshair.x + crosshair.width > p->x &&
                            crosshair.y < p->y + p->height && crosshair.height + crosshair.y > p->y) {
                                crosshair.didDestroyPill = 1;
                                return MOUSE_COLLISION;
                        }
                        else status.bonus = 0;
                }
        if (status.state == BONUS_LEVEL) {
                if (monster.x < p->x + p->width && monster.x + monster.width > p->x && monster.y < p->y + p->height &&
                    monster.height + monster.y > p->y) return MONSTER_COLLISION;
        }
        if (p->x + p->width >= getHRES() || p->x <= 0) return REFLECTION_COLLISION;
        return NO_COLLISION;
}

void testCollisionAllPills(){
        unsigned int i;
        for (i = 0; i < pillCount; i++) {
                switch(testCollision(pillArr[i])) {
                case BOUNDARIES_COLLISION:
                        if (pillArr[i]->color != TYPE_3 && status.state == GAME) {
                                status.lives--;
                                status.bonus = 0;
                        }
                        removePill(i);
                        break;
                case MOUSE_COLLISION:
                        if (pillArr[i]->color == TYPE_3) {
                                status.state = GAME_OVER;
                                return;
                        }
                        if (pillArr[i]->color == crosshair.color) {
                                removePill(i);
                                status.score++;
                                status.bonus++;
                                return;
                        } else {
                                status.bonus = 0;
                                status.lives -= wrongColorClick;
                        }
                        break;
                case REFLECTION_COLLISION:
			if (pillArr[i]->x <= 0)
				pillArr[i]->xspeed = 
abs(pillArr[i]->xspeed);
         		else
				pillArr[i]->xspeed = 
-abs(pillArr[i]->xspeed);
                        break;
                case MONSTER_COLLISION:
                        removePill(i);
                        status.bonusScore++;
                        break;
                default:
                        break;
                }
        }
}

void updateAllPills(){
        unsigned int i;
        for (i = 0; i < pillCount; i++) {
                updatePill(pillArr[i]);
        }
}

void statusManager(){
        switch(status.state) {
        case SET_UP:
                status.state = MAIN_MENU;
                return;
        case MAIN_MENU:
                mainMenuOptions();
                if (keyboard.q == PRESSED)
                        status.state = LEAVE;
                return;
        case OPTIONS_MENU:
                optionsMenuOptions();
                return;
        default:
                break;

        }
}

void multiplayerStatusManager() {
        static int gameOverCounter = 0;
        switch(status.state) {
        case MULTI_PLAYER_SYNC:
                switch (lobbyManager()) {
                case -1:
                        status.state = MAIN_MENU;
                        status.mode = NO_MODE;
                        serialport_cleanQueues();
                        break;
                case 0:
                        status.state = INIT_GAME;
                        status.seed = getSeed();
                        break;
                case 1:
                        break;
                }
                return;
        case INIT_GAME:
                status.state = GAME;
                return;
        case GAME:
                if (checkSerialPort()) return;
                if (status.lives == 0) {
                        char msg[5];
                        sprintf(msg, "l%u", status.score);
                        serialport_sendStringToBuffer(msg);
                        status.state = GO_WAIT;
                        //printf("sent %s\n", msg);
                        //enviar mensagem ao outro a dizer que ganhou e ele perdeu
                        //send score
                        return;
                }
                if (status.bonus == bonusClicks) {
                        //mandar mensagem para bloquear o rato
                        serialport_sendStringToBuffer(BONUS);
                        //printf("sent %s\n", BONUS);
                        status.bonus = 0;
                        return;
                }
                //testar se recebe coisas
                break;
        case BONUS_LEVEL:
                if (checkSerialPort()) return;
                if (status.bonusTimer == 0) {
                        status.state = GAME;
                        return;
                }

                //bloquear rato
                break;
        case GAME_OVER:
                gameOverCounter = 0;
                gameOverMenuMPOptions();
                return;
        case GO_WAIT:
                if (checkSerialPort()) return;
                gameOverCounter++;
                if (gameOverCounter >= 3*60) {
                        status.state = GAME_OVER;
                        status.multiplayerScore = 0;
                        printf("entering game over after time out\n");
                }
        default:
                break;
        }

}

int checkSerialPort() {
        char * string = (char *) serialport_getWordFromBuffer();
        if (string != NULL) {
                //printf("recevied %s", string);
                if (strcmp(string, BONUS) == 0) {
                        status.state = BONUS_LEVEL;
                        status.bonusTimer = 60*2;
                }
                if (string[0] == 'l') {
                        status.multiplayerScore = strtol(string + 1, NULL, 10);
                        char msg[5];
                        sprintf(msg, "w%u", status.score);
                        //printf("Sent %s\n", msg);
                        serialport_sendStringToBuffer(msg);
                        status.win = 1;
                        status.state = GAME_OVER;
                        serialport_cleanQueues();
                        return 1;
                }
                if (string[0] == 'w') {
                        status.multiplayerScore = strtol(string + 1, NULL, 10);
                        status.state = GAME_OVER;
                        serialport_cleanQueues();
                        status.win = 0;
                        return 1;
                }
                free(string);
        }
        return 0;
}

void singleplayerStatusManager() {
        switch(status.state) {
        case INIT_GAME:
                status.state = GAME;
                return;
        case GAME:
                if (status.lives == 0) {
                        status.state = GAME_OVER;
                        return;
                }
                if (status.bonus == bonusClicks) {
                        status.state = INIT_BONUS;
                        return;
                }
                break;
        case BONUS_LEVEL:
                if (status.bonusTimer == 0) {
                        status.state = RESUME_GAME;
                        return;
                }
                break;
        case RESUME_GAME:
                status.state = GAME;
                return;
        case INIT_BONUS:
                status.state = BONUS_LEVEL;
                return;
        case GAME_PAUSE: case BONUS_PAUSE:
                pauseMenuOptions();
                return;
        case GAME_OVER:
                gameOverMenuOptions();
                return;
        default:
                break;
        }
}

void multiplayerEventManager() {
        switch (status.state) {
        case MULTI_PLAYER_SYNC:
                drawMPBuffer();
                drawCursor();
                break;
        case INIT_GAME:
                srand(status.seed);
                status.difficulty = EASY;
                loadGameVariables();
                drawBackground();
                resetPills();
                status.score = 0;
                status.lives = MAX_LIVES;
                status.bonus = 0;
                status.bonusScore = 0;
                drawStatBar();
                drawScore(status.bonusScore);
                drawLives(status.lives);
                drawBonusCounter(status.bonus);
                break;
        case BONUS_LEVEL:
                drawBackground();
                status.bonusTimer--;
                spawnManager();
                pillManager();
                drawStatBar();
                drawScore(status.score);
                drawLives(status.lives);
                drawBonusCounter(status.bonus);
                //bloquear rato
                crosshair.draw();
                break;
        case GAME:
                drawBackground();
                spawnManager();
                pillManager();
                drawStatBar();
                drawScore(status.score);
                drawLives(status.lives);
                drawBonusCounter(status.bonus);
                crosshair.draw();
                break;
        case GAME_OVER:
                drawBackground();
                drawGameOverMenuMP();
                gameOverMenuMPMouseHover();
                drawCursor();
                //printf("my score: %d, theirs score: %d, win: %d\n", status.score, status.multiplayerScore, status.win);
                break;
        default:
                break;
        }
        multiplayerStatusManager();
}

void singleplayerEventManager() {
        switch (status.state) {
        case INIT_GAME:
                status.seed = generateSeed();
                srand(status.seed);
                loadGameVariables();
                drawBackground();
                resetPills();
                status.score = 0;
                status.lives = MAX_LIVES;
                status.bonus = 0;
                status.bonusScore = 0;
                drawStatBar();
                drawScore(status.bonusScore);
                drawLives(status.lives);
                drawBonusCounter(status.bonus);
                break;
        case BONUS_LEVEL:
                drawBackground();
                status.bonusTimer--;
                if (timerCounter % 20 == 0) createPillBonus();
                monsterManager();
                pillManager();
                drawStatBar();
                drawScore(status.bonusScore);
                drawLives(status.lives);
                drawBonusCounter(status.bonusTimer/60);
                break;
        case INIT_BONUS:
                drawBackground();
                resetPills();
                status.bonusScore = 0;
                status.bonusTimer = 10*60;
                resetMonster();
                drawStatBar();
                drawScore(status.bonusScore);
                drawLives(status.lives);
                drawBonusCounter(status.bonusTimer/60);
                //reset view and handlers
                break;
        case GAME:
                drawBackground();
                spawnManager();
                pillManager();
                drawStatBar();
                drawScore(status.score);
                drawLives(status.lives);
                drawBonusCounter(status.bonus);
                crosshair.draw();
                break;
        case RESUME_GAME:
                drawBackground();
                resetPills();
                if (status.lives < MAX_LIVES)
                        status.lives++;
                status.score += status.bonusScore;
                status.bonus = 0;
                drawStatBar();
                drawScore(status.score);
                drawLives(status.lives);
                drawBonusCounter(status.bonus);
                crosshair.draw();
                break;
        case GAME_PAUSE:
                drawBackground();
                drawAllPills();
                drawStatBar();
                drawScore(status.score);
                drawLives(status.lives);
                drawBonusCounter(status.bonus);
                drawPauseMenu();
                pauseMenuMouseHover();
                drawCursor();
                break;
        case BONUS_PAUSE:
                drawBackground();
                drawMonster();
                drawAllPills();
                drawStatBar();
                drawScore(status.bonusScore);
                drawLives(status.lives);
                drawBonusCounter(status.bonusTimer/60);
                drawPauseMenu();
                pauseMenuMouseHover();
                drawCursor();
                break;
        case GAME_OVER:
                drawBackground();
                drawGameOverMenu();
                gameOverMenuMouseHover();
                drawCursor();
                break;
        default:
                break;
        }
        singleplayerStatusManager();
}

void eventManager() {
        if (status.mode == SINGLE_PLAYER) {
                singleplayerEventManager();
                return;
        }
        if (status.mode == MULTI_PLAYER) {
                multiplayerEventManager();
                return;
        }
        //printf("state %d\n", status.state);
        switch(status.state) {
        case LEAVE:
                // change view and handlers to main_menu. maybe show intermediate view with score
                gameLeave();
                return;
        case SET_UP:
                printf("theme: %d\n", status.theme);
                updateTheme();
                printf("theme: %d\n", status.theme);
                loadBitmaps();
                initCrosshair();
                initCursor();
                initView();
                initMonster();
                initPauseMenu();
                initMainMenu();
                initOptionsMenu();
                initGameOverMenu();
                initGameOverMenuMP();
                break;
        case MAIN_MENU:
                if (status.autoTheme)
                        updateTheme();
                drawBackground();
                drawMainMenu();
                mainMenuMouseHover();
                drawDate(date.day, date.month, date.year, date.hour, date.minutes, date.seconds);
                drawCursor();
                break;
        case OPTIONS_MENU:
                drawBackground();
                drawOptionsMenu();
                optionsMenuMouseHover();
                drawCursor();
                break;
        default:
                break;
        }
        statusManager();
}

void spawnManager(){
        int rate = initialSpawnRate - (status.score/spawnRateEvolution);
        if (rate < maxSpawnRate) rate = maxSpawnRate;
        if (timerCounter % rate == 0) createPill();
        //printf("rate: %d\n", rate);
}

void gameLeave() {
        vg_exit();
        freeBitmaps();
        freeMenuButtons();
        unsubscribe();
        exit(0);
}

unsigned int generateSeed(){
        Date currDate = getCurrentDate();
        unsigned int seed = currDate.year + currDate.month + currDate.day + currDate.hour + currDate.minutes + currDate.seconds;
        return seed;
}
