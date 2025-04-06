#include "..\lib\SMSlib.h"
#include "..\lib\PSGlib.h"
#include "..\lib\GSLib.h"

#include "..\banks\bank2.h"
#include "..\banks\bank3.h"
#include "..\banks\bank4.h"

#include "resources.c"
#include "objects.c"

// Player Logic
void UpdatePlayerAnimations(char i);
void UpdatePlayer(char i);
void MovePlayerNoCollision(char i);
void MovePlayer(char i);
void MoveUp(char i);
void MoveDown(char i);
void MoveLeft(char i);
void MoveRight(char i);
void UpdateAction(char i);

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
void StopMusic(void);

#define DIRECTION_NONE  	    0
#define DIRECTION_UP 			1
#define DIRECTION_DOWN 			2
#define DIRECTION_LEFT  		3
#define DIRECTION_RIGHT  		4
#define DIRECTION_UP_LEFT		5
#define DIRECTION_UP_RIGHT 		6
#define DIRECTION_DOWN_LEFT  	7
#define DIRECTION_DOWN_RIGHT  	8

#define ACTION_STATIONARY 		0
#define ACTION_ONE  			1
#define ACTION_TWO  			2
#define ACTION_MOVE  			3
#define ACTION_STUN  			4

// Metatile ids require updating every time the map is exported
#define METATILE_TRIGGER_OFF    8
#define METATILE_TRIGGER_ON     104
#define METATILE_TURRET         0

#define PLAYER_COUNT            2
#define PLAYER_ONE              0
#define PLAYER_TWO              1
#define PLAYER_START_X          904
#define PLAYER_START_Y          928
#define PLAYER_SPRITE_START_X   136
#define PLAYER_SPRITE_START_Y   96
#define PLAYER_ACTION_FRAME_COUNT       15
#define PLAYER_ACTION_INTERACTION_FRAME 14
#define PLAYER_STUN_FRAME_COUNT         20
#define PLAYER_COLLISION_VALUE  	    1
#define PLAYER_ANIMATION_HOLD_DURATION  4

// Screenspace defines
#define SCREEN_WIDTH            256
#define SCREEN_HEIGHT           192
#define FONT_VRAM_OFFSET        320
#define START_WOLRD_OFFSET_X    768
#define START_WOLRD_OFFSET_Y    832

// Game states
#define GAME_STATE_TITLE    0
#define GAME_STATE_GAME     1
#define GAME_STATE_END      2

// Game settings
#define MAX_FACTORY_NUM 5

// Enemies
#define ENEMY_BULLET_COUNT          6
#define ENEMY_BULLET_SPEED_DEFAULT  2

const unsigned char PLAYER_SPEED_DEFAULT =  2;
const unsigned char PLAYER_SPEED_DIAGONAL = 1;
const unsigned char PLAYER_BULLET_SPEED_DEFAULT =  4;
const unsigned char PLAYER_BULLET_SPEED_DIAGONAL = 2;

const unsigned char SCREEN_EDGE_Y       = 96;
const unsigned char SCREEN_EDGE_X_INNER = 120;
const unsigned char SCREEN_EDGE_X_OUTER = 136;

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

unsigned char scrolltable[ugtbatch_scrolltable_bin_size];

// Gamestate and counters
unsigned char gameState = GAME_STATE_GAME;
unsigned char numFactories = MAX_FACTORY_NUM; // when this reaches 0, game is won

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
                // Simple rendering for title screen - no scrolling needed
                SMS_waitForVBlank();
                //UNSAFE_SMS_copySpritestoSAT();
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
                // Simple rendering for end screen - no scrolling needed
                SMS_waitForVBlank();
                //UNSAFE_SMS_copySpritestoSAT();
                HandleEndScreen();
                break;
        }
        
        // Play audio
        PSGFrame();
    }
}

void LoadAndPlayMusic(void) 
{
    // switch to bank that has music
    SMS_mapROMBank(kaijulove_psg_bank);
    PSGPlay(&kaijulove_psg);
}

void StopMusic(void)
{
    PSGStop();
}

// title and end screen handling
void LoadTitleScreen(void)
{
    StopMusic();

    gameState = GAME_STATE_TITLE;

    // Clear the screen and load title graphics
    SMS_displayOff();
    SMS_VRAMmemsetW(0, 0, 0x4000); // Clear VRAM
    
    // Load the title screen tiles and data
    SMS_mapROMBank(startendpalette_pal_bin_bank);
    SMS_loadBGPalette(&startendpalette_pal_bin);
    SMS_loadTiles(&start_tiles_bin, 0, start_tiles_bin_size);
    SMS_loadTileMap(0,0,&start_map_bin, start_map_bin_size);
    
    // Reset scroll position
    SMS_setBGScrollX(0);
    SMS_setBGScrollY(0);
    
    SMS_displayOn();
}

void LoadEndScreen(void)
{
    StopMusic();

    gameState = GAME_STATE_END;

    // Clear the screen and load end screen graphics
    SMS_displayOff();
    SMS_VRAMmemsetW(0, 0, 0x4000); // Clear VRAM
    
    // Load the end screen tiles and data
    SMS_mapROMBank(startendpalette_pal_bin_bank);
    SMS_loadBGPalette(&startendpalette_pal_bin);
    SMS_loadTiles(&end_tiles_bin, 0, end_tiles_bin_size);
    SMS_loadTileMap(0,0,&end_map_bin, end_map_bin_size);
    
    // Reset scroll position
    SMS_setBGScrollX(0);
    SMS_setBGScrollY(0);
    
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
    SMS_displayOn();

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
        playersSprites[i].spriteOneIndex = (i << 2);
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
        enemyBullets[i].size = 8;
        enemyBullets[i].speed = 4;
        enemyBullets[i].direction = DIRECTION_DOWN;
        enemyBullets[i].spriteOneIndex = 12;
        enemyBullets[i].spriteTwoIndex = 0;
    }

    // Reset game progress variables
    numFactories = MAX_FACTORY_NUM;
    playerTwoJoined = 0;

    // Init audio & play music
    LoadAndPlayMusic();

    // Set bank for streaming tiles (should use same bank as audio)
    SMS_mapROMBank(player_tiles_bin_bank);
}

void HandleTitleScreen(void)
{
    // TODO: Prevent holding key 1 for direct skipping after end screen

    // Check for player one action button to start game
    if (keyStatus & PORT_A_KEY_1)
    {
        LoadGameScreen();
    }
}

void HandleEndScreen(void)
{
    // Check for button press to return to title
    if (keyStatus & PORT_A_KEY_1)
    {
        gameState = GAME_STATE_TITLE;
        LoadTitleScreen();
    }
}

void HandleGameScreen(void)
{
    // Shoot take n steps, 0 means new actions can be started
    for(char i = 0; i < PLAYER_COUNT; ++i)
    {
        if (players[i].actionCount == 0) UpdatePlayer(i);
        if (players[i].actionCount != 0) { players[i].actionCount--; UpdateAction(i); }
        UpdatePlayerAnimations(i);
        UpdateBullets(i);
        UpdateEnemyBullets();
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
}

void RenderSpritesUnsafe(void)
{
    for(char i = 0; i< PLAYER_COUNT; ++i)
    {
        if(!playersSprites[i].isVisible) return;
        UNSAFE_SMS_VRAMmemcpy128(players[i].ramDataAddress, &player_tiles_bin[playersSprites[i].animationFrameData[playersSprites[i].currentAnimationFrame]]);
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

void UpdatePlayer(char i)
{
    // Reset scroll, Player One only
    if(i == PLAYER_ONE)
    {
        scrollXThisFrame = 0;
        scrollYThisFrame = 0;
    }

    // Reset movement values every frame
	players[i].action = ACTION_STATIONARY;
    players[i].inputVertical = DIRECTION_NONE;
    players[i].inputHorizontal = DIRECTION_NONE;

    // TSpawn second player if they have not joined
    if(i == PLAYER_TWO && !playerTwoJoined)
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
        //return; TODO: Enable this later
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
            players[i].actionCount = PLAYER_ACTION_FRAME_COUNT;
            return;
        }
        else if (keyStatus & PORT_A_KEY_2)
        {
            players[i].action = ACTION_TWO;
            players[i].actionCount = PLAYER_ACTION_FRAME_COUNT;
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
            players[i].actionCount = PLAYER_ACTION_FRAME_COUNT;
            return;
        }
        else if (keyStatus & PORT_B_KEY_2)
        {
            players[i].action = ACTION_TWO;
            players[i].actionCount = PLAYER_ACTION_FRAME_COUNT;
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
        for(char j = 0; j < playersSprites[i].speed; ++j) MovePlayer(i); //MovePlayerNoCollision(i);
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
    if(players[i].actionCount == PLAYER_ACTION_INTERACTION_FRAME)
    {
        if(players[i].action == ACTION_ONE)
        {
            //PSGSFXPlay(&explosion0_psg, SFX_CHANNEL3);
        }
        else if(players[i].action == ACTION_TWO)
        {
            ShootBullet(i);
        }
    }
}

// Modify your MetatileInteraction function to load the end screen
void MetatileInteraction(unsigned char *metatile)
{
    if (*metatile == METATILE_TRIGGER_ON)
    {
        *metatile = METATILE_TRIGGER_OFF;
        GSL_metatileUpdate();
    }
    else if (*metatile == METATILE_TRIGGER_OFF)
    {
        *metatile = METATILE_TRIGGER_ON;
        GSL_metatileUpdate();
        
        // Decrement factory count and check if game is won
        numFactories--;
        if (numFactories == 0)
        {
            gameState = GAME_STATE_END;
            LoadEndScreen();  // Load the end screen when game is won
            
            // TODO end screen song
            // PSGPlay(&end_psg);
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
            return;
        }
    }
}

void MoveBulletUp(char i, char j)       { players[i].bullets[j].positionY--; players[i].bullets[j].spriteY--; }
void MoveBulletDown(char i, char j)     { players[i].bullets[j].positionY++; players[i].bullets[j].spriteY++; }
void MoveBulletLeft(char i, char j)     { players[i].bullets[j].positionX--; players[i].bullets[j].spriteX--; }
void MoveBulletRight(char i, char j)    { players[i].bullets[j].positionX++; players[i].bullets[j].spriteX++; }

void UpdateBullets(char i)
{
    for(char j = 0; j < PLAYER_BULLET_COUNT; ++j)
    {
        if(players[i].bullets[j].isVisible)
        {
            // Move bullets based on speed and direction
            for(char k = 0; k < players[i].bullets[j].speed; ++k)
            {
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
            }

            // Player bullet collision to metatiles
            unsigned char metatile = *GSL_metatileLookup(players[i].bullets[j].positionX, players[i].bullets[j].positionY);
            if(metatile == METATILE_TRIGGER_OFF || metatile == METATILE_TRIGGER_ON)
            {
                MetatileFactoryHit(metatile);
                players[i].bullets[j].isVisible = 0;
            }
            else if(metatile == METATILE_TURRET)
            {
                // TODO: Hit turret!
                players[i].bullets[j].isVisible = 0;
            }
            else if(metatilesMetaLUT[metatile] & PLAYER_COLLISION_VALUE == 1)
            {
                // Hit wall!
                players[i].bullets[j].isVisible = 0;
            }

            // Player bullet collision to enemy sprites
            // TODO
        }
    }
}

void MetatileFactoryHit(unsigned char *metatile)
{
    if (metatile == METATILE_TRIGGER_ON)
    {
        metatile = METATILE_TRIGGER_OFF;
        GSL_metatileUpdate();
    }
    else if (metatile == METATILE_TRIGGER_OFF)
    {
        metatile = METATILE_TRIGGER_ON;
        GSL_metatileUpdate();
        
        // Decrement factory count and check if game is won
        numFactories--;
        if (numFactories == 0)
        {
            LoadEndScreen();
        }
    }
}

void MoveEnemyBulletUp(char i)       { enemyBullets[i].positionY -= enemyBullets[i].speed; enemyBullets[i].spriteY -= enemyBullets[i].speed; }
void MoveEnemyBulletDown(char i)     { enemyBullets[i].positionY += enemyBullets[i].speed; enemyBullets[i].spriteY += enemyBullets[i].speed; }
void MoveEnemyBulletLeft(char i)     { enemyBullets[i].positionX -= enemyBullets[i].speed; enemyBullets[i].spriteX -= enemyBullets[i].speed; }
void MoveEnemyBulletRight(char i)    { enemyBullets[i].positionX += enemyBullets[i].speed; enemyBullets[i].spriteX += enemyBullets[i].speed; }

void UpdateEnemyBullets(void)
{
    // DEBUG BULLET SHOOT STARTS
    bulletShootTimer++;
    if(bulletShootTimer > bulletShootRate)
    {
        bulletShootTimer = 0;

        for(char i = 0; i < ENEMY_BULLET_COUNT; ++i)
        {
            if(!enemyBullets[i].isVisible)
            {
                enemyBullets[i].isVisible = 1;
                enemyBullets[i].positionX = playersSprites[PLAYER_ONE].positionX - 64;
                enemyBullets[i].positionY = playersSprites[PLAYER_ONE].positionY - 64;
                enemyBullets[i].spriteX = playersSprites[PLAYER_ONE].spriteX - 64;
                enemyBullets[i].spriteY = playersSprites[PLAYER_ONE].spriteY - 64;
                enemyBullets[i].speed = ENEMY_BULLET_SPEED_DEFAULT;
                enemyBullets[i].direction = bulletShootDir;
                bulletShootDir++;
                if(bulletShootDir > 8) bulletShootDir = 1;
                break;
            }
        }
    }
    // DEBUG BULLET SHOOT ENDS

    for(char i = 0; i < ENEMY_BULLET_COUNT; ++i)
    {
        if(enemyBullets[i].isVisible)
        {
            {
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
            }

            // Apply scrolling
            enemyBullets[i].positionX -= scrollXThisFrame;
            enemyBullets[i].spriteX -= scrollXThisFrame;
            enemyBullets[i].positionY -= scrollYThisFrame;
            enemyBullets[i].spriteY -= scrollYThisFrame;

            // Destory bullets off screen
            if(enemyBullets[i].spriteY < 8
            || enemyBullets[i].spriteX < 8
            || enemyBullets[i].spriteY > SCREEN_HEIGHT - 8
            || enemyBullets[i].spriteX > SCREEN_WIDTH - 8)
            {
                enemyBullets[i].isVisible = 0;
            }

             // Check collisions to players
             for(char j = 0; j < PLAYER_COUNT; ++j)
            {
                if(playersSprites[j].isVisible)
                {
                    if(spriteToSpriteCollision(&playersSprites[j], &enemyBullets[i]))
                    {
                        players[j].action = ACTION_STUN;
                        players[j].actionCount = PLAYER_STUN_FRAME_COUNT;
                        enemyBullets[i].isVisible = 0;
                    }
                }
            }
        }
    }
}