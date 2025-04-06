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
void MetatileSteppedOn(unsigned char *metatile);
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
void ShootTurretBullet(char turretIndex);
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

// Roar
void Roar(char i);

// Enemy AI
void InitTurrets(void);
void ScanMapForTurrets(void);
void ActivateTurretAt(unsigned int posX, unsigned int posY, unsigned char mode);
char GetClosestPlayer(unsigned int posX, unsigned int posY);
char GetEnemyFireDirection(unsigned int sourceX, unsigned int sourceY, 
                          unsigned int targetX, unsigned int targetY);
void CheckTurretsInBoundingBox(void);
void ActivateTurretsInSector(char sectorIndex, char sectorX, char sectorY);
void UpdateTurrets(void);
// Helper Functions
int abs_delta(int delta);

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

// Turret Scanning
unsigned char turretScanCounter = 0;

// Gamestate and counters
unsigned char gameState = GAME_STATE_GAME;
unsigned char numFactories = MAX_FACTORY_NUM; // when this reaches 0, game is won
unsigned char menuButtonPressed = 0;

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
    SMS_loadTiles(&start_tiles_bin, 0, start_tiles_bin_size);
    SMS_loadTileMap(0,0,&start_map_bin, start_map_bin_size);
    
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
    SMS_loadTiles(&end_tiles_bin, 0, end_tiles_bin_size);
    SMS_loadTileMap(0,0,&end_map_bin, end_map_bin_size);
    
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
        enemyBullets[i].speed = ENEMY_BULLET_SPEED_DEFAULT;
        enemyBullets[i].direction = DIRECTION_DOWN;
        enemyBullets[i].spriteOneIndex = 12;
        enemyBullets[i].spriteTwoIndex = 0;
    }

    // Reset game progress variables
    numFactories = MAX_FACTORY_NUM;
    playerTwoJoined = 0;
    endCounter = 0;

    // Initialize turrets
    InitTurrets();

    // Init audio & play music
    LoadAndPlayMusic();

    // Set bank for streaming tiles (should use same bank as audio)
    SMS_mapROMBank(player_tiles_bin_bank);

    // Loading complete, start display
    SMS_displayOn();
}

void HandleTitleScreen(void)
{
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
        if (players[i].actionCount == 0) UpdatePlayer(i);
        if (players[i].actionCount != 0) { players[i].actionCount--; UpdateAction(i); }
        UpdatePlayerAnimations(i);
        UpdateBullets(i);
    }

    // Update general functions
    UpdateEnemyBullets();
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
        MetatileSteppedOn(metatile);
    }
}

void MetatileSteppedOn(unsigned char *metatile)
{
    for(char i  = 0; i < MAX_TILE_PAIRS; ++i)
    {
        if (*metatile == walkableTilePairs[0].normalTile)
        {
            *metatile = walkableTilePairs[0].flowerTile;
            GSL_metatileUpdate();
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
            }

            // Player bullet collision to metatiles
            unsigned char *metatile = GSL_metatileLookup(players[i].bullets[j].positionX, players[i].bullets[j].positionY);
            if(*metatile == METATILE_FACTORY || *metatile == METATILE_FACTORY_CLAIMED)
            {
                MetatileFactoryHit(metatile);
                players[i].bullets[j].isVisible = 0;
            }
            else if(*metatile == METATILE_TURRET)
            {
                // TODO: Hit turret!
                players[i].bullets[j].isVisible = 0;
            }
            else if(metatilesMetaLUT[*metatile] & PLAYER_COLLISION_VALUE == 1)
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
    }
}

void MoveEnemyBulletUp(char i)       { enemyBullets[i].positionY -= enemyBullets[i].speed; enemyBullets[i].spriteY -= enemyBullets[i].speed; }
void MoveEnemyBulletDown(char i)     { enemyBullets[i].positionY += enemyBullets[i].speed; enemyBullets[i].spriteY += enemyBullets[i].speed; }
void MoveEnemyBulletLeft(char i)     { enemyBullets[i].positionX -= enemyBullets[i].speed; enemyBullets[i].spriteX -= enemyBullets[i].speed; }
void MoveEnemyBulletRight(char i)    { enemyBullets[i].positionX += enemyBullets[i].speed; enemyBullets[i].spriteX += enemyBullets[i].speed; }


void UpdateEnemyBullets(void) {
    // check active turrets first
    UpdateTurrets();
    
    for(char i = 0; i < ENEMY_BULLET_COUNT; ++i) {
        if(enemyBullets[i].isVisible) {
            // Movement code for bullets
            switch (enemyBullets[i].direction) {
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
            
            // Apply scrolling (unchanged)
            
            // Apply scrolling (unchanged)
            enemyBullets[i].positionX -= scrollXThisFrame;
            enemyBullets[i].spriteX -= scrollXThisFrame;
            enemyBullets[i].positionY -= scrollYThisFrame;
            enemyBullets[i].spriteY -= scrollYThisFrame;
            
            // Check for off-screen (unchanged)
            
            // Check for off-screen (unchanged)
            if(enemyBullets[i].spriteY < 8
            || enemyBullets[i].spriteX < 8
            || enemyBullets[i].spriteY > SCREEN_HEIGHT - 8
            || enemyBullets[i].spriteX > SCREEN_WIDTH - 8) {
                enemyBullets[i].isVisible = 0;
            }
            
            // Check collisions with players (unchanged)
            for(char j = 0; j < PLAYER_COUNT; ++j) {
                if(playersSprites[j].isVisible) {
                    if(spriteToSpriteCollision(&playersSprites[j], &enemyBullets[i])) {
                        LoadAndPlaySFX(SFX_EXPLOSION);
                        players[j].action = ACTION_STUN;
                        players[j].actionCount = PLAYER_STUN_FRAME_COUNT;
                        enemyBullets[i].isVisible = 0;
                    }
                }
            }
        }
    }
}





/// Enemy AI Logic Start

void InitTurrets(void) {
    // Initialize all turrets as inactive
    for(char i = 0; i < MAX_ACTIVE_TURRETS; ++i) {
        turrets[i].isActive = 1;
        turrets[i].isDestroyed = 0;
        turrets[i].shootTimer = 0;
        // Default to random shooting
        turrets[i].fireMode = 0;
    }
    
    // Scan the entire map and populate turret sectors
    ScanMapForTurrets();
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
    
    // Determine the direction based on delta values
    char direction;
    
    // Check if mostly vertical
    if (abs_delta(deltaY) > abs_delta(deltaX)) {
        // Vertical movement dominant
        if (deltaY < 0) {
            // Moving up
            if (deltaX < 0) direction = DIRECTION_UP_LEFT;
            else if (deltaX > 0) direction = DIRECTION_UP_RIGHT;
            else direction = DIRECTION_UP;
        } else {
            // Moving down
            if (deltaX < 0) direction = DIRECTION_DOWN_LEFT;
            else if (deltaX > 0) direction = DIRECTION_DOWN_RIGHT;
            else direction = DIRECTION_DOWN;
        }
    } else {
        // Horizontal movement dominant
        if (deltaX < 0) {
            // Moving left
            if (deltaY < 0) direction = DIRECTION_UP_LEFT;
            else if (deltaY > 0) direction = DIRECTION_DOWN_LEFT;
            else direction = DIRECTION_LEFT;
        } else {
            // Moving right
            if (deltaY < 0) direction = DIRECTION_UP_RIGHT;
            else if (deltaY > 0) direction = DIRECTION_DOWN_RIGHT;
            else direction = DIRECTION_RIGHT;
        }
    }
    
    return direction;
}

// gets absolute value
int abs_delta(int delta) {
    return (delta < 0) ? -delta : delta;
}

// Scan the entire map for turrets and store them in sectors
void ScanMapForTurrets(void) {
    unsigned int mapWidth = GSL_getMapWidthInPixels();
    unsigned int mapHeight = GSL_getMapHeightInPixels();
    
    // Reset sector data
    sectorCount = 0;
    for (unsigned char i = 0; i < MAX_SECTORS; i++) {
        turretSectors[i].count = 0;
    }
    
    // Scan the map in tile-sized (16 pixel) increments to save time
    for (unsigned int y = 0; y < mapHeight; y += 16) {
        for (unsigned int x = 0; x < mapWidth; x += 16) {
            // Check if this tile is a turret
            unsigned char metatile = *GSL_metatileLookup(x, y);
            if (metatile == METATILE_TURRET) {
                // Calculate sector coordinates
                unsigned char sx = x >> 8;  // Divide by 256
                unsigned char sy = y >> 8;
                
                // Calculate relative position within sector
                unsigned char rx = x & 0xFF;  // Modulo 256
                unsigned char ry = y & 0xFF;
                
                // Look for an existing sector or create a new one
                char sectorIndex = -1;
                
                // Check if this sector already exists
                for (unsigned char i = 0; i < sectorCount; i++) {
                    if (turretSectors[i].sector_x == sx && 
                        turretSectors[i].sector_y == sy) {
                        sectorIndex = i;
                        break;
                    }
                }
                
                // If sector doesn't exist, create a new one
                if (sectorIndex == -1) {
                    // Check if we have room for a new sector
                    if (sectorCount >= MAX_SECTORS) {
                        // Too many sectors, skip this turret
                        continue;
                    }
                    
                    sectorIndex = sectorCount;
                    turretSectors[sectorIndex].sector_x = sx;
                    turretSectors[sectorIndex].sector_y = sy;
                    turretSectors[sectorIndex].count = 0;
                    sectorCount++;
                }
                
                // Check if sector has room for another turret
                if (turretSectors[sectorIndex].count >= MAX_TURRETS_PER_SECTOR) {
                    // Sector full, skip this turret
                    continue;
                }
                
                // Add turret to sector
                unsigned char tIndex = turretSectors[sectorIndex].count;
                turretSectors[sectorIndex].turrets[tIndex].rel_x = rx;
                turretSectors[sectorIndex].turrets[tIndex].rel_y = ry;
                turretSectors[sectorIndex].turrets[tIndex].mode = 0; // Default to random
                turretSectors[sectorIndex].count++;
                
                // Also initialize the TurretInfo in the global array
                // Find a free slot
                for (unsigned char t = 0; t < MAX_ACTIVE_TURRETS; t++) {
                    if (!turrets[t].isActive && !turrets[t].isDestroyed) {
                        // Calculate absolute position
                        turrets[t].positionX = (sx << 8) + rx;
                        turrets[t].positionY = (sy << 8) + ry;
                        turrets[t].sector_x = sx;
                        turrets[t].sector_y = sy;
                        turrets[t].rel_x = rx;
                        turrets[t].rel_y = ry;
                        turrets[t].isActive = 0;
                        turrets[t].isDestroyed = 0;
                        turrets[t].shootTimer = 0;
                        turrets[t].fireMode = 0; // Default to random
                        
                        // Alternate between firing modes for variety
                        static unsigned char modeSelector = 0;
                        modeSelector = (modeSelector + 1) & 0x01; // Toggle between 0 and 1
                        turrets[t].fireMode = modeSelector;
                        
                        break;
                    }
                }
            }
        }
    }
}

// Check turrets in bounding box around player 1
void CheckTurretsInBoundingBox(void) {
    // Get player's actual position
    unsigned int playerPosX = playersSprites[PLAYER_ONE].positionX;
    unsigned int playerPosY = playersSprites[PLAYER_ONE].positionY;
    
    // Calculate which sector the player is in
    unsigned char playerSectorX = playerPosX >> 8; // Divide by 256
    unsigned char playerSectorY = playerPosY >> 8;
    
    // Calculate relative position within the sector
    unsigned char playerRelX = playerPosX & 0xFF;
    unsigned char playerRelY = playerPosY & 0xFF;
    
    // First, deactivate all turrets
    for (char i = 0; i < MAX_ACTIVE_TURRETS; i++) {
        if (turrets[i].isActive && !turrets[i].isDestroyed) {
            turrets[i].isActive = 0;
        }
    }
    
    // Calculate which sectors might overlap with the activation box
    signed char leftSectorOffset = (playerRelX < ACTIVATION_BOX_HALF_WIDTH) ? -1 : 0;
    signed char rightSectorOffset = (playerRelX + ACTIVATION_BOX_HALF_WIDTH > 255) ? 1 : 0;
    signed char topSectorOffset = (playerRelY < ACTIVATION_BOX_HALF_HEIGHT) ? -1 : 0;
    signed char bottomSectorOffset = (playerRelY + ACTIVATION_BOX_HALF_HEIGHT > 255) ? 1 : 0;
    
    // Check each potentially overlapping sector
    for (signed char sxOffset = leftSectorOffset; sxOffset <= rightSectorOffset; sxOffset++) {
        for (signed char syOffset = topSectorOffset; syOffset <= bottomSectorOffset; syOffset++) {
            // Calculate the sector to check with bounds checking
            unsigned char checkSectorX = playerSectorX;
            unsigned char checkSectorY = playerSectorY;
            
            // Only apply negative offset if we won't underflow
            if (sxOffset < 0) {
                if (playerSectorX > 0) {
                    checkSectorX = playerSectorX + sxOffset;
                }
            } else {
                checkSectorX = playerSectorX + sxOffset;
            }
            
            if (syOffset < 0) {
                if (playerSectorY > 0) {
                    checkSectorY = playerSectorY + syOffset;
                }
            } else {
                checkSectorY = playerSectorY + syOffset;
            }
            
            // Find this sector in our list
            for (char s = 0; s < sectorCount; s++) {
                if (turretSectors[s].sector_x == checkSectorX && 
                    turretSectors[s].sector_y == checkSectorY) {
                    
                    // Process turrets in this sector
                    ActivateTurretsInSector(s, checkSectorX, checkSectorY);
                    break;
                }
            }
        }
    }
}


// Activates turrets in a sector if they're within the bounding box
void ActivateTurretsInSector(char sectorIndex, char sectorX, char sectorY) {
    // Get player's actual world position
    unsigned int playerPosX = playersSprites[PLAYER_ONE].positionX;
    unsigned int playerPosY = playersSprites[PLAYER_ONE].positionY;
    
    // Calculate absolute bounds of player's activation box
    unsigned int boxLeft = playerPosX - ACTIVATION_BOX_HALF_WIDTH;
    unsigned int boxRight = playerPosX + ACTIVATION_BOX_HALF_WIDTH;
    unsigned int boxTop = playerPosY - ACTIVATION_BOX_HALF_HEIGHT;
    unsigned int boxBottom = playerPosY + ACTIVATION_BOX_HALF_HEIGHT;
    
    // Check each turret in the sector
    for (char t = 0; t < turretSectors[sectorIndex].count; t++) {
        // Calculate absolute position of this turret
        unsigned int turretPosX = (sectorX << 8) | turretSectors[sectorIndex].turrets[t].rel_x;
        unsigned int turretPosY = (sectorY << 8) | turretSectors[sectorIndex].turrets[t].rel_y;
        
        // Simple AABB check - is turret position inside activation box?
        if (turretPosX >= boxLeft && turretPosX <= boxRight &&
            turretPosY >= boxTop && turretPosY <= boxBottom) {
            
            // Find this turret in global array and activate it
            for (char i = 0; i < MAX_ACTIVE_TURRETS; i++) {
                if (!turrets[i].isDestroyed && 
                    turrets[i].sector_x == sectorX && 
                    turrets[i].sector_y == sectorY &&
                    turrets[i].rel_x == turretSectors[sectorIndex].turrets[t].rel_x && 
                    turrets[i].rel_y == turretSectors[sectorIndex].turrets[t].rel_y) {
                    
                    turrets[i].isActive = 1;
                    break;
                }
            }
        }
    }
}

// Update turrets - called each frame
void UpdateTurrets(void) {
    // Update which turrets should be active
    CheckTurretsInBoundingBox();
    
    // Process active turrets
    for (char i = 0; i < MAX_ACTIVE_TURRETS; i++) {
        if (turrets[i].isActive && !turrets[i].isDestroyed) {
            // Update shoot timer
            turrets[i].shootTimer++;
            
            // Check if it's time to shoot
            if (turrets[i].shootTimer >= TURRET_SHOOT_RATE) {
                turrets[i].shootTimer = 0;
                
                // Shoot bullet
                ShootTurretBullet(i);
            }
        }
    }
}

// Makes a turret shoot a bullet (8-bit optimized version)
void ShootTurretBullet(char turretIndex) {
    // Find an available enemy bullet
    for (char i = 0; i < ENEMY_BULLET_COUNT; i++) {
        if (!enemyBullets[i].isVisible) {
            // Calculate absolute world position of turret
            unsigned int turretPosX = (turrets[turretIndex].sector_x << 8) | turrets[turretIndex].rel_x;
            unsigned int turretPosY = (turrets[turretIndex].sector_y << 8) | turrets[turretIndex].rel_y;
            
            // Set bullet world position
            enemyBullets[i].positionX = turretPosX;
            enemyBullets[i].positionY = turretPosY;
            
            // Convert world to screen coordinates
            // These values are centered on the player's view
            enemyBullets[i].spriteX = (unsigned char)((turretPosX - scrollXTotal + PLAYER_SPRITE_START_X) & 0xFF);
            enemyBullets[i].spriteY = (unsigned char)((turretPosY - scrollYTotal + PLAYER_SPRITE_START_Y) & 0xFF);
            
            // If off-screen, don't even bother shooting
            if (enemyBullets[i].spriteX < 8 || enemyBullets[i].spriteX > (SCREEN_WIDTH - 8) ||
                enemyBullets[i].spriteY < 8 || enemyBullets[i].spriteY > (SCREEN_HEIGHT - 8)) {
                return;
            }
            
            enemyBullets[i].isVisible = 1;
            enemyBullets[i].speed = ENEMY_BULLET_SPEED_DEFAULT;
            
            // Determine direction - either random or target player
            if (turrets[turretIndex].fireMode == 0) {
                // Random direction - simplified approach 
                static unsigned char pseudoRandom = 0;
                pseudoRandom = (pseudoRandom + 1) & 0x07; // Quick mod 8
                enemyBullets[i].direction = pseudoRandom + 1; // Convert to 1-8 range
            } else {
                // Target closest player
                char targetPlayer = GetClosestPlayer(turretPosX, turretPosY);
                
                // Get direction based on target position
                unsigned int targetX = playersSprites[targetPlayer].positionX;
                unsigned int targetY = playersSprites[targetPlayer].positionY;
                
                enemyBullets[i].direction = GetEnemyFireDirection(
                    turretPosX, turretPosY,
                    targetX, targetY
                );
            }
            
            // Adjust speed for diagonal directions
            if (enemyBullets[i].direction == DIRECTION_UP_LEFT || 
                enemyBullets[i].direction == DIRECTION_UP_RIGHT || 
                enemyBullets[i].direction == DIRECTION_DOWN_LEFT || 
                enemyBullets[i].direction == DIRECTION_DOWN_RIGHT) {
                enemyBullets[i].speed = ENEMY_BULLET_SPEED_DEFAULT >> 1;
            }
            
            // Play sound effect
            LoadAndPlaySFX(SFX_ENEMY_SHOOT);
            return;
        }
    }
}
/// Enemy AI Logic End