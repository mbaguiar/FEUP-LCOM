#include "GameModel.h"
#include "video_gr.h"

unsigned int pillCount = 0;

static Bitmap * color1CrosshairBmp;
static Bitmap * color2CrosshairBmp;
static Bitmap * color1PillBmp;
static Bitmap * color2PillBmp;
static Bitmap * monsterBmp;
static Bitmap * backgroundBmp;
static Bitmap * statBarBmp;
static Bitmap * lifeBmp;
static Bitmap * bonusCounterBmp;
static Bitmap * aerolite[10];
static Bitmap * aeroliteSmall[12];
static Bitmap * pillBonus[7];
static Bitmap * multiplayerBuffer[5];
static Bitmap * pauseMenuBmp;
static Bitmap * resumeButtonBmp;
static Bitmap * resumeButtonPressedBmp;
static Bitmap * restartButtonBmp;
static Bitmap * restartButtonPressedBmp;
static Bitmap * exitButtonBmp;
static Bitmap * exitPauseButtonPressedBmp;
static Bitmap * mainMenuBmp;
static Bitmap * singleplayerBmp;
static Bitmap * singleplayerButtonPressedBmp;
static Bitmap * multiplayerBmp;
static Bitmap * multiplayerButtonPressedBmp;
static Bitmap * optionsBmp;
static Bitmap * optionsButtonPressedBmp;
static Bitmap * exit2Bmp;
static Bitmap * exitButtonPressedBmp;
static Bitmap * cursorBmp;
static Bitmap * optionsMenuBmp;
static Bitmap * autoButtonBmp;
static Bitmap * autoButtonPressedBmp;
static Bitmap * dayButtonBmp;
static Bitmap * dayButtonPressedBmp;
static Bitmap * nightButtonBmp;
static Bitmap * nightButtonPressedBmp;
static Bitmap * xmasButtonBmp;
static Bitmap * xmasButtonPressedBmp;
static Bitmap * easyButtonBmp;
static Bitmap * easyButtonPressedBmp;
static Bitmap * mediumButtonBmp;
static Bitmap * mediumButtonPressedBmp;
static Bitmap * hardButtonBmp;
static Bitmap * hardButtonPressedBmp;
static Bitmap * backButtonBmp;
static Bitmap * backButtonPressedBmp;
static Bitmap * deathPillBmp;
static Bitmap * gameOverMenuBmp;
static Bitmap * restartButtonGOBmp;
static Bitmap * restartButtonGOPressedBmp;
static Bitmap * exitButtonGOBmp;
static Bitmap * exitButtonGOPressedBmp;
static Bitmap * gameOverMenuMPWinBmp;
static Bitmap * gameOverMenuMPLoseBmp;
static Bitmap * exitButtonGOMPBmp;
static Bitmap * exitButtonGOMPPressedBmp;

void loadThemedBitmaps(){
        switch(status.theme) {
        case DAY:
                color1CrosshairBmp = (Bitmap *) loadBitmap(themedImgPath("color1crosshair", "day"));
                color2CrosshairBmp = (Bitmap *) loadBitmap(themedImgPath("color2crosshair", "day"));
                color1PillBmp = (Bitmap *) loadBitmap(themedImgPath("color1pill", "day"));
                color2PillBmp = (Bitmap *) loadBitmap(themedImgPath("color2pill", "day"));
                deathPillBmp = (Bitmap *) loadBitmap(themedImgPath("deathPill", "day"));
                backgroundBmp = (Bitmap *) loadBitmap(themedImgPath("background", "day"));
                monsterBmp = (Bitmap *) loadBitmap(imgPath("monster"));
                gameOverMenuBmp = (Bitmap *) loadBitmap(imgPath("gameOverMenu"));

                break;
        case NIGHT:
                color1CrosshairBmp = (Bitmap *) loadBitmap(themedImgPath("color1crosshair", "night"));
                color2CrosshairBmp = (Bitmap *) loadBitmap(themedImgPath("color2crosshair", "night"));
                color1PillBmp = (Bitmap *) loadBitmap(themedImgPath("color1pill", "night"));
                color2PillBmp = (Bitmap *) loadBitmap(themedImgPath("color2pill", "night"));
                deathPillBmp = (Bitmap *) loadBitmap(themedImgPath("deathPill", "night"));
                backgroundBmp = (Bitmap *) loadBitmap(themedImgPath("background", "night"));
                monsterBmp = (Bitmap *) loadBitmap(imgPath("monster"));
                gameOverMenuBmp = (Bitmap *) loadBitmap(imgPath("gameOverMenu"));
                break;
        case CHRISTMAS:
                color1CrosshairBmp = (Bitmap *) loadBitmap(themedImgPath("color1crosshair", "xmas"));
                color2CrosshairBmp = (Bitmap *) loadBitmap(themedImgPath("color2crosshair", "xmas"));
                color1PillBmp = (Bitmap *) loadBitmap(themedImgPath("color1pill", "xmas"));
                color2PillBmp = (Bitmap *) loadBitmap(themedImgPath("color2pill", "xmas"));
                deathPillBmp = (Bitmap *) loadBitmap(themedImgPath("deathPill", "xmas"));
                backgroundBmp = (Bitmap *) loadBitmap(themedImgPath("background", "xmas"));
                monsterBmp = (Bitmap *) loadBitmap(imgPath("santa"));
                gameOverMenuBmp = (Bitmap *) loadBitmap(themedImgPath("gameOverMenu", "xmas"));
                break;
        default:
                break;
        }
}

void freeThemedBitmaps() {
        deleteBitmap(color1CrosshairBmp);
        deleteBitmap(color2CrosshairBmp);
        deleteBitmap(color1PillBmp);
        deleteBitmap(color2PillBmp);
        deleteBitmap(backgroundBmp);
        deleteBitmap(deathPillBmp);
        deleteBitmap(monsterBmp);
        deleteBitmap(gameOverMenuBmp);
        int i;
        for (i = 0; i < 7; i++) {
                deleteBitmap(pillBonus[i]);
        }
}

void freeBitmaps() {
        freeThemedBitmaps();
        deleteBitmap(lifeBmp);
        deleteBitmap(bonusCounterBmp);
        deleteBitmap(cursorBmp);
        deleteBitmap(mainMenuBmp);
        deleteBitmap(singleplayerBmp);
        deleteBitmap(singleplayerButtonPressedBmp);
        deleteBitmap(multiplayerBmp);
        deleteBitmap(multiplayerButtonPressedBmp);
        deleteBitmap(optionsBmp);
        deleteBitmap(optionsButtonPressedBmp);
        deleteBitmap(exit2Bmp);
        deleteBitmap(exitButtonPressedBmp);
        deleteBitmap(optionsMenuBmp);
        deleteBitmap(dayButtonBmp);
        deleteBitmap(nightButtonBmp);
        deleteBitmap(xmasButtonBmp);
        deleteBitmap(autoButtonBmp);
        deleteBitmap(easyButtonBmp);
        deleteBitmap(mediumButtonBmp);
        deleteBitmap(hardButtonBmp);
        deleteBitmap(backButtonBmp);
        deleteBitmap(backButtonPressedBmp);
        deleteBitmap(autoButtonPressedBmp);
        deleteBitmap(dayButtonPressedBmp);
        deleteBitmap(nightButtonPressedBmp);
        deleteBitmap(xmasButtonPressedBmp);
        deleteBitmap(easyButtonPressedBmp);
        deleteBitmap(mediumButtonPressedBmp);
        deleteBitmap(hardButtonPressedBmp);
        deleteBitmap(pauseMenuBmp);
        deleteBitmap(resumeButtonBmp);
        deleteBitmap(resumeButtonPressedBmp);
        deleteBitmap(restartButtonBmp);
        deleteBitmap(restartButtonPressedBmp);
        deleteBitmap(exitButtonBmp);
        deleteBitmap(exitPauseButtonPressedBmp);
        deleteBitmap(restartButtonGOBmp);
        deleteBitmap(restartButtonGOPressedBmp);
        deleteBitmap(exitButtonGOBmp);
        deleteBitmap(exitButtonGOPressedBmp);
        deleteBitmap(exitButtonGOMPBmp);
        deleteBitmap(exitButtonGOMPPressedBmp);
        deleteBitmap(statBarBmp);
        deleteBitmap(gameOverMenuMPWinBmp);
        deleteBitmap(gameOverMenuMPLoseBmp);
        int i;
        for (i = 0; i < 10; i++) {
                deleteBitmap(aerolite[i]);
        }
        for (i = 0; i < 12; i++) {
                deleteBitmap(aeroliteSmall[i]);
        }
        for (i = 0; i < 5; i++) {
                deleteBitmap(multiplayerBuffer[i]);
        }

}

void loadBitmaps() {
        //Change bmps based on game theme
        loadThemedBitmaps();
        lifeBmp = (Bitmap *) loadBitmap(imgPath("life"));
        bonusCounterBmp = (Bitmap *) loadBitmap(imgPath("bonusCounter"));
        cursorBmp = (Bitmap *) loadBitmap(imgPath("cursor"));
        loadFont();
        loadFontSmall();
        loadPillBonus();
        loadMPBuffer();
        mainMenuBmp = (Bitmap *) loadBitmap(imgPath("mainMenu"));
        singleplayerBmp = (Bitmap *) loadBitmap(imgPath("singleplayerButton"));
        singleplayerButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("singleplayerButtonPressed"));
        multiplayerBmp = (Bitmap *) loadBitmap(imgPath("multiplayerButton"));
        multiplayerButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("multiplayerButtonPressed"));
        optionsBmp = (Bitmap *) loadBitmap(imgPath("optionsButton"));
        optionsButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("optionsButtonPressed"));
        exit2Bmp = (Bitmap *) loadBitmap(imgPath("exitButton"));
        exitButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("exitButtonPressed"));
        optionsMenuBmp = (Bitmap *) loadBitmap(imgPath("optionsMenu"));
        dayButtonBmp = (Bitmap *) loadBitmap(imgPath("dayButton"));
        dayButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("dayButtonPressed"));
        nightButtonBmp = (Bitmap *) loadBitmap(imgPath("nightButton"));
        nightButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("nightButtonPressed"));
        xmasButtonBmp = (Bitmap *) loadBitmap(imgPath("xmasButton"));
        xmasButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("xmasButtonPressed"));
        autoButtonBmp = (Bitmap *) loadBitmap(imgPath("automaticButton"));
        autoButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("automaticButtonPressed"));
        easyButtonBmp = (Bitmap *) loadBitmap(imgPath("easyButton"));
        easyButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("easyButtonPressed"));
        mediumButtonBmp = (Bitmap *) loadBitmap(imgPath("mediumButton"));
        mediumButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("mediumButtonPressed"));
        hardButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("hardButtonPressed"));
        hardButtonBmp = (Bitmap *) loadBitmap(imgPath("hardButton"));
        backButtonBmp = (Bitmap *) loadBitmap(imgPath("backButton"));
        backButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("backButtonPressed"));
        pauseMenuBmp = (Bitmap *) loadBitmap(imgPath("pauseMenu"));
        resumeButtonBmp = (Bitmap *) loadBitmap(imgPath("resumeButton"));
        resumeButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("resumeButtonPressed"));
        restartButtonBmp = (Bitmap *) loadBitmap(imgPath("restartButton"));
        restartButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("restartButtonPressed"));
        exitButtonBmp = (Bitmap *) loadBitmap(imgPath("exitPauseButton"));
        exitPauseButtonPressedBmp = (Bitmap *) loadBitmap(imgPath("exitPauseButtonPressed"));
        restartButtonGOBmp = (Bitmap *) loadBitmap(imgPath("restartButtonGO"));
        restartButtonGOPressedBmp = (Bitmap *) loadBitmap(imgPath("restartButtonGOPressed"));
        exitButtonGOBmp = (Bitmap *) loadBitmap(imgPath("exitButtonGO"));
        exitButtonGOPressedBmp = (Bitmap *) loadBitmap(imgPath("exitButtonGOPressed"));
        gameOverMenuMPWinBmp = (Bitmap *) loadBitmap(imgPath("gameOverMenuMPWin"));
        gameOverMenuMPLoseBmp = (Bitmap *) loadBitmap(imgPath("gameOverMenuMPLose"));
        exitButtonGOMPBmp = (Bitmap *) loadBitmap(imgPath("exitButtonGOMP"));
        exitButtonGOMPPressedBmp = (Bitmap *) loadBitmap(imgPath("exitButtonGOMPPressed"));
        statBarBmp = (Bitmap *) loadBitmap(imgPath("statBar"));
}

void freeMenuButtons() {
        free(mainMenu.singleplayer);
        free(mainMenu.multiplayer);
        free(mainMenu.options);
        free(mainMenu.exit);
        free(optionsMenu.autoTheme);
        free(optionsMenu.day);
        free(optionsMenu.night);
        free(optionsMenu.xmas);
        free(optionsMenu.easy);
        free(optionsMenu.hard);
        free(optionsMenu.medium);
        free(optionsMenu.back);
        free(pauseMenu.resume);
        free(pauseMenu.restart);
        free(pauseMenu.exit);
        free(gameOverMenu.restart);
        free(gameOverMenu.exit);
        free(gameOverMenuMP.exit);
}

void loadFont() {
        int i;
        char path[] = "/home/proj/res/fonts/aerolite/regular/X.bmp";
        for (i = 0; i < 10; i++) {
                path[38] = i + '0';
                Bitmap * bmp = (Bitmap *) loadBitmap(path);
                aerolite[i] = bmp;
        }
}

void loadFontSmall() {
        int i;
        char path[] = "/home/proj/res/fonts/aerolite/small/X.bmp";
        for (i = 0; i < 10; i++) {
                path[36] = i + '0';
                Bitmap * bmp = (Bitmap *) loadBitmap(path);
                aeroliteSmall[i] = bmp;
        }
        Bitmap * bmp = (Bitmap *) loadBitmap("/home/proj/res/fonts/aerolite/small/colon.bmp");
        aeroliteSmall[10] = bmp;
        bmp = (Bitmap *) loadBitmap("/home/proj/res/fonts/aerolite/small/slash.bmp");
        aeroliteSmall[11] = bmp;
}

void loadPillBonus() {
        int i;
        char * path;
        if (status.theme == CHRISTMAS) path = themedImgPath("bonusPillX", "xmas");
        else path = imgPath("bonusPillX");
        for (i = 0; i < 7; i++) {
                path[28] = i + '1';
                Bitmap * bmp = (Bitmap *) loadBitmap(path);
                pillBonus[i] = bmp;
        }
}

void loadMPBuffer() {
        int i;
        char path[] = "/home/proj/res/img/multiplayerBufferX.bmp";
        for (i = 1; i <= 5; i++) {
                path[36] = i + '0';
                Bitmap * bmp = (Bitmap *) loadBitmap(path);
                multiplayerBuffer[i-1] = bmp;
        }

}

void loadGameVariables(){
        switch(status.difficulty) {
        case EASY:
                maxXSpeed = EASY_MAX_XSPEED;
                minXSpeed = EASY_MIN_XSPEED;
                maxYSpeed = EASY_MAX_YSPEED;
                minYSpeed = EASY_MIN_YSPEED;
                bonusClicks = EASY_BONUS_CLICKS;
                initialSpawnRate = EASY_INITIAL_SPAWN_RATE;
                deathPillRate = EASY_DEATH_PILL_RATE;
                wrongColorClick = EASY_WRONG_CLICK;
                maxSpawnRate = MAX_SPAWN_RATE;
                spawnRateEvolution = EASY_SPAWN_RATE_EV;
                break;
        case MEDIUM:
                maxXSpeed = MEDIUM_MAX_XSPEED;
                minXSpeed = MEDIUM_MIN_XSPEED;
                maxYSpeed = MEDIUM_MAX_YSPEED;
                minYSpeed = MEDIUM_MIN_YSPEED;
                bonusClicks = MEDIUM_BONUS_CLICKS;
                initialSpawnRate = MEDIUM_INITIAL_SPAWN_RATE;
                deathPillRate = MEDIUM_DEATH_PILL_RATE;
                wrongColorClick = MEDIUM_WRONG_CLICK;
                maxSpawnRate = MAX_SPAWN_RATE;
                spawnRateEvolution = MEDIUM_SPAWN_RATE_EV;
                break;
        case HARD:
                maxXSpeed = HARD_MAX_XSPEED;
                minXSpeed = HARD_MIN_XSPEED;
                maxYSpeed = HARD_MAX_YSPEED;
                minYSpeed = HARD_MIN_YSPEED;
                bonusClicks = HARD_BONUS_CLICKS;
                initialSpawnRate = HARD_INITIAL_SPAWN_RATE;
                deathPillRate = HARD_DEATH_PILL_RATE;
                wrongColorClick = HARD_WRONG_CLICK;
                maxSpawnRate = MAX_SPAWN_RATE;
                spawnRateEvolution = HARD_SPAWN_RATE_EV;
                break;
        }
        srand(status.seed);
}

void drawScore(int score){
        drawAerolite(score, 512, 42);

}

void drawAerolite(int score, int x, int y) {
        int fdigit = score/10;
        int sdigit = score % 10;
        int mdigit = score/100;
        if (score < 10) {
                drawBitmap(aerolite[score], x, y, ALIGN_CENTER, 1);
        }else if (score < 100) {
                int fdigit = score/10;
                int sdigit = score % 10;
                drawBitmap(aerolite[fdigit], x-aerolite[fdigit]->bitmapInfoHeader.width/2, y, ALIGN_CENTER, 1);
                drawBitmap(aerolite[sdigit], x+aerolite[sdigit]->bitmapInfoHeader.width/2, y, ALIGN_CENTER, 1);
        } else {
                fdigit = fdigit % 10;
                drawBitmap(aerolite[fdigit], x, y, ALIGN_CENTER, 1);
                drawBitmap(aerolite[sdigit], x+aerolite[sdigit]->bitmapInfoHeader.width, y, ALIGN_CENTER, 1);
                drawBitmap(aerolite[mdigit], x-aerolite[mdigit]->bitmapInfoHeader.width, y, ALIGN_CENTER, 1);
        }
}

void drawScoreGameOver(int score) {
        drawAerolite(score, gameOverMenu.x + 315, gameOverMenu.y + 202);
}

void drawLives(int lives){
        int x = 632; int y = 36;
        while (lives != 0) {
                drawBitmap(lifeBmp, x, y, ALIGN_CENTER, 1);
                x += 40;
                lives--;
        }
}

void drawBonusCounter(int bonusCounter){
        int x = 33; int y = 36;
        bonusCounter = bonusCounter * 10/bonusClicks;
        while (bonusCounter != 0) {
                drawBitmap(bonusCounterBmp, x, y, ALIGN_CENTER, 1);
                x += 40;
                bonusCounter--;
        }
}

void initCrosshair() {
        crosshair.color = 0;
        crosshair.view = color1CrosshairBmp;
        crosshair.height = crosshair.view->bitmapInfoHeader.height;
        crosshair.width = crosshair.view->bitmapInfoHeader.width;
        crosshair.x = getHRES()/2 - crosshair.width/2;
        crosshair.y = getVRES()/2 - crosshair.height/2;
        crosshair.draw = drawCrosshair;
}

void initView() {
        view.background = backgroundBmp;
        view.statBar = statBarBmp;
}

void initCursor() {
        cursor.view = cursorBmp;
        cursor.x = getHRES()/2 - cursor.view->bitmapInfoHeader.width/2;
        cursor.y = getVRES()/2 - cursor.view->bitmapInfoHeader.height/2;
        cursor.width = cursor.view->bitmapInfoHeader.width;
        cursor.height = cursor.view->bitmapInfoHeader.height;
}

void initButton(Button * button, Bitmap * view1, Bitmap * view2, int x, int y) {
        button->view = view1;
        button->view2 = view2;
        button->width = button->view->bitmapInfoHeader.width;
        button->height = button->view->bitmapInfoHeader.height;
        button->x = x - button->width/2;
        button->y = y - button->height/2;
}

void initPauseMenu() {
        pauseMenu.resume = (Button *) malloc(sizeof(Button));
        pauseMenu.restart = (Button *) malloc(sizeof(Button));
        pauseMenu.exit = (Button *) malloc(sizeof(Button));
        pauseMenu.view = pauseMenuBmp;
        pauseMenu.x = getHRES()/2 - pauseMenu.view->bitmapInfoHeader.width/2;
        pauseMenu.y = getVRES()/2 - pauseMenu.view->bitmapInfoHeader.height/2 + 20;
        initButton(pauseMenu.resume, resumeButtonBmp, resumeButtonPressedBmp, pauseMenu.x + 102, pauseMenu.y + 180);
        initButton(pauseMenu.restart, restartButtonBmp, restartButtonPressedBmp, pauseMenu.x + 102, pauseMenu.y + 282);
        initButton(pauseMenu.exit, exitButtonBmp, exitPauseButtonPressedBmp, pauseMenu.x + 102, pauseMenu.y + 382);
}

void initMainMenu() {
        mainMenu.singleplayer = (Button *) malloc(sizeof(Button));
        mainMenu.multiplayer = (Button *) malloc(sizeof(Button));
        mainMenu.options = (Button *) malloc(sizeof(Button));
        mainMenu.exit = (Button *) malloc(sizeof(Button));
        mainMenu.view = mainMenuBmp;
        mainMenu.x = getHRES()/2 - mainMenu.view->bitmapInfoHeader.width/2;
        mainMenu.y = getVRES()/2 - mainMenu.view->bitmapInfoHeader.height/2;
        initButton(mainMenu.singleplayer, singleplayerBmp, singleplayerButtonPressedBmp, mainMenu.x + 511, mainMenu.y + 335);
        initButton(mainMenu.multiplayer, multiplayerBmp, multiplayerButtonPressedBmp, mainMenu.x + 511, mainMenu.y + 439);
        initButton(mainMenu.options, optionsBmp, optionsButtonPressedBmp, mainMenu.x + 511, mainMenu.y + 541);
        initButton(mainMenu.exit, exit2Bmp, exitButtonPressedBmp, mainMenu.x + 511, mainMenu.y + 642);
}

void initOptionsMenu() {
        optionsMenu.autoTheme = (Button *) malloc(sizeof(Button));
        optionsMenu.day = (Button *) malloc(sizeof(Button));
        optionsMenu.night = (Button *) malloc(sizeof(Button));
        optionsMenu.xmas = (Button *) malloc(sizeof(Button));
        optionsMenu.easy = (Button *) malloc(sizeof(Button));
        optionsMenu.medium = (Button *) malloc(sizeof(Button));
        optionsMenu.hard = (Button *) malloc(sizeof(Button));
        optionsMenu.back = (Button *) malloc(sizeof(Button));
        optionsMenu.view = optionsMenuBmp;
        optionsMenu.x = getHRES()/2 - optionsMenu.view->bitmapInfoHeader.width/2;
        optionsMenu.y = getVRES()/2 - optionsMenu.view->bitmapInfoHeader.height/2;
        initButton(optionsMenu.autoTheme, autoButtonBmp, autoButtonPressedBmp, optionsMenu.x + 367, optionsMenu.x + 346);
        initButton(optionsMenu.day, dayButtonBmp, dayButtonPressedBmp, optionsMenu.x + 393, optionsMenu.x + 426);
        initButton(optionsMenu.night, nightButtonBmp,nightButtonPressedBmp, optionsMenu.x + 393, optionsMenu.x + 506);
        initButton(optionsMenu.xmas, xmasButtonBmp, xmasButtonPressedBmp, optionsMenu.x + 393, optionsMenu.x + 586);
        initButton(optionsMenu.easy, easyButtonBmp, easyButtonPressedBmp, optionsMenu.x + 657, optionsMenu.x + 346);
        initButton(optionsMenu.medium, mediumButtonBmp, mediumButtonPressedBmp, optionsMenu.x + 657, optionsMenu.x + 426);
        initButton(optionsMenu.hard, hardButtonBmp, hardButtonPressedBmp, optionsMenu.x + 657, optionsMenu.x + 506);
        initButton(optionsMenu.back, backButtonBmp, backButtonPressedBmp, optionsMenu.x + 583, optionsMenu.x + 586);
}

void initGameOverMenu() {
        gameOverMenu.restart = (Button *) malloc(sizeof(Button));
        gameOverMenu.exit = (Button *) malloc(sizeof(Button));
        gameOverMenu.view = gameOverMenuBmp;
        gameOverMenu.x = getHRES()/2 - gameOverMenu.view->bitmapInfoHeader.width/2;
        gameOverMenu.y = getVRES()/2 - gameOverMenu.view->bitmapInfoHeader.height/2;
        initButton(gameOverMenu.restart, restartButtonGOBmp, restartButtonGOPressedBmp, gameOverMenu.x + 131, gameOverMenu.y + 296);
        initButton(gameOverMenu.exit, exitButtonGOBmp, exitButtonGOPressedBmp, gameOverMenu.x + 131, gameOverMenu.y + 396);
}

void initGameOverMenuMP() {
        gameOverMenuMP.exit = (Button *) malloc(sizeof(Button));
        gameOverMenuMP.viewLose = gameOverMenuMPLoseBmp;
        gameOverMenuMP.viewWin = gameOverMenuMPWinBmp;
        gameOverMenuMP.y = getVRES()/2 - gameOverMenuMP.viewWin->bitmapInfoHeader.height/2;
        gameOverMenuMP.x = getHRES()/2 - gameOverMenuMP.viewWin->bitmapInfoHeader.width/2;
        initButton(gameOverMenuMP.exit, exitButtonGOMPBmp, exitButtonGOMPPressedBmp, gameOverMenuMP.x + 94, gameOverMenuMP.y + 458);
}

void initStatus() {
        status.score = 0;
        status.lives = MAX_LIVES;
        status.bonus = 0;
        status.bonusScore = 0;
        status.bonusTimer = 10;
        status.state = SET_UP;
        status.difficulty = EASY;
        status.theme = NO_THEME;
        status.autoTheme = 1;
        status.mode = NO_MODE;
}

Pill * initPill(){
        Pill * pill = (Pill *) malloc(sizeof(Pill));
        int type = rand() % 100;
        if (type < deathPillRate) {
                pill->color = TYPE_3;
                pill->view = deathPillBmp;
        } else {
                pill->color = type % 2;
                if (pill->color == TYPE_1) pill->view = color1PillBmp;
                else pill->view = color2PillBmp;
        }
        pill->height = pill->view->bitmapInfoHeader.height;
        pill->width = pill->view->bitmapInfoHeader.width;
        pill->x = rand() % (getHRES() - (pill->width * 2)) + pill->width;
        pill->y = -pill->height;
        if (rand() % 2) pill->xspeed = rand() % 2 + minXSpeed;
        else pill->xspeed = -(rand() % 2 + minXSpeed);
        pill->yspeed = rand() % 3 + minYSpeed;
        return pill;
}

Pill * initPillBonus() {
        Pill * pill = (Pill *) malloc(sizeof(Pill));
        pill->view = pillBonus[rand() % 7];
        pill->height = pill->view->bitmapInfoHeader.height;
        pill->width = pill->view->bitmapInfoHeader.width;
        pill->x = rand() % (getHRES() - (pill->width * 2)) + pill->width;
        pill->y = -pill->view->bitmapInfoHeader.height;
        pill->xspeed = 0;
        pill->yspeed = rand() % 3 + 8;
        return pill;
}

void initMonster() {
        monster.view = monsterBmp;
        monster.height = monster.view->bitmapInfoHeader.height;
        monster.width = monster.view->bitmapInfoHeader.width;
        monster.x = getHRES()/2 - monster.height/2;
        monster.y = getVRES() - monster.height;
        monster.xSpeed = 7;
}

void changeColor() {
        if (crosshair.color == TYPE_1) {
                crosshair.view = color2CrosshairBmp;
                crosshair.color = TYPE_2;
        } else {
                crosshair.view = color1CrosshairBmp;
                crosshair.color = TYPE_1;
        }
}

void changeTheme() {
        freeThemedBitmaps();
        loadThemedBitmaps();
        initMonster();
        loadPillBonus();
}

void drawCrosshair() {
        drawBitmap(crosshair.view, crosshair.x, crosshair.y, ALIGN_LEFT, 1);
}

void drawBackground() {
        drawBitmap(view.background, 0, 0, ALIGN_LEFT, 0);
}

void drawStatBar() {
        drawBitmap(view.statBar, 0, 0, ALIGN_LEFT, 1);
}

void drawPill(Pill * p){
        drawBitmap(p->view, p->x, p->y, ALIGN_LEFT, 1);
}

void drawMonster() {
        drawBitmap(monster.view, monster.x, monster.y, ALIGN_LEFT, 1);
}

void drawButton(Button * button) {
        drawBitmap(button->view, button->x, button->y, ALIGN_LEFT, 0);
}

void drawPressedButton(Button * button) {
        drawBitmap(button->view2, button->x, button->y, ALIGN_LEFT, 0);
}

void drawMainMenu() {
        drawBitmap(mainMenu.view, mainMenu.x, mainMenu.y, ALIGN_LEFT, 0);
        drawButton(mainMenu.singleplayer);
        drawButton(mainMenu.multiplayer);
        drawButton(mainMenu.options);
        drawButton(mainMenu.exit);
}

void drawPauseMenu() {
        drawBitmap(pauseMenu.view, pauseMenu.x, pauseMenu.y, ALIGN_LEFT, 1);
        drawButton(pauseMenu.resume);
        drawButton(pauseMenu.restart);
        drawButton(pauseMenu.exit);
}

void drawDate(int day, int month, int year, int hours, int minutes, int seconds) {
        drawBitmap(aeroliteSmall[day/10], 760, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[day%10], 776, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[11], 792, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[month/10], 808, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[month%10], 824, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[11], 840, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[year/10], 856, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[(year%10)%10], 872, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[hours/10], 904, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[hours%10], 920, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[10], 936, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[minutes/10], 944, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[minutes%10], 960, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[10], 978, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[seconds/10], 984, 736, ALIGN_LEFT, 1);
        drawBitmap(aeroliteSmall[seconds%10], 1000, 736, ALIGN_LEFT, 1);
}

void drawOptionsMenu() {
        drawBitmap(optionsMenu.view, optionsMenu.x, optionsMenu.y, ALIGN_LEFT, 0);
        if (status.autoTheme) drawPressedButton(optionsMenu.autoTheme);
        else drawButton(optionsMenu.autoTheme);
        if (!status.autoTheme && status.theme == DAY) drawPressedButton(optionsMenu.day);
        else drawButton(optionsMenu.day);
        if (!status.autoTheme && status.theme == NIGHT) drawPressedButton(optionsMenu.night);
        else drawButton(optionsMenu.night);
        if (!status.autoTheme && status.theme == CHRISTMAS) drawPressedButton(optionsMenu.xmas);
        else drawButton(optionsMenu.xmas);
        if (status.difficulty == EASY) drawPressedButton(optionsMenu.easy);
        else drawButton(optionsMenu.easy);
        if (status.difficulty == MEDIUM) drawPressedButton(optionsMenu.medium);
        else drawButton(optionsMenu.medium);
        if (status.difficulty == HARD) drawPressedButton(optionsMenu.hard);
        else drawButton(optionsMenu.hard);
        drawButton(optionsMenu.back);
}

void drawGameOverMenu() {
        drawBitmap(gameOverMenu.view, gameOverMenu.x, gameOverMenu.y, ALIGN_LEFT, 1);
        drawButton(gameOverMenu.restart);
        drawButton(gameOverMenu.exit);
        drawScoreGameOver(status.score);
}

void drawGameOverMenuMP() {
        if (status.win == 1) drawBitmap(gameOverMenuMP.viewWin, gameOverMenuMP.x, gameOverMenuMP.y, ALIGN_LEFT, 1);
        else drawBitmap(gameOverMenuMP.viewLose, gameOverMenuMP.x, gameOverMenuMP.y, ALIGN_LEFT, 1);
        drawButton(gameOverMenuMP.exit);
        drawAerolite(status.score, gameOverMenuMP.x + 125, gameOverMenuMP.y + 276);
        drawAerolite(status.multiplayerScore, gameOverMenuMP.x + 395, gameOverMenuMP.y + 276);
}

void drawCursor() {
        drawBitmap(cursor.view, cursor.x, cursor.y, ALIGN_LEFT, 1);
}

void drawAllPills(){
        unsigned int i;
        for (i = 0; i < pillCount; i++) {
                drawPill(pillArr[i]);
        }
}

void drawMPBuffer() {
        static int i = 0;
        i++;
        if (i/120 >= 5) i = 0;
        drawBitmap(multiplayerBuffer[i/120], 0, 0, ALIGN_LEFT, 0);
}
