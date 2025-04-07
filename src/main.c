#include "..\lib\SMSlib.h"
#include "..\lib\PSGlib.h"
#include "..\lib\GSLib.h"

#include "..\banks\bank2.h"
#include "..\banks\bank3.h"
#include "..\banks\bank4.h"

#include "definitions.h"
#include "resources.c"
#include "objects.c"

// Player Logic
void ClearPlayerVariables(char i);
void UpdatePlayerAnimations(char i);
void UpdatePlayer(char i);
void MovePlayerNoCollision(char i);
void MovePlayer(char i);
void MoveUp(char i);
void MoveDown(char i);
void MoveLeft(char i);
void MoveRight(char i);
void UpdateAction(char i);
void UpdateNumFactoriesSpriteIds(void);

// Player blocking
char IsPlayerUpBlocked(char i);
char IsPlayerDownBlocked(char i);
char IsPlayerLeftBlocked(char i);
char IsPlayerRightBlocked(char i);
char IsPlayerUpLeftBlocked(char i);
char IsPlayerUpRightBlocked(char i);
char IsPlayerDownLeftBlocked(char i);
char IsPlayerDownRightBlocked(char i);

// Metatiles
void MetatileSwapWalkable(unsigned char *metatile);
void MetatileSwapShootable(unsigned char *metatile);
void MetatileFactoryHit(unsigned char * metatile);
char GetTopLeftMetatile(char i);
char GetTopRightMetatile(char i);
char GetBottomLeftMetatile(char i);
char GetBottomRightMetatile(char i);
char GetLowerLeftMetatile(char i);
char GetLowerRightMetatile(char i);
char GetLeftUpMetatile(char i);
char GetLeftDownMetatile(char i);
char GetRightUpMetatile(char i);
char GetRightDownMetatile(char i);

// Bullet Behavior
void ShootBullet(char i);
char ShootTurretBullet(char turretIndex, char direction);
void UpdateBullets(char i);
void MoveBulletUp(char i, char j);
void MoveBulletDown(char i, char j);
void MoveBulletLeft(char i, char j);
void MoveBulletRight(char i, char j);
void UpdateEnemyBullets(void);
void MoveEnemyBulletUp(char i);
void MoveEnemyBulletDown(char i); 
void MoveEnemyBulletLeft(char i);
void MoveEnemyBulletRight(char i);
void UpdateEnemyBulletPosition(char i);

// Roar
void Roar(char i);

// Enemy AI
void InitTurrets(void);
void ScanMapForTurrets(void);
char GetClosestPlayer(unsigned int posX, unsigned int posY);
char GetEnemyFireDirection(unsigned int sourceX, unsigned int sourceY, 
                          unsigned int targetX, unsigned int targetY);
void CheckTurretsInBoundingBox(void);
void UpdateTurrets(void);

// Helper Functions
int abs_delta(int delta);
char RandomDirection(void);

unsigned char nextRandomByte(void);

// Title and End Screen
void LoadTitleScreen(void);
void LoadEndScreen(void);
void LoadGameScreen(void);
void HandleTitleScreen(void);
void HandleEndScreen(void);
void HandleGameScreen(void);

// Sprite Rendering
void RenderSprites(void);
void RenderSpritesUnsafe(void);

// Music and SFX handling 
void LoadAndPlayMusic(void);
void LoadAndPlaySFX(char sfx);
void StopAllAudio(void);
unsigned char musicResumePosition = 0;
unsigned char sfxCountdown = 0;

// Only Player One scrolls
unsigned int scrollXTotal = 0;
unsigned int scrollYTotal = 0;
unsigned char scrollXThisFrame = 0;
unsigned char scrollYThisFrame = 0;

// Bullet debug timers
unsigned char bulletShootTimer = 0;
unsigned char bulletShootRate = 10;
unsigned char bulletShootDir = 1;

unsigned int keyStatus = 0;
unsigned char playerTwoJoined = 0;
unsigned char endCounter = 0;

// Copy of the map in ram
unsigned char scrolltable[ugtbatch_scrolltable_bin_size];

// Turret Scanning and Bullet Behavior
unsigned char turretCheckIndex = 0;
unsigned char lastRandomDirection = 0;

// Gamestate and counters
unsigned char gameState = GAME_STATE_GAME;
unsigned char numFactories = MAX_FACTORY_NUM; // when this reaches 0, game is won
unsigned char numFactoriesSpriteIdOne = 0;
unsigned char numFactoriesSpriteIdTwo = 0;
unsigned char menuButtonPressed = 0;
unsigned char menuStartFlasher = 0;
unsigned char menuStartVisible = 0;
unsigned char playerStreamTurn = 0;
unsigned char enemyBulletForCollisionUpdate = 0;

struct PlayerObject players[PLAYER_COUNT];
struct SpriteObject playersSprites[PLAYER_COUNT];
struct SpriteObject enemyBullets[ENEMY_BULLET_COUNT];

// Header
SMS_EMBED_SEGA_ROM_HEADER(999, 0);
SMS_EMBED_SDSC_HEADER(1, 0, 2024, 9, 24, "Samuli", "LDJAM57", "Love");

void main(void)
{
    // Load the title screen first
    LoadTitleScreen();

    while(1)
    {
        // Read gamepad
        keyStatus = SMS_getKeysStatus();
        
        // Handle current game state
        switch (gameState)
        {
            case GAME_STATE_TITLE:
                SMS_waitForVBlank();
                HandleTitleScreen();
                break;
                
            case GAME_STATE_GAME:
                HandleGameScreen();
                if(gameState == GAME_STATE_END) break;
                // Game-specific rendering
                GSL_scroll(scrollXThisFrame, scrollYThisFrame);
                SMS_waitForVBlank();
                UNSAFE_SMS_copySpritestoSAT();
                SMS_initSprites();
                RenderSpritesUnsafe();
                RenderSprites();
                GSL_VBlank();
                break;
                
            case GAME_STATE_END:
                SMS_waitForVBlank();
                HandleEndScreen();
                break;
        }
        
        // Play audio
        PSGFrame();
        PSGSFXFrame();
    }
}

void LoadAndPlayMusic(void) 
{
    SMS_mapROMBank(kaijulove_psg_bank);
    PSGPlay(&kaijulove_psg);
}

void LoadAndPlaySFX(char sfx)
{   
    switch(sfx)
    {
        case SFX_EXPLOSION:
            SMS_mapROMBank(explosion0_psg_bank);
            PSGSFXPlay(&explosion0_psg, SFX_CHANNEL3);
        break;

        case SFX_CONVERT:
            SMS_mapROMBank(convert_psg_bank);
            PSGSFXPlay(&convert_psg, SFX_CHANNEL3);
        break;

        case SFX_CONVERT_FACTORY:
            SMS_mapROMBank(convertfactory_psg_bank);
            PSGSFXPlay(&convertfactory_psg, SFX_CHANNEL3);
        break;

        case SFX_ENEMY_SHOOT:
            SMS_mapROMBank(enemyshoot0_psg_bank);
            PSGSFXPlay(&enemyshoot0_psg, SFX_CHANNEL0);
        break;

        case SFX_MAP:
            SMS_mapROMBank(map0_psg_bank);
            PSGSFXPlay(&map0_psg, SFX_CHANNEL0);
        break;

        case SFX_ROAR:
            SMS_mapROMBank(roar0_psg_bank);
            PSGSFXPlay(&roar0_psg, SFX_CHANNEL0);
        break;

        case SFX_PLAYER_SHOOT:
            SMS_mapROMBank(shoot0_psg_bank);
            PSGSFXPlay(&shoot0_psg, SFX_CHANNEL0);
        break;
    }
}

void StopAllAudio(void)
{
    PSGStop();
    PSGSFXStop();
}

void LoadTitleScreen(void)
{
    gameState = GAME_STATE_TITLE;
    StopAllAudio();

    // Setup VPD
    SMS_VRAMmemsetW(0, 0, 0);
    SMS_loadSpritePalette(black_palette_bin);
    SMS_VDPturnOffFeature(VDPFEATURE_HIDEFIRSTCOL);
    SMS_waitForVBlank();
    SMS_displayOff();
    SMS_setBGScrollX(0);
    SMS_setBGScrollY(0);
    
    // Load the title screen tiles and data
    SMS_mapROMBank(startendpalette_pal_bin_bank);
    SMS_loadBGPalette(&startendpalette_pal_bin);
    //SMS_loadTiles(&start_tiles_bin, 0, start_tiles_bin_size);
    //SMS_loadTileMap(0,0,&start_map_bin, start_map_bin_size);
    SMS_mapROMBank(font_tiles_bin_bank);
    SMS_loadTiles(&font_tiles_bin, FONT_VRAM_OFFSET, font_tiles_bin_size);
    SMS_configureTextRenderer(FONT_VRAM_OFFSET - 32);
    SMS_printatXY(7, 3, "--==GOODZILLA==--");
    SMS_printatXY(10, 8, "Press START");
    SMS_printatXY(7, 16, "Player Two can join");
    SMS_printatXY(7, 17, "by pressing START");
    SMS_printatXY(7, 18, "during gameplay!");
    // Loading complete, start display
    SMS_displayOn();
}

void LoadEndScreen(void)
{
    gameState = GAME_STATE_END;
    StopAllAudio();

    // Setup VPD
    SMS_VRAMmemsetW(0, 0, 0);
    SMS_loadSpritePalette(black_palette_bin);
    SMS_VDPturnOffFeature(VDPFEATURE_HIDEFIRSTCOL);
    SMS_waitForVBlank();
    SMS_displayOff();
    SMS_setBGScrollX(0);
    SMS_setBGScrollY(0);
    
    // Load the end screen tiles and data
    SMS_mapROMBank(startendpalette_pal_bin_bank);
    SMS_loadBGPalette(&startendpalette_pal_bin);
    //SMS_loadTiles(&end_tiles_bin, 0, end_tiles_bin_size);
    //SMS_loadTileMap(0,0,&end_map_bin, end_map_bin_size);
    SMS_mapROMBank(font_tiles_bin_bank);
    SMS_loadTiles(&font_tiles_bin, FONT_VRAM_OFFSET, font_tiles_bin_size);
    SMS_configureTextRenderer(FONT_VRAM_OFFSET - 32);
    SMS_printatXY(7, 3, "World liberated!");
    SMS_printatXY(10, 8, "Press START");
    SMS_printatXY(3, 16, "CREDITS:");
    SMS_printatXY(4, 17, "        [[    [");
    SMS_printatXY(4, 18, "Samuli Jaaskelainen");
    SMS_printatXY(4, 19, "Keith Davis");
    SMS_printatXY(4, 20, "Andrea Kranck");
    SMS_printatXY(4, 21, "Borb");
    
    // Loading complete, start display
    SMS_displayOn();
}

void LoadGameScreen(void)
{
    gameState = GAME_STATE_GAME;

    // Setup VDP
    SMS_displayOff();
    SMS_useFirstHalfTilesforSprites(0);
    SMS_VDPturnOnFeature(VDPFEATURE_HIDEFIRSTCOL);
    SMS_setSpriteMode(SPRITEMODE_TALL);
    SMS_VRAMmemsetW(0, 0, 0);
    SMS_mapROMBank(ugtbatch_palette_bin_bank);
    SMS_loadBGPalette(&ugtbatch_palette_bin);
    SMS_mapROMBank(spritepalette_pal_bin_bank);
    SMS_loadSpritePalette(&spritepalette_pal_bin);
    SMS_mapROMBank(ugtbatch_tiles_bin_bank);
    SMS_loadTiles(&ugtbatch_tiles_bin, 0, ugtbatch_tiles_bin_size);
    SMS_mapROMBank(pollen_tiles_bin_bank);
    SMS_loadTiles(&pollen_tiles_bin, 264, pollen_tiles_bin_size);
    SMS_mapROMBank(bullet_tiles_bin_bank);
    SMS_loadTiles(&bullet_tiles_bin, 268, bullet_tiles_bin_size);
    SMS_mapROMBank(factory_tiles_bin_bank);
    SMS_loadTiles(&factory_tiles_bin, 270, factory_tiles_bin_size);
    SMS_mapROMBank(spritenumbers_tiles_bin_bank);
    SMS_loadTiles(&spritenumbers_tiles_bin, 274, spritenumbers_tiles_bin_size);

    // Setup scrolltable
    SMS_mapROMBank(ugtbatch_scrolltable_bin_bank);
    for (int i = 0; i < ugtbatch_scrolltable_bin_size; i++) scrolltable[i] = *(ugtbatch_scrolltable_bin + i);

    // Setup GSL
    SMS_mapROMBank(ugtbatch_metatiles_bin_bank);
    GSL_initializeMap(&scrolltable, &ugtbatch_metatiles_bin);
    GSL_positionWindow(START_WOLRD_OFFSET_X, START_WOLRD_OFFSET_Y);
    scrollXTotal = START_WOLRD_OFFSET_X;
    scrollYTotal = START_WOLRD_OFFSET_Y;
    GSL_refreshVDP();

    // Init players
    for(char i = 0; i < PLAYER_COUNT; ++i)
    {
        playersSprites[i].positionX = PLAYER_START_X;
        playersSprites[i].positionY = PLAYER_START_Y;
        playersSprites[i].spriteX = PLAYER_SPRITE_START_X;
        playersSprites[i].spriteY = PLAYER_SPRITE_START_Y;
        playersSprites[i].isVisible = i == 0;
        playersSprites[i].size = 16;
        playersSprites[i].speed = PLAYER_SPEED_DEFAULT;
        playersSprites[i].animationFrameCounter = 0;
        playersSprites[i].currentAnimationFrame = 0;
        playersSprites[i].animationFrameDataCount = 4;
        setSpriteAnimation(&playersSprites[i], playerAnimIdleUp);
        playersSprites[i].direction = DIRECTION_DOWN;
        players[i].ramDataAddress = i == 0 ? 8192 : 8320;
        playersSprites[i].spriteOneIndex = i << 2;
        playersSprites[i].spriteTwoIndex = 2 + (i << 2);
        players[i].action = ACTION_STATIONARY;
        players[i].actionCount = 0;
        players[i].actionOnePressed = 0;
        players[i].inputVertical = DIRECTION_NONE;
        players[i].inputHorizontal = DIRECTION_NONE;

        for(char j = 0; j < PLAYER_BULLET_COUNT; ++j)
        {
            players[i].bullets[j].positionX = 0;
            players[i].bullets[j].positionY = 0;
            players[i].bullets[j].spriteX = 0;
            players[i].bullets[j].spriteY = 0;
            players[i].bullets[j].isVisible = 0;
            players[i].bullets[j].size = 16;
            players[i].bullets[j].speed = 4;
            players[i].bullets[j].direction = DIRECTION_DOWN;
            players[i].bullets[j].spriteOneIndex = 8;
            players[i].bullets[j].spriteTwoIndex = 10;
        }
    }

    // Init enemy bullets
    for(char i = 0; i < ENEMY_BULLET_COUNT; ++i)
    {
        enemyBullets[i].positionX = 0;
        enemyBullets[i].positionY = 0;
        enemyBullets[i].spriteX = 0;
        enemyBullets[i].spriteY = 0;
        enemyBullets[i].isVisible = 0;
        enemyBullets[i].size = 14;
        enemyBullets[i].speed = ENEMY_BULLET_SPEED_DEFAULT;
        enemyBullets[i].direction = DIRECTION_DOWN;
        enemyBullets[i].spriteOneIndex = 12;
        enemyBullets[i].spriteTwoIndex = 0;
    }

    // Reset game progress variables
    numFactories = MAX_FACTORY_NUM;
    playerTwoJoined = 0;
    endCounter = 0;
    UpdateNumFactoriesSpriteIds();
  
    // Initialize turrets
    ScanMapForTurrets();
    InitTurrets();

    // Reload sprite palette after loading flicker animation
    SMS_mapROMBank(spritepalette_pal_bin_bank);
    SMS_loadSpritePalette(&spritepalette_pal_bin);

    // Init audio & play music
    LoadAndPlayMusic();

    // Set bank for streaming tiles (should use same bank as audio)
    SMS_mapROMBank(player_tiles_bin_bank);

    // Loading complete, start display
    SMS_displayOn();
}

void HandleTitleScreen(void)
{
    menuStartFlasher++;
    if(menuStartFlasher > 20)
    {
        menuStartFlasher = 0;
        if(menuStartVisible)
        {
            SMS_printatXY(10, 8, "           ");
            menuStartVisible = 0;
        }
        else
        {
            SMS_printatXY(10, 8, "Press START");
            menuStartVisible = 1;
        }
    }

    if(menuButtonPressed)
    {
        if (!(keyStatus & PORT_A_KEY_1))
        {
            menuButtonPressed = 0;
        }
    }
    else
    {
        if (keyStatus & PORT_A_KEY_1)
        {
            menuButtonPressed = 1;
            LoadGameScreen();
        }
    }
}

void HandleEndScreen(void)
{
    menuStartFlasher++;
    if(menuStartFlasher > 20)
    {
        menuStartFlasher = 0;
        if(menuStartVisible)
        {
            SMS_printatXY(10, 8, "           ");
            menuStartVisible = 0;
        }
        else
        {
            SMS_printatXY(10, 8, "Press START");
            menuStartVisible = 1;
        }
    }

    if(menuButtonPressed)
    {
        if (!(keyStatus & PORT_A_KEY_1))
        {
            menuButtonPressed = 0;
        }
    }
    else
    {
        if (keyStatus & PORT_A_KEY_1)
        {
            menuButtonPressed = 1;
            LoadTitleScreen();
        }
    }
}

void HandleGameScreen(void)
{
    // Update all player related functions
    for(char i = 0; i < PLAYER_COUNT; ++i)
    {
        // Shoot take n steps, 0 means new actions can be started
        ClearPlayerVariables(i);
        if (players[i].actionCount == 0) UpdatePlayer(i);
        if (players[i].actionCount != 0) { players[i].actionCount--; UpdateAction(i); }
        UpdatePlayerAnimations(i);
        UpdateBullets(i);
    }

    // Update general functions
    UpdateEnemyBullets();
    CheckTurretsInBoundingBox(); // check one turret per frame
    UpdateTurrets(); // Update and handle shooting for all active turrets
    if(endCounter > 0)
    {
        endCounter--;
        if(endCounter == 0)
        {
            LoadEndScreen();
        }
    }
}

void RenderSprites(void)
{
    for(char i = 0; i < PLAYER_COUNT; ++i)
    {
        if(!playersSprites[i].isVisible) continue;

        // Add player sprites, tall mode is in use 248
        SMS_addSprite(playersSprites[i].spriteX - 8, playersSprites[i].spriteY - 8, playersSprites[i].spriteOneIndex);
        SMS_addSprite(playersSprites[i].spriteX, playersSprites[i].spriteY - 8, playersSprites[i].spriteTwoIndex);

        // Render player bullets
        for(char j = 0; j < PLAYER_BULLET_COUNT; ++j)
        {
            if(players[i].bullets[j].isVisible)
            {
                SMS_addSprite(players[i].bullets[j].spriteX - 8, players[i].bullets[j].spriteY - 8, players[i].bullets[j].spriteOneIndex);
                SMS_addSprite(players[i].bullets[j].spriteX, players[i].bullets[j].spriteY - 8, players[i].bullets[j].spriteTwoIndex);
            }
        }
    }

    // Render enemy bullets
    for(char j = 0; j < ENEMY_BULLET_COUNT; ++j)
    {
        if(enemyBullets[j].isVisible)
        {
            SMS_addSprite(enemyBullets[j].spriteX - 4, enemyBullets[j].spriteY - 4, enemyBullets[j].spriteOneIndex);
        }
    }

    // Render UI, shared between players
    SMS_addSprite(UI_X, UI_Y, 270);
    SMS_addSprite(UI_X + 8, UI_Y, 272);

    if(numFactories > 9)
    {
        SMS_addSprite(UI_X + 17, UI_Y, numFactoriesSpriteIdOne);
        SMS_addSprite(UI_X + 25,UI_Y, numFactoriesSpriteIdTwo);
    }
    else
    {
        SMS_addSprite(UI_X + 17, UI_Y, numFactoriesSpriteIdOne);
    }
}

void UpdateNumFactoriesSpriteIds(void)
{
    if(numFactories > 9)
    {
        // Split tens and ones
        unsigned char digits[2] = {0,0};
        unsigned char splitter = numFactories;
        while (splitter >= 10)
        {
            splitter -= 10;
            digits[0]++;
        }
        digits[1] = splitter;
        numFactoriesSpriteIdOne = 274 + (digits[0] << 1);
        numFactoriesSpriteIdTwo = 274 + (digits[1] << 1);
    }
    else
    {
        numFactoriesSpriteIdOne = 274 + (numFactories << 1);
    }
}

void RenderSpritesUnsafe(void)
{
    if(playerTwoJoined)
    {
        if(playerStreamTurn == 4)
        {
            UNSAFE_SMS_VRAMmemcpy128(players[PLAYER_ONE].ramDataAddress, &player_tiles_bin[playersSprites[PLAYER_ONE].animationFrameData[playersSprites[PLAYER_ONE].currentAnimationFrame]]);
        }
        else if(playerStreamTurn == 8)
        {
            playerStreamTurn = 0;
            UNSAFE_SMS_VRAMmemcpy128(players[PLAYER_TWO].ramDataAddress, &player_tiles_bin[playersSprites[PLAYER_TWO].animationFrameData[playersSprites[PLAYER_TWO].currentAnimationFrame]]);
        }
        playerStreamTurn++;
    }
    else
    {
        if(playerStreamTurn == 8)
        {
            playerStreamTurn = 0;
            UNSAFE_SMS_VRAMmemcpy128(players[PLAYER_ONE].ramDataAddress, &player_tiles_bin[playersSprites[PLAYER_ONE].animationFrameData[playersSprites[PLAYER_ONE].currentAnimationFrame]]);
        }
        playerStreamTurn++;
    }
}

void UpdatePlayerAnimations(char i)
{
    // Update animation frames
    if(playersSprites[i].animationFrameCounter > PLAYER_ANIMATION_HOLD_DURATION)
    {
        playersSprites[i].animationFrameCounter = 0;
        playersSprites[i].currentAnimationFrame++;
        if(playersSprites[i].currentAnimationFrame == playersSprites[i].animationFrameDataCount) playersSprites[i].currentAnimationFrame = 0;
    }
    playersSprites[i].animationFrameCounter++;

    // Update which animation to play
    switch (playersSprites[i].direction)
    {
        case DIRECTION_UP:
            if(players[i].action == ACTION_MOVE) setSpriteAnimation(&playersSprites[i], playerAnimMoveUp);
            else if(players[i].action == ACTION_ONE) setSpriteAnimation(&playersSprites[i], playerAnimOne);
            else if(players[i].action == ACTION_TWO) setSpriteAnimation(&playersSprites[i], playerAnimTwo);
            else if(players[i].action == ACTION_STUN) setSpriteAnimation(&playersSprites[i], playerAnimA);
            else setSpriteAnimation(&playersSprites[i], playerAnimIdleUp);
        break;

        case DIRECTION_DOWN:
            if(players[i].action == ACTION_MOVE) setSpriteAnimation(&playersSprites[i], playerAnimMoveDown);
            else if(players[i].action == ACTION_ONE) setSpriteAnimation(&playersSprites[i], playerAnimOne);
            else if(players[i].action == ACTION_TWO) setSpriteAnimation(&playersSprites[i], playerAnimTwo);
            else if(players[i].action == ACTION_STUN) setSpriteAnimation(&playersSprites[i], playerAnimA);
            else setSpriteAnimation(&playersSprites[i], playerAnimIdleDown);
        break;

        case DIRECTION_LEFT:
        case DIRECTION_UP_LEFT:
        case DIRECTION_DOWN_LEFT:
            if(players[i].action == ACTION_MOVE) setSpriteAnimation(&playersSprites[i], playerAnimMoveLeft);
            else if(players[i].action == ACTION_ONE) setSpriteAnimation(&playersSprites[i], playerAnimOne);
            else if(players[i].action == ACTION_TWO) setSpriteAnimation(&playersSprites[i], playerAnimTwo);
            else if(players[i].action == ACTION_STUN) setSpriteAnimation(&playersSprites[i], playerAnimA);
            else setSpriteAnimation(&playersSprites[i], playerAnimIdleLeft);
        break;

        case DIRECTION_RIGHT:
        case DIRECTION_UP_RIGHT:
        case DIRECTION_DOWN_RIGHT:
            if(players[i].action == ACTION_MOVE) setSpriteAnimation(&playersSprites[i], playerAnimMoveRight);
            else if(players[i].action == ACTION_ONE) setSpriteAnimation(&playersSprites[i], playerAnimOne);
            else if(players[i].action == ACTION_TWO) setSpriteAnimation(&playersSprites[i], playerAnimTwo);
            else if(players[i].action == ACTION_STUN) setSpriteAnimation(&playersSprites[i], playerAnimA);
            else setSpriteAnimation(&playersSprites[i], playerAnimIdleRight);
        break;
    }

    // Update total scroll, Player One only
    if(i == PLAYER_ONE)
    {
        scrollXTotal += scrollXThisFrame;
        scrollYTotal += scrollYThisFrame;
    }
}

void ClearPlayerVariables(char i)
{
    // Reset scroll, Player One only
    if(i == PLAYER_ONE)
    {
        scrollXThisFrame = 0;
        scrollYThisFrame = 0;
    }
}

void UpdatePlayer(char i)
{
    // Reset movement and action values every frame
    players[i].inputVertical = DIRECTION_NONE;
    players[i].inputHorizontal = DIRECTION_NONE;
    players[i].action = ACTION_STATIONARY;

    // Spawn second player if they have not joined
    if(!playerTwoJoined && i == PLAYER_TWO)
    {
        if ((keyStatus & PORT_B_KEY_1) || (keyStatus & PORT_B_KEY_2))
        {
            playerTwoJoined = 1;
            playersSprites[1].isVisible = 1;
            playersSprites[1].positionX = playersSprites[0].positionX;
            playersSprites[1].positionY = playersSprites[0].positionY;
            playersSprites[1].spriteX = playersSprites[0].spriteX;
            playersSprites[1].spriteY = playersSprites[0].spriteY;
        }
        return;
    }

    if(i == PLAYER_ONE)
    {
        // Reset roar button
        if (!(keyStatus & PORT_A_KEY_1)) players[i].actionOnePressed = 0;

        // Start actions
        if (players[i].actionOnePressed == 0 && (keyStatus & PORT_A_KEY_1))
        {
            players[i].actionOnePressed = 1;
            players[i].action = ACTION_ONE;
            players[i].actionCount = PLAYER_ROAR_FRAME_COUNT;
            players[i].actionFrame = PLAYER_ROAR_FRAME;
            return;
        }
        else if (keyStatus & PORT_A_KEY_2)
        {
            players[i].action = ACTION_TWO;
            players[i].actionCount = PLAYER_ACTION_FRAME_COUNT;
            players[i].actionFrame = PLAYER_ACTION_INTERACTION_FRAME;
            return;
        }

        // Read movement keys
        if (keyStatus & PORT_A_KEY_UP)
        { 
            players[i].action = ACTION_MOVE;
            players[i].inputVertical = DIRECTION_UP;
        }
        else if (keyStatus & PORT_A_KEY_DOWN)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputVertical = DIRECTION_DOWN;
        }

        if (keyStatus & PORT_A_KEY_LEFT)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputHorizontal= DIRECTION_LEFT;
        }
        else if (keyStatus & PORT_A_KEY_RIGHT)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputHorizontal = DIRECTION_RIGHT;
        }
    }
    else // Player Two
    {
        // Reset roar button
        if (!(keyStatus & PORT_B_KEY_1)) players[i].actionOnePressed = 0;

        // Start actions
        if (players[i].actionOnePressed == 0 && (keyStatus & PORT_B_KEY_1))
        {
            players[i].actionOnePressed = 1;
            players[i].action = ACTION_ONE;
            players[i].actionCount = PLAYER_ROAR_FRAME_COUNT;
            players[i].actionFrame = PLAYER_ROAR_FRAME;
            return;
        }
        else if (keyStatus & PORT_B_KEY_2)
        {
            players[i].action = ACTION_TWO;
            players[i].actionCount = PLAYER_ACTION_FRAME_COUNT;
            players[i].actionFrame = PLAYER_ACTION_INTERACTION_FRAME;
            return;
        }

        // Read movement keys
        if (keyStatus & PORT_B_KEY_UP)
        { 
            players[i].action = ACTION_MOVE;
            players[i].inputVertical = DIRECTION_UP;
        }
        else if (keyStatus & PORT_B_KEY_DOWN)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputVertical = DIRECTION_DOWN;
        }

        if (keyStatus & PORT_B_KEY_LEFT)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputHorizontal= DIRECTION_LEFT;
        }
        else if (keyStatus & PORT_B_KEY_RIGHT)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputHorizontal = DIRECTION_RIGHT;
        }
    }

    // Update player direction
    if(players[i].inputVertical == DIRECTION_UP)
    {
        if(players[i].inputHorizontal == DIRECTION_LEFT) playersSprites[i].direction = DIRECTION_UP_LEFT;
        else if(players[i].inputHorizontal == DIRECTION_RIGHT) playersSprites[i].direction = DIRECTION_UP_RIGHT;
        else playersSprites[i].direction = DIRECTION_UP;
    }
    else if(players[i].inputVertical == DIRECTION_DOWN)
    {
        if(players[i].inputHorizontal == DIRECTION_LEFT) playersSprites[i].direction = DIRECTION_DOWN_LEFT;
        else if(players[i].inputHorizontal == DIRECTION_RIGHT) playersSprites[i].direction = DIRECTION_DOWN_RIGHT;
        else playersSprites[i].direction = DIRECTION_DOWN;
    }
    else
    {
        if(players[i].inputHorizontal == DIRECTION_LEFT) playersSprites[i].direction = DIRECTION_LEFT;
        else if(players[i].inputHorizontal == DIRECTION_RIGHT) playersSprites[i].direction = DIRECTION_RIGHT;
    }

    // Update movement speed
    if(players[i].action == ACTION_MOVE)
    {
        switch (playersSprites[i].direction)
        {
            case DIRECTION_UP_LEFT:
            case DIRECTION_DOWN_LEFT:
            case DIRECTION_UP_RIGHT:
            case DIRECTION_DOWN_RIGHT:
                playersSprites[i].speed = PLAYER_SPEED_DIAGONAL;
            break;

            default:
                playersSprites[i].speed = PLAYER_SPEED_DEFAULT;
            break;
        }

        // Move player
        for(char j = 0; j < playersSprites[i].speed; ++j) MovePlayer(i);  //MovePlayerNoCollision(i); 

        // Check metatile stepped on
        unsigned char *metatile = GSL_metatileLookup(playersSprites[i].positionX, playersSprites[i].positionY);
        MetatileSwapWalkable(metatile);
    }
}

void MetatileSwapWalkable(unsigned char *metatile)
{
    for(char i = 0; i < MAX_TILE_PAIRS_WALKABLE; ++i)
    {
        if (*metatile == walkableTilePairs[i].normalTile)
        {
            *metatile = walkableTilePairs[i].flowerTile;
            GSL_metatileUpdate();
        }
    }
}

void MetatileSwapShootable(unsigned char *metatile)
{
    for(char i = 0; i < MAX_TILE_PAIRS_SHOOTABLE; ++i)
    {
        if (*metatile == shootableTilePairs[i].normalTile)
        {
            *metatile = shootableTilePairs[i].flowerTile;
            GSL_metatileUpdate();
            LoadAndPlaySFX(SFX_CONVERT);
        }
    }
}

/*
Player spite and collider in pixels, x == collider, s == player position
OOOOOOOOOOOOOOOO
OOOOOOOOOOOOOOOO
OOOOOOOOOOOOOOOO
OOOOOOOOOOOOOOOO
OOOOOOOOOOOOOOOO
OOOOOOOOOOOOOOOO
OOOOOOOOOOOOOOOO
OOOOOOOOOOOOOOOO
XXXXXXXXSXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX
*/

char GetTopLeftMetatile(char i)     { return *(GSL_metatileLookup(playersSprites[i].positionX - 8, playersSprites[i].positionY - 1)); }
char GetTopRightMetatile(char i)    { return *(GSL_metatileLookup(playersSprites[i].positionX + 7, playersSprites[i].positionY - 1)); }
char GetBottomLeftMetatile(char i)  { return *(GSL_metatileLookup(playersSprites[i].positionX - 8, playersSprites[i].positionY + 8)); }
char GetBottomRightMetatile(char i) { return *(GSL_metatileLookup(playersSprites[i].positionX + 7, playersSprites[i].positionY + 8)); }
char GetLowerLeftMetatile(char i)   { return *(GSL_metatileLookup(playersSprites[i].positionX - 9, playersSprites[i].positionY + 7)); }
char GetLowerRightMetatile(char i)  { return *(GSL_metatileLookup(playersSprites[i].positionX + 8, playersSprites[i].positionY + 7)); }
char GetLeftUpMetatile(char i)      { return *(GSL_metatileLookup(playersSprites[i].positionX - 9, playersSprites[i].positionY - 1)); }
char GetLeftDownMetatile(char i)    { return *(GSL_metatileLookup(playersSprites[i].positionX - 9, playersSprites[i].positionY + 8)); }
char GetRightUpMetatile(char i)     { return *(GSL_metatileLookup(playersSprites[i].positionX + 8, playersSprites[i].positionY - 1)); }
char GetRightDownMetatile(char i)   { return *(GSL_metatileLookup(playersSprites[i].positionX + 8, playersSprites[i].positionY + 8)); }
char IsPlayerUpBlocked(char i)          { return ((metatilesMetaLUT[GetTopLeftMetatile(i)] & PLAYER_COLLISION_VALUE) == 1 || (metatilesMetaLUT[GetTopRightMetatile(i)] & PLAYER_COLLISION_VALUE) == 1); }
char IsPlayerDownBlocked(char i)        { return (metatilesMetaLUT[GetBottomLeftMetatile(i)] & PLAYER_COLLISION_VALUE) == 1 || (metatilesMetaLUT[GetBottomRightMetatile(i)] & PLAYER_COLLISION_VALUE) == 1; }
char IsPlayerLeftBlocked(char i)        { return (metatilesMetaLUT[GetLowerLeftMetatile(i)] & PLAYER_COLLISION_VALUE) == 1; }
char IsPlayerRightBlocked(char i)       { return (metatilesMetaLUT[GetLowerRightMetatile(i)] & PLAYER_COLLISION_VALUE) == 1; }
char IsPlayerUpLeftBlocked(char i)      { return (metatilesMetaLUT[GetLeftUpMetatile(i)] & PLAYER_COLLISION_VALUE) == 1; }
char IsPlayerUpRightBlocked(char i)     { return (metatilesMetaLUT[GetRightUpMetatile(i)] & PLAYER_COLLISION_VALUE) == 1; }
char IsPlayerDownLeftBlocked(char i)    { return (metatilesMetaLUT[GetLeftDownMetatile(i)] & PLAYER_COLLISION_VALUE) == 1; }
char IsPlayerDownRightBlocked(char i)   { return (metatilesMetaLUT[GetRightDownMetatile(i)] & PLAYER_COLLISION_VALUE) == 1; }

// Moves player one pixel
void MovePlayerNoCollision(char i)
{
    switch (playersSprites[i].direction)
    {
        case DIRECTION_UP:
            MoveUp(i);
        break;

        case DIRECTION_DOWN:
            MoveDown(i);
        break;

        case DIRECTION_LEFT:
            MoveLeft(i);
        break;

        case DIRECTION_RIGHT:
            MoveRight(i);
        break;

        case DIRECTION_UP_LEFT:
            MoveUp(i);
            MoveLeft(i);
        break;

        case DIRECTION_UP_RIGHT:
            MoveUp(i);
            MoveRight(i);
        break;

        case DIRECTION_DOWN_LEFT:
            MoveDown(i);
            MoveLeft(i);
        break;

        case DIRECTION_DOWN_RIGHT:
            MoveDown(i);
            MoveRight(i);
        break;
    }
}

// Moves player one pixel
void MovePlayer(char i)
{
    switch (playersSprites[i].direction)
    {
        case DIRECTION_UP:
            if (!IsPlayerUpBlocked(i)) MoveUp(i);
        break;

        case DIRECTION_DOWN:
            if (!IsPlayerDownBlocked(i)) MoveDown(i);
        break;

        case DIRECTION_LEFT:
            if (!IsPlayerLeftBlocked(i)) MoveLeft(i);
        break;

        case DIRECTION_RIGHT:
            if (!IsPlayerRightBlocked(i)) MoveRight(i);
        break;

        case DIRECTION_UP_LEFT:
            if (IsPlayerUpLeftBlocked(i))
            {
                if(!IsPlayerUpBlocked(i)) MoveUp(i);
                else if(!IsPlayerLeftBlocked(i)) MoveLeft(i);
            }
            else
            {
                if(!IsPlayerUpBlocked(i)) MoveUp(i);
                if(!IsPlayerLeftBlocked(i)) MoveLeft(i);
            }
        break;

        case DIRECTION_UP_RIGHT:
            if (IsPlayerUpRightBlocked(i))
            {
                if(!IsPlayerUpBlocked(i)) MoveUp(i);
                else if(!IsPlayerRightBlocked(i)) MoveRight(i);
            }
            else
            {
                if(!IsPlayerUpBlocked(i)) MoveUp(i);
                if(!IsPlayerRightBlocked(i)) MoveRight(i);
            }
        break;

        case DIRECTION_DOWN_LEFT:
            if (IsPlayerDownLeftBlocked(i))
            {
                if(!IsPlayerDownBlocked(i)) MoveDown(i);
                else if(!IsPlayerLeftBlocked(i)) MoveLeft(i);
            }
            else
            {
                if(!IsPlayerDownBlocked(i)) MoveDown(i);
                if(!IsPlayerLeftBlocked(i)) MoveLeft(i);
            }
        break;

        case DIRECTION_DOWN_RIGHT:
            if (IsPlayerDownRightBlocked(i))
            {
                if(!IsPlayerDownBlocked(i)) MoveDown(i);
                else if(!IsPlayerRightBlocked(i)) MoveRight(i);
            }
            else
            {
                if(!IsPlayerDownBlocked(i)) MoveDown(i);
                if(!IsPlayerRightBlocked(i)) MoveRight(i);
            }
        break;
    }
}

void MoveUp(char i)
{
    if(i == PLAYER_ONE)
    {
        if(playersSprites[PLAYER_TWO].spriteY > SCREEN_HEIGHT - 8 && playerTwoJoined)
            return;

        if (playersSprites[i].positionY <= SCREEN_EDGE_Y || playersSprites[i].positionY > GSL_getMapHeightInPixels() - SCREEN_EDGE_Y)
        {
            playersSprites[i].spriteY--;
        }
        else 
        {
            scrollYThisFrame--;
            playersSprites[PLAYER_TWO].spriteY++;
        }
    }
    else
    {
        if(playersSprites[PLAYER_TWO].spriteY < 8)
            return;

        playersSprites[i].spriteY--;
    }

    playersSprites[i].positionY--;
}

void MoveDown(char i)
{
    if(i == PLAYER_ONE)
    {
        if(playersSprites[PLAYER_TWO].spriteY < 8 && playerTwoJoined)
            return;

        if (playersSprites[i].positionY < SCREEN_EDGE_Y || playersSprites[i].positionY >= GSL_getMapHeightInPixels() - SCREEN_EDGE_Y)
        {
            playersSprites[i].spriteY++;
        }
        else 
        {
            scrollYThisFrame++;
            playersSprites[PLAYER_TWO].spriteY--;
        }
    }
    else
    {
        if(playersSprites[PLAYER_TWO].spriteY > SCREEN_HEIGHT - 8)
            return;

        playersSprites[i].spriteY++;
    }

    playersSprites[i].positionY++;
}

void MoveLeft(char i)
{
    if(i == PLAYER_ONE)
    {
        if(playersSprites[PLAYER_TWO].spriteX > SCREEN_WIDTH - 8 && playerTwoJoined)
            return;

        if (playersSprites[i].positionX <= SCREEN_EDGE_X_OUTER || playersSprites[i].positionX > GSL_getMapWidthInPixels() - SCREEN_EDGE_X_INNER)
        {
            playersSprites[i].spriteX--;
        }
        else
        {
            scrollXThisFrame--;
            playersSprites[PLAYER_TWO].spriteX++;
        }
    }
    else
    {
        if(playersSprites[PLAYER_TWO].spriteX < 8)
            return;

        playersSprites[i].spriteX--;
    }

    playersSprites[i].positionX--;
}

void MoveRight(char i)
{
    if(i == PLAYER_ONE)
    {
        if(playersSprites[PLAYER_TWO].spriteX < 8 && playerTwoJoined)
            return;

        if (playersSprites[i].positionX < SCREEN_EDGE_X_OUTER || playersSprites[i].positionX >= GSL_getMapWidthInPixels() - SCREEN_EDGE_X_INNER)
        {
            playersSprites[i].spriteX++;
        }
        else
        {
            scrollXThisFrame++;
            playersSprites[PLAYER_TWO].spriteX--;
        }
    }
    else
    {
        if(playersSprites[PLAYER_TWO].spriteX > SCREEN_WIDTH - 8)
            return;

        playersSprites[i].spriteX++;
    }

    playersSprites[i].positionX++;
}

void UpdateAction(char i)
{
    if(players[i].actionCount == players[i].actionFrame)
    {
        if(players[i].action == ACTION_ONE)
        {
            Roar(i);
        }
        else if(players[i].action == ACTION_TWO)
        {
            ShootBullet(i);
        }
    }
}

void Roar(char i)
{
    LoadAndPlaySFX(SFX_ROAR);

    for(char j = 0; j < ENEMY_BULLET_COUNT; ++j)
    {
        if(boxCollisionToPoint(playersSprites[i].spriteX - 40, playersSprites[i].spriteY - 40, 80, enemyBullets[j].spriteX, enemyBullets[j].spriteY))
        {
            enemyBullets[j].isVisible = 0;
        }
    }
}

void ShootBullet(char i)
{
    // Loop through bullets and see if any are available
    for(char j = 0; j < PLAYER_BULLET_COUNT; ++j)
    {
        if(!players[i].bullets[j].isVisible)
        {
            players[i].bullets[j].isVisible = 1;
            players[i].bullets[j].positionX = playersSprites[i].positionX;
            players[i].bullets[j].positionY = playersSprites[i].positionY;
            players[i].bullets[j].spriteX = playersSprites[i].spriteX;
            players[i].bullets[j].spriteY = playersSprites[i].spriteY;
            players[i].bullets[j].direction = playersSprites[i].direction;
            switch (players[i].bullets[j].direction)
            {
                case DIRECTION_UP_LEFT:
                case DIRECTION_DOWN_LEFT:
                case DIRECTION_UP_RIGHT:
                case DIRECTION_DOWN_RIGHT:
                    players[i].bullets[j].speed = PLAYER_BULLET_SPEED_DIAGONAL;
                break;

                default:
                    players[i].bullets[j].speed = PLAYER_BULLET_SPEED_DEFAULT;
                break;
            }
            LoadAndPlaySFX(SFX_PLAYER_SHOOT);
            return;
        }
    }
}

void MoveBulletUp(char i, char j)       { players[i].bullets[j].positionY -= players[i].bullets[j].speed; players[i].bullets[j].spriteY -= players[i].bullets[j].speed; }
void MoveBulletDown(char i, char j)     { players[i].bullets[j].positionY += players[i].bullets[j].speed; players[i].bullets[j].spriteY += players[i].bullets[j].speed; }
void MoveBulletLeft(char i, char j)     { players[i].bullets[j].positionX -= players[i].bullets[j].speed; players[i].bullets[j].spriteX -= players[i].bullets[j].speed; }
void MoveBulletRight(char i, char j)    { players[i].bullets[j].positionX += players[i].bullets[j].speed; players[i].bullets[j].spriteX += players[i].bullets[j].speed; }

void UpdateBullets(char i)
{
    for(char j = 0; j < PLAYER_BULLET_COUNT; ++j)
    {
        if(players[i].bullets[j].isVisible)
        {
            // Move bullets based on and direction
            switch (players[i].bullets[j].direction)
            {
                case DIRECTION_UP:
                    MoveBulletUp(i, j);
                break;

                case DIRECTION_DOWN:
                    MoveBulletDown(i, j);
                break;

                case DIRECTION_LEFT:
                    MoveBulletLeft(i, j);
                break;

                case DIRECTION_RIGHT:
                    MoveBulletRight(i, j);
                break;

                case DIRECTION_UP_LEFT:
                    MoveBulletUp(i, j);
                    MoveBulletLeft(i, j);
                break;

                case DIRECTION_UP_RIGHT:
                    MoveBulletUp(i, j);
                    MoveBulletRight(i, j);
                break;

                case DIRECTION_DOWN_LEFT:
                    MoveBulletDown(i, j);
                    MoveBulletLeft(i, j);
                break;

                case DIRECTION_DOWN_RIGHT:
                    MoveBulletDown(i, j);
                    MoveBulletRight(i, j);
                break;
            }

            // Apply scrolling
            players[i].bullets[j].positionX -= scrollXThisFrame;
            players[i].bullets[j].spriteX -= scrollXThisFrame;
            players[i].bullets[j].positionY -= scrollYThisFrame;
            players[i].bullets[j].spriteY -= scrollYThisFrame;

            // Destory bullets off screen
            if(players[i].bullets[j].spriteY < 8
            || players[i].bullets[j].spriteX < 8
            || players[i].bullets[j].spriteY > SCREEN_HEIGHT - 8
            || players[i].bullets[j].spriteX > SCREEN_WIDTH - 8)
            {
                players[i].bullets[j].isVisible = 0;
                continue;
            }

            // Player bullet collision to metatiles
            unsigned char *metatile = GSL_metatileLookup(players[i].bullets[j].positionX, players[i].bullets[j].positionY);
            if(*metatile == METATILE_FACTORY || *metatile == METATILE_FACTORY_CLAIMED)
            {
                MetatileFactoryHit(metatile);
                players[i].bullets[j].isVisible = 0;
                continue;
            }
            else if(*metatile == METATILE_TURRET)
            {
                for(char k = 0; k < MAX_ACTIVE_TURRETS; ++k)
                {
                    if(boxCollisionToPointInt(players[i].bullets[j].positionX - 16, players[i].bullets[j].positionY - 16, 32, turrets[k].positionX, turrets[k].positionY))
                    {
                        MetatileSwapShootable(metatile);
                        turrets[k].isDestroyed = 1;
                        LoadAndPlaySFX(SFX_EXPLOSION);
                    }
                }
                players[i].bullets[j].isVisible = 0;
                continue;
            }
            else if(metatilesMetaLUT[*metatile] & PLAYER_COLLISION_VALUE == 1)
            {
                // Hit wall!
                players[i].bullets[j].isVisible = 0;
                continue;
            }
        }
    }
}

void MetatileFactoryHit(unsigned char *metatile)
{
    if (*metatile == METATILE_FACTORY)
    {
        *metatile = METATILE_FACTORY_CLAIMED;
        GSL_metatileUpdate();
        
        // Decrement factory count and check if game is won
        LoadAndPlaySFX(SFX_CONVERT_FACTORY);
        numFactories--;
        if (numFactories == 0)
        {
            endCounter = 60;
        }
        UpdateNumFactoriesSpriteIds();
    }
}


void MoveEnemyBulletUp(char i)       { enemyBullets[i].positionY -= enemyBullets[i].speed; enemyBullets[i].spriteY -= enemyBullets[i].speed; }
void MoveEnemyBulletDown(char i)     { enemyBullets[i].positionY += enemyBullets[i].speed; enemyBullets[i].spriteY += enemyBullets[i].speed; }
void MoveEnemyBulletLeft(char i)     { enemyBullets[i].positionX -= enemyBullets[i].speed; enemyBullets[i].spriteX -= enemyBullets[i].speed; }
void MoveEnemyBulletRight(char i)    { enemyBullets[i].positionX += enemyBullets[i].speed; enemyBullets[i].spriteX += enemyBullets[i].speed; }

void UpdateEnemyBullets(void)
{
    enemyBulletForCollisionUpdate++;
    if(enemyBulletForCollisionUpdate == ENEMY_BULLET_COUNT) enemyBulletForCollisionUpdate = 0;

    for(char i = 0; i < ENEMY_BULLET_COUNT; ++i)
    {
        if(enemyBullets[i].isVisible)
        {
            // Movement code for bullets
            switch (enemyBullets[i].direction)
            {
                case DIRECTION_UP:
                    MoveEnemyBulletUp(i);
                break;
                
                case DIRECTION_DOWN:
                    MoveEnemyBulletDown(i);
                break;
                
                case DIRECTION_LEFT:
                    MoveEnemyBulletLeft(i);
                break;
                
                case DIRECTION_RIGHT:
                    MoveEnemyBulletRight(i);
                break;
                
                case DIRECTION_UP_LEFT:
                    MoveEnemyBulletUp(i);
                    MoveEnemyBulletLeft(i);
                break;
                
                case DIRECTION_UP_RIGHT:
                    MoveEnemyBulletUp(i);
                    MoveEnemyBulletRight(i);
                break;
                
                case DIRECTION_DOWN_LEFT:
                    MoveEnemyBulletDown(i);
                    MoveEnemyBulletLeft(i);
                break;
                
                case DIRECTION_DOWN_RIGHT:
                    MoveEnemyBulletDown(i);
                    MoveEnemyBulletRight(i);
                break;
            }
            
            // Apply scrolling
            enemyBullets[i].positionX -= scrollXThisFrame;
            enemyBullets[i].spriteX -= scrollXThisFrame;
            enemyBullets[i].positionY -= scrollYThisFrame;
            enemyBullets[i].spriteY -= scrollYThisFrame;

            // Check for off-screen
            if(enemyBullets[i].spriteY < 8
            || enemyBullets[i].spriteX < 8
            || enemyBullets[i].spriteY > SCREEN_HEIGHT - 8
            || enemyBullets[i].spriteX > SCREEN_WIDTH - 8)
            {
                enemyBullets[i].isVisible = 0;
                continue;
            }

            // Check collisions with players, only one bullet collision check per frame
            if(enemyBulletForCollisionUpdate == i)
            {
                for(char j = 0; j < PLAYER_COUNT; ++j)
                {
                    if(playersSprites[j].isVisible && players[j].action != ACTION_STUN)
                    {
                        if(spriteToSpriteCollision(&playersSprites[j], &enemyBullets[i]))
                        {
                            LoadAndPlaySFX(SFX_EXPLOSION);
                            players[j].action = ACTION_STUN;
                            players[j].actionCount = PLAYER_STUN_FRAME_COUNT;
                            players[j].actionFrame = 99;
                            enemyBullets[i].isVisible = 0;
                            continue;
                        }
                    }
                }
            }
        }
    }
}

void UpdateEnemyBulletPosition(char i)
{
    // Add fractional parts first
    unsigned int newSubpixelX = enemyBullets[i].subpixelX + enemyBullets[i].velocityX_frac;
    unsigned int newSubpixelY = enemyBullets[i].subpixelY + enemyBullets[i].velocityY_frac;
    
    // Update integer positions with any carry from fractional addition
    enemyBullets[i].positionX += enemyBullets[i].velocityX + (newSubpixelX >> 8);
    enemyBullets[i].positionY += enemyBullets[i].velocityY + (newSubpixelY >> 8);
    
    // Store only the low byte of subpixel positions
    enemyBullets[i].subpixelX = newSubpixelX & 0xFF;
    enemyBullets[i].subpixelY = newSubpixelY & 0xFF;
    
    // Update sprite positions for rendering
    enemyBullets[i].spriteX = enemyBullets[i].positionX - scrollXTotal;
    enemyBullets[i].spriteY = enemyBullets[i].positionY - scrollYTotal;
}
/// Enemy AI Logic Start

void InitTurrets(void) {
    // Initialize all turrets as active for debugging purposes
    for(char i = 0; i < MAX_ACTIVE_TURRETS; ++i) {
        turrets[i].isActive = 1;
        turrets[i].isDestroyed = 0;
        turrets[i].shootTimer = 0;
        // Default to random shooting
        turrets[i].fireMode = 1;
    }
}


// Returns index of closest player (0 or 1) to given position
// If player two is not active, always returns 0 (player 1)
char GetClosestPlayer(unsigned int posX, unsigned int posY) {
    if (!playerTwoJoined) return PLAYER_ONE;
    
    // Calculate Manhattan distance to player one
    int distX1 = playersSprites[PLAYER_ONE].positionX > posX ? 
                 playersSprites[PLAYER_ONE].positionX - posX : 
                 posX - playersSprites[PLAYER_ONE].positionX;
                 
    int distY1 = playersSprites[PLAYER_ONE].positionY > posY ? 
                 playersSprites[PLAYER_ONE].positionY - posY : 
                 posY - playersSprites[PLAYER_ONE].positionY;
    
    int totalDist1 = distX1 + distY1;
    
    // Calculate distance to player two
    int distX2 = playersSprites[PLAYER_TWO].positionX > posX ? 
                 playersSprites[PLAYER_TWO].positionX - posX : 
                 posX - playersSprites[PLAYER_TWO].positionX;
                 
    int distY2 = playersSprites[PLAYER_TWO].positionY > posY ? 
                 playersSprites[PLAYER_TWO].positionY - posY : 
                 posY - playersSprites[PLAYER_TWO].positionY;
    
    int totalDist2 = distX2 + distY2;
    
    // Return index of closest player
    return (totalDist1 <= totalDist2) ? PLAYER_ONE : PLAYER_TWO;
}

// Lookup table for direction based on angle octants
// Maps from 0-7 to DIRECTION_UP, DIRECTION_UP_RIGHT, etc.
const unsigned char directionLookup[8] = {
    DIRECTION_RIGHT,
    DIRECTION_UP_RIGHT,
    DIRECTION_UP,
    DIRECTION_UP_LEFT,
    DIRECTION_LEFT,
    DIRECTION_DOWN_LEFT,
    DIRECTION_DOWN,
    DIRECTION_DOWN_RIGHT
};

// Determines which of 8 directions to fire in to aim at a target
// Returns DIRECTION_* value
// char GetEnemyFireDirection(unsigned int sourceX, unsigned int sourceY, 
//                           unsigned int targetX, unsigned int targetY) {
    
//     // Calculate deltas (these can be negative)
//     int deltaX = 0;
//     int deltaY = 0;
    
//     // Handle potential overflow in subtraction
//     if (targetX >= sourceX) {
//         deltaX = targetX - sourceX;
//     } else {
//         deltaX = -(sourceX - targetX);
//     }
    
//     if (targetY >= sourceY) {
//         deltaY = targetY - sourceY;
//     } else {
//         deltaY = -(sourceY - targetY);
//     }
    
//     // Determine the direction based on delta values
//     char direction;
    
//     // Check if mostly vertical
//     if (abs_delta(deltaY) > abs_delta(deltaX)) {
//         // Vertical movement dominant
//         if (deltaY < 0) {
//             // Moving up
//             if (deltaX < 0) direction = DIRECTION_UP_LEFT;
//             else if (deltaX > 0) direction = DIRECTION_UP_RIGHT;
//             else direction = DIRECTION_UP;
//         } else {
//             // Moving down
//             if (deltaX < 0) direction = DIRECTION_DOWN_LEFT;
//             else if (deltaX > 0) direction = DIRECTION_DOWN_RIGHT;
//             else direction = DIRECTION_DOWN;
//         }
//     } else {
//         // Horizontal movement dominant
//         if (deltaX < 0) {
//             // Moving left
//             if (deltaY < 0) direction = DIRECTION_UP_LEFT;
//             else if (deltaY > 0) direction = DIRECTION_DOWN_LEFT;
//             else direction = DIRECTION_LEFT;
//         } else {
//             // Moving right
//             if (deltaY < 0) direction = DIRECTION_UP_RIGHT;
//             else if (deltaY > 0) direction = DIRECTION_DOWN_RIGHT;
//             else direction = DIRECTION_RIGHT;
//         }
//     }
    
//     return direction;
// }

char GetEnemyFireDirection(unsigned int sourceX, unsigned int sourceY, 
    unsigned int targetX, unsigned int targetY) {

    // Calculate deltas (these can be negative)
    int deltaX = 0;
    int deltaY = 0;

    // Handle potential overflow in subtraction
    if (targetX >= sourceX) {
    deltaX = targetX - sourceX;
    } else {
    deltaX = -(sourceX - targetX);
    }

    if (targetY >= sourceY) {
    deltaY = targetY - sourceY;
    } else {
    deltaY = -(sourceY - targetY);
    }

    // Determine octant (0-7) based on deltas
    char octant;

    // First determine quadrant
    if (deltaY < 0) {
    if (deltaX < 0) {
    // Upper-left quadrant
    if (abs_delta(deltaY) > abs_delta(deltaX) * 2) {
    octant = 6; // Mostly up
    } else if (abs_delta(deltaX) > abs_delta(deltaY) * 2) {
    octant = 4; // Mostly left
    } else {
    octant = 5; // Diagonal up-left
    }
    } else {
    // Upper-right quadrant
    if (abs_delta(deltaY) > abs_delta(deltaX) * 2) {
    octant = 6; // Mostly up
    } else if (abs_delta(deltaX) > abs_delta(deltaY) * 2) {
    octant = 0; // Mostly right
    } else {
    octant = 7; // Diagonal up-right
    }
    }
    } else {
    if (deltaX < 0) {
    // Lower-left quadrant
    if (abs_delta(deltaY) > abs_delta(deltaX) * 2) {
    octant = 2; // Mostly down
    } else if (abs_delta(deltaX) > abs_delta(deltaY) * 2) {
    octant = 4; // Mostly left
    } else {
    octant = 3; // Diagonal down-left
    }
    } else {
    // Lower-right quadrant
    if (abs_delta(deltaY) > abs_delta(deltaX) * 2) {
    octant = 2; // Mostly down
    } else if (abs_delta(deltaX) > abs_delta(deltaY) * 2) {
    octant = 0; // Mostly right
    } else {
    octant = 1; // Diagonal down-right
    }
    }
}

// Map octant to corresponding direction in 128-direction system
// Using the existing mapping from directionTo128 where possible
// 0 = RIGHT (0°), 16 = DOWN_RIGHT (45°), 32 = DOWN (90°), etc.
const unsigned char octantToDirection[8] = {
0,    // RIGHT
16,   // DOWN_RIGHT
32,   // DOWN
48,   // DOWN_LEFT
64,   // LEFT
80,   // UP_LEFT
96,   // UP
112   // UP_RIGHT
};

return octantToDirection[octant];
}

// gets absolute value
int abs_delta(int delta)
{
    return (delta < 0) ? -delta : delta;
}

/// Enemy AI Logic End


// TURRET LOGIC START

void ScanMapForTurrets(void)
{
    unsigned int mapWidth = GSL_getMapWidthInPixels();
    unsigned int mapHeight = GSL_getMapHeightInPixels();
    unsigned char activeTurretCount = 0;
    
    // Reset all turrets first
    for (char i = 0; i < MAX_ACTIVE_TURRETS; i++)
    {
        turrets[i].isActive = 0;
        turrets[i].isDestroyed = 0;
    }
    
    // Color to animate loading
    char color = 0; 
    char colorChangeCounter = 0;

    // Loop through the map in 16-pixel steps (instead of 8)
    for (unsigned int y = 0; y < mapHeight; y += 16)
    {
        colorChangeCounter++;
        if(colorChangeCounter > 8)
        {
            color++;
            SMS_setColor(color);
            SMS_setNextSpriteColoratIndex(0);
            colorChangeCounter = 0;
        }

        for (unsigned int x = 0; x < mapWidth; x += 16)
        {
            // Get the metatile at this position
            unsigned char *metatile = GSL_metatileLookup(x, y);
            
            // Check if this is a turret
            if (*metatile == METATILE_TURRET)
            {
                // Store world coordinates
                if (activeTurretCount < MAX_ACTIVE_TURRETS) {
                    turrets[activeTurretCount].positionX = x;
                    turrets[activeTurretCount].positionY = y;
                    turrets[activeTurretCount].isActive = 0; // Start inactive
                    turrets[activeTurretCount].isDestroyed = 0;
                    turrets[activeTurretCount].shootTimer = 0;
                    turrets[activeTurretCount].fireMode = (activeTurretCount % 2); // Alternate modes
                    
                    activeTurretCount++;
                }
            }
        }
    }
    
    // Reset the color after scanning is complete
    SMS_setColor(0);
}

void CheckTurretsInBoundingBox(void)
{
    // Get current viewport boundaries from GSLib
    unsigned int viewportLeft = GSL_getCurrentX();
    unsigned int viewportTop = GSL_getCurrentY();
    unsigned int viewportRight = viewportLeft + SCREEN_WIDTH;
    unsigned int viewportBottom = viewportTop + SCREEN_HEIGHT;
    
    // Add a small margin around the viewport to activate turrets just after they appear on screen.
    unsigned int activationLeft = viewportLeft + 4;
    unsigned int activationRight = viewportRight - 4;
    unsigned int activationTop = viewportTop + 2;
    unsigned int activationBottom = viewportBottom - 2;

    // Prevent right/bottom from exceeding map boundaries
    unsigned int mapWidth = GSL_getMapWidthInPixels();
    unsigned int mapHeight = GSL_getMapHeightInPixels();
    
    if (activationRight > mapWidth) activationRight = mapWidth;
    if (activationBottom > mapHeight) activationBottom = mapHeight;
    
    // Check a single turret per frame
    if (!turrets[turretCheckIndex].isDestroyed)
    {
        // Check if this turret is within the activation area
        if (turrets[turretCheckIndex].positionX >= activationLeft && 
            turrets[turretCheckIndex].positionX <= activationRight &&
            turrets[turretCheckIndex].positionY >= activationTop && 
            turrets[turretCheckIndex].positionY <= activationBottom)
        {
            if(!turrets[turretCheckIndex].isActive)
            {
                // Play sound effect when turret is activated
                LoadAndPlaySFX(SFX_MAP);
            }
            // Activate this turret
            turrets[turretCheckIndex].isActive = 1;
        }
        else
        {
            // Deactivate turrets outside the activation area
            turrets[turretCheckIndex].isActive = 0;
        }
    }
    
    // Move to next turret
    turretCheckIndex++;
    if (turretCheckIndex >= MAX_ACTIVE_TURRETS)
    {
        turretCheckIndex = 0;
    }
}

void UpdateTurrets(void)
{
    // Only process active turrets
    for (char i = 0; i < MAX_ACTIVE_TURRETS; i++)
    {
        if (turrets[i].isActive && !turrets[i].isDestroyed)
        {
            // Increment shoot timer
            turrets[i].shootTimer++;
            
            // Check if it's time to shoot
            if (turrets[i].shootTimer > TURRET_SHOOT_RATE)
            {
                // Reset timer
                turrets[i].shootTimer = 0;
                
                // Determine direction based on fire mode
                char direction = DIRECTION_DOWN;
                
                switch (turrets[i].fireMode)
                {
                    case 0: // Random firing mode
                        direction = RandomDirection();
                        break;
                        
                    case 1: // Player-targeted mode
                        {
                            // Get the closest player
                            char playerIndex = GetClosestPlayer(turrets[i].positionX, turrets[i].positionY);
                            
                            // Calculate direction to that player
                            direction = GetEnemyFireDirection(
                                turrets[i].positionX, 
                                turrets[i].positionY,
                                playersSprites[playerIndex].positionX, 
                                playersSprites[playerIndex].positionY
                            );
                        }
                        break;
                        
                    default: // Fallback to random
                        direction = RandomDirection();
                        break;
                }
                
                // Try to shoot a bullet
                ShootTurretBullet(i, direction);
            }
        }
    }
}

// char ShootTurretBullet(char turretIndex, char oldDirection) {
//     // Find free bullet slot
//     for (char i = 0; i < ENEMY_BULLET_COUNT; i++) {
//         if (!enemyBullets[i].isVisible) {
//             // Convert from old 8-direction to new 128-direction system
//             unsigned char newDirection = ConvertOldDirectionToNew(oldDirection);
            
//             // Initialize position
//             enemyBullets[i].isVisible = 1;
//             enemyBullets[i].positionX = turrets[turretIndex].positionX + 8;
//             enemyBullets[i].positionY = turrets[turretIndex].positionY + 8;
//             enemyBullets[i].spriteX = enemyBullets[i].positionX - scrollXTotal;
//             enemyBullets[i].spriteY = enemyBullets[i].positionY - scrollYTotal;
            
//             // Reset subpixel components
//             enemyBullets[i].subpixelX = 0;
//             enemyBullets[i].subpixelY = 0;
            
//             // Get vector components directly from the lookup tables
//             DirectionVector vector;
//             GetDirectionVector(newDirection, &vector);
            
//             // Adjust speed based on direction (diagonal or cardinal)
//             if (directionFlags[newDirection] & DIRECTION_FLAG_DIAGONAL) {
//                 // For diagonals, scale down by about 0.7 (approximately 181/256)
//                 // We apply the scaling by working with the vector components
//                 enemyBullets[i].velocityX = (vector.x * 181) >> 8;
//                 enemyBullets[i].velocityY = (vector.y * 181) >> 8;
                
//                 // Store fractional parts
//                 enemyBullets[i].velocityX_frac = (vector.x * 181) & 0xFF;
//                 enemyBullets[i].velocityY_frac = (vector.y * 181) & 0xFF;
//             } else {
//                 // For cardinal directions, use full speed
//                 enemyBullets[i].velocityX = vector.x >> 8;
//                 enemyBullets[i].velocityY = vector.y >> 8;
//                 enemyBullets[i].velocityX_frac = vector.x & 0xFF;
//                 enemyBullets[i].velocityY_frac = vector.y & 0xFF;
//             }
            
//             // Store the original direction value for compatibility
//             enemyBullets[i].direction = oldDirection;
            
//             // Play sound effect if bullet is on screen
//             if (enemyBullets[i].spriteX >= 8 && enemyBullets[i].spriteX <= SCREEN_WIDTH - 8 &&
//                 enemyBullets[i].spriteY >= 8 && enemyBullets[i].spriteY <= SCREEN_HEIGHT - 8) {
//                 LoadAndPlaySFX(SFX_ENEMY_SHOOT);
//             }
            
//             return i;
//         }
//     }
//     return -1;
// }

char ShootTurretBullet(char turretIndex, char direction) {
    // Find free bullet slot
    for (char i = 0; i < ENEMY_BULLET_COUNT; i++) {
        if (!enemyBullets[i].isVisible) {
            // Direction is already in the 128-direction system
            // No need to convert from old 8-direction system
            
            // Initialize position
            enemyBullets[i].isVisible = 1;
            enemyBullets[i].positionX = turrets[turretIndex].positionX + 8;
            enemyBullets[i].positionY = turrets[turretIndex].positionY + 8;
            enemyBullets[i].spriteX = enemyBullets[i].positionX - scrollXTotal;
            enemyBullets[i].spriteY = enemyBullets[i].positionY - scrollYTotal;
            
            // Reset subpixel components
            enemyBullets[i].subpixelX = 0;
            enemyBullets[i].subpixelY = 0;
            
            // Get vector components directly from the lookup tables
            DirectionVector vector;
            GetDirectionVector(direction, &vector);
            
            // Adjust speed based on direction (diagonal or cardinal)
            if (directionFlags[direction] & DIRECTION_FLAG_DIAGONAL) {
                // For diagonals, scale down by about 0.7 (approximately 181/256)
                enemyBullets[i].velocityX = (vector.x * 181) >> 8;
                enemyBullets[i].velocityY = (vector.y * 181) >> 8;
                
                // Store fractional parts
                enemyBullets[i].velocityX_frac = (vector.x * 181) & 0xFF;
                enemyBullets[i].velocityY_frac = (vector.y * 181) & 0xFF;
            } else {
                // For cardinal directions, use full speed
                enemyBullets[i].velocityX = vector.x >> 8;
                enemyBullets[i].velocityY = vector.y >> 8;
                enemyBullets[i].velocityX_frac = vector.x & 0xFF;
                enemyBullets[i].velocityY_frac = vector.y & 0xFF;
            }
            
            // Store the original direction value for compatibility with existing code
            // We can map back to the old 8-direction system if needed
            switch (direction) {
                case 0: enemyBullets[i].direction = DIRECTION_RIGHT; break;
                case 16: enemyBullets[i].direction = DIRECTION_DOWN_RIGHT; break;
                case 32: enemyBullets[i].direction = DIRECTION_DOWN; break;
                case 48: enemyBullets[i].direction = DIRECTION_DOWN_LEFT; break;
                case 64: enemyBullets[i].direction = DIRECTION_LEFT; break;
                case 80: enemyBullets[i].direction = DIRECTION_UP_LEFT; break;
                case 96: enemyBullets[i].direction = DIRECTION_UP; break;
                case 112: enemyBullets[i].direction = DIRECTION_UP_RIGHT; break;
                default: enemyBullets[i].direction = DIRECTION_DOWN; break;
            }
            
            // Play sound effect if bullet is on screen
            if (enemyBullets[i].spriteX >= 8 && enemyBullets[i].spriteX <= SCREEN_WIDTH - 8 &&
                enemyBullets[i].spriteY >= 8 && enemyBullets[i].spriteY <= SCREEN_HEIGHT - 8) {
                LoadAndPlaySFX(SFX_ENEMY_SHOOT);
            }
            
            return i;
        }
    }
    return -1;
}

// Helper function to get a random direction (1-8), should not return same direction twice in a row
// char RandomDirection(void) {
//     // Get a random byte
//     unsigned char randomValue = nextRandomByte();
    
//     // Use the low 3 bits (0-7) to index our direction map
//     unsigned char dirIndex = randomValue & 0x07;
    
//     // Get a new direction from the lookup table
//     char newDirection = directionMap[dirIndex];
    
//     // Avoid repeating the last direction
//     if (newDirection == lastRandomDirection) {
//         // Simple increment with wrap around, using just bitwise ops
//         dirIndex = (dirIndex + 1) & 0x07;
//         newDirection = directionMap[dirIndex];
//     }
    
//     // Store this direction for next time
//     lastRandomDirection = newDirection;
    
//     return newDirection;
// }

// Helper function to get a random direction from the 8 cardinal/ordinal directions and directly interface with the lookup tables
char RandomDirection(void) {
    // Get a random byte
    unsigned char randomValue = nextRandomByte();
    
    // Use the low 3 bits (0-7) to get a value between 0-7
    unsigned char dirIndex = randomValue & 0x07;
    
    // Convert to the 128-direction system directly
    // These values are at 45-degree intervals (0, 16, 32, 48, 64, 80, 96, 112)
    char newDirection = dirIndex << 4; // Multiply by 16
    
    // Avoid repeating the last direction
    if (newDirection == lastRandomDirection) {
        // Simple increment with wrap around (to next 45-degree direction)
        dirIndex = (dirIndex + 1) & 0x07;
        newDirection = dirIndex << 4;
    }
    
    // Store this direction for next time
    lastRandomDirection = newDirection;
    
    return newDirection;
}


// Pseudo-random number generator helper
unsigned char nextRandomByte(void) {
    static unsigned char seed = 127; // Initial seed value
    
    // Simple LFSR-style operation using just shifts and XOR
    seed = (seed << 1) | ((((seed >> 7) ^ (seed >> 5) ^ (seed >> 4) ^ (seed >> 3)) & 1));
    
    return seed;
}