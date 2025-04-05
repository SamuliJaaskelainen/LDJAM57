#include "..\lib\SMSlib.h"
#include "..\lib\PSGlib.h"
#include "..\lib\GSLib.h"

#include "..\banks\bank2.h"

#include "resources.c"
#include "objects.c"

void UpdatePlayer(char i);
void MovePlayerNoCollision(char i);
void MovePlayer(char i);
void MoveUp(char i);
void MoveDown(char i);
void MoveLeft(char i);
void MoveRight(char i);
void UpdateAction(char i);
void MetatileInteraction(unsigned char * metatile);
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
char IsPlayerUpBlocked(char i);
char IsPlayerDownBlocked(char i);
char IsPlayerLeftBlocked(char i);
char IsPlayerRightBlocked(char i);
char IsPlayerUpLeftBlocked(char i);
char IsPlayerUpRightBlocked(char i);
char IsPlayerDownLeftBlocked(char i);
char IsPlayerDownRightBlocked(char i);
void RenderSprites(void);
void RenderSpritesUnsafe(void);

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

// Metatile ids require updating every time the map is exported
#define METATILE_TRIGGER_OFF 8
#define METATILE_TRIGGER_ON 104

#define PLAYER_COUNT            2
#define PLAYER_START_X          904
#define PLAYER_START_Y          928
#define PLAYER_SPRITE_START_X   136
#define PLAYER_SPRITE_START_Y   96
#define PLAYER_ACTION_FRAME_COUNT       9
#define PLAYER_ACTION_INTERACTION_FRAME 5
#define PLAYER_COLLISION_VALUE  	    1

#define PLAYER_ANIMATION_HOLD_DURATION  4
#define PLAYER_ANIMATION_FRAME_COUNT  	4

#define FONT_VRAM_OFFSET 320
#define START_WOLRD_OFFSET_X 768
#define START_WOLRD_OFFSET_Y 832

const unsigned char PLAYER_SPEED_DEFAULT =  2;
const unsigned char PLAYER_SPEED_DIAGONAL = 1;

const unsigned char SCREEN_EDGE_Y       = 96;
const unsigned char SCREEN_EDGE_X_INNER = 120;
const unsigned char SCREEN_EDGE_X_OUTER = 136;

// Only Player One scrolls
unsigned char scrollXTotal = 0;
unsigned char scrollYTotal = 0;
unsigned char scrollXOffset = 0;
unsigned char scrollYOffset = 0;

unsigned char playerTwoJoined = 0;

unsigned char scrolltable[ugtbatch_scrolltable_bin_size];

struct PlayerObject players[PLAYER_COUNT];
struct SpriteObject playersSprites[PLAYER_COUNT];

// Header
SMS_EMBED_SEGA_ROM_HEADER(999, 0);
SMS_EMBED_SDSC_HEADER(1, 0, 2024, 9, 24, "Samuli", "LDJAM57", "Love");

void main(void)
{
    // Setup VDP
    SMS_displayOff();
    SMS_useFirstHalfTilesforSprites(0);
    SMS_VDPturnOnFeature(VDPFEATURE_HIDEFIRSTCOL);
    SMS_setSpriteMode (SPRITEMODE_TALL);
    SMS_VRAMmemsetW(0, 0, 0);
    SMS_mapROMBank(2);
	SMS_loadBGPalette(&ugtbatch_palette_bin);
	SMS_loadSpritePalette(&spritepalette_pal_bin);
    SMS_loadTiles(&ugtbatch_tiles_bin, 0, ugtbatch_tiles_bin_size);
    SMS_displayOn();

    // Setup scrolltable
    for (int i = 0; i < ugtbatch_scrolltable_bin_size; i++) scrolltable[i] = *(ugtbatch_scrolltable_bin + i);

    // Setup GSL
    GSL_initializeMap(&scrolltable, &ugtbatch_metatiles_bin);
    GSL_positionWindow(START_WOLRD_OFFSET_X, START_WOLRD_OFFSET_Y);
    scrollXTotal = START_WOLRD_OFFSET_X;
    scrollYTotal = START_WOLRD_OFFSET_Y;
    GSL_refreshVDP();

    // Init audio & play music
    PSGPlay(&soundtest_psg);

    // Init player variables
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
        playersSprites[i].bank = 2;
        playersSprites[i].ramDataAddress = i == 0 ? 8192 : 8320;
        playersSprites[i].spriteOneIndex = (i << 2);
        playersSprites[i].spriteTwoIndex = 2 + (i << 2);
        players[i].action = ACTION_STATIONARY;
        players[i].actionCount = 0;
        players[i].actionOnePressed = 0;
        players[i].actionTwoPressed = 0;
        players[i].inputVertical = DIRECTION_NONE;
        players[i].inputHorizontal = DIRECTION_NONE;
    }

    while(1)
    {
        // Player actions take n steps, 0 means new actions can be started
        for(char i = 0; i < PLAYER_COUNT; ++i)
        {
            if (players[i].actionCount == 0) UpdatePlayer(i);
            if (players[i].actionCount != 0) { players[i].actionCount--; UpdateAction(i); }
        }

        // Render (be extra careful if reordering)
        GSL_scroll(scrollXOffset, scrollYOffset);
        SMS_waitForVBlank();
        UNSAFE_SMS_copySpritestoSAT();
        SMS_initSprites();
        RenderSpritesUnsafe();
        RenderSprites();
        GSL_VBlank();

        // Play audio
        PSGFrame();
    }
}

void RenderSprites(void)
{
    for(char i = 0; i < PLAYER_COUNT; ++i)
    {
        if(!playersSprites[i].isVisible) continue;

        // Add player sprites, tall mode is in use
        SMS_addSprite(playersSprites[i].spriteX + 248, playersSprites[i].spriteY + 248, playersSprites[i].spriteOneIndex);
        SMS_addSprite(playersSprites[i].spriteX, playersSprites[i].spriteY + 248, playersSprites[i].spriteTwoIndex);

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
                else setSpriteAnimation(&playersSprites[i], playerAnimIdleUp);
            break;

            case DIRECTION_DOWN:
                if(players[i].action == ACTION_MOVE) setSpriteAnimation(&playersSprites[i], playerAnimMoveDown);
                else if(players[i].action == ACTION_ONE) setSpriteAnimation(&playersSprites[i], playerAnimOne);
                else if(players[i].action == ACTION_TWO) setSpriteAnimation(&playersSprites[i], playerAnimTwo);
                else setSpriteAnimation(&playersSprites[i], playerAnimIdleDown);
            break;

            case DIRECTION_LEFT:
            case DIRECTION_UP_LEFT:
            case DIRECTION_DOWN_LEFT:
                if(players[i].action == ACTION_MOVE) setSpriteAnimation(&playersSprites[i], playerAnimMoveLeft);
                else if(players[i].action == ACTION_ONE) setSpriteAnimation(&playersSprites[i], playerAnimOne);
                else if(players[i].action == ACTION_TWO) setSpriteAnimation(&playersSprites[i], playerAnimTwo);
                else setSpriteAnimation(&playersSprites[i], playerAnimIdleLeft);
            break;

            case DIRECTION_RIGHT:
            case DIRECTION_UP_RIGHT:
            case DIRECTION_DOWN_RIGHT:
                if(players[i].action == ACTION_MOVE) setSpriteAnimation(&playersSprites[i], playerAnimMoveRight);
                else if(players[i].action == ACTION_ONE) setSpriteAnimation(&playersSprites[i], playerAnimOne);
                else if(players[i].action == ACTION_TWO) setSpriteAnimation(&playersSprites[i], playerAnimTwo);
                else setSpriteAnimation(&playersSprites[i], playerAnimIdleRight);
            break;
        }
    }
}

void RenderSpritesUnsafe(void)
{
    for(char i = 0; i< PLAYER_COUNT; ++i)
    {
        if(!playersSprites[i].isVisible) return;

        SMS_mapROMBank(playersSprites[i].bank);
        UNSAFE_SMS_VRAMmemcpy128(playersSprites[i].ramDataAddress, &player_tiles_bin[playersSprites[i].animationFrameData[playersSprites[i].currentAnimationFrame]]);
    }
}

void UpdatePlayer(char i)
{
	// Reset movement values every frame
	players[i].action = ACTION_STATIONARY;
    if(i == 0)
    {
        scrollXOffset = 0;
        scrollYOffset = 0;
    }
    players[i].inputVertical = DIRECTION_NONE;
    players[i].inputHorizontal = DIRECTION_NONE;

    // Read gamepad
    unsigned int ks = SMS_getKeysStatus();

    // TSpawn second player if they have not joined
    if(i == 1 && !playerTwoJoined)
    {
        if ((ks & PORT_B_KEY_1) || (ks & PORT_B_KEY_2))
        {
            playerTwoJoined = 1;
            playersSprites[1].isVisible = 1;
            playersSprites[1].positionX = playersSprites[0].positionX;
            playersSprites[1].positionY = playersSprites[0].positionY;
        }
    }

    if(i == 0)
    {
        // Reset action buttons
        if (!(ks & PORT_A_KEY_1)) players[i].actionOnePressed = 0;
        if (!(ks & PORT_A_KEY_2)) players[i].actionTwoPressed = 0;

        // Start actions
        if (players[i].actionOnePressed == 0 && (ks & PORT_A_KEY_1))
        {
            players[i].actionOnePressed = 1;
            players[i].action = ACTION_ONE;
            players[i].actionCount = PLAYER_ACTION_FRAME_COUNT;
            return;
        }
        else if (players[i].actionTwoPressed == 0 && (ks & PORT_A_KEY_2))
        {
            players[i].actionTwoPressed = 1;
            players[i].action = ACTION_TWO;
            players[i].actionCount = PLAYER_ACTION_FRAME_COUNT;
            return;
        }

        // Read movement keys
        if (ks & PORT_A_KEY_UP)
        { 
            players[i].action = ACTION_MOVE;
            players[i].inputVertical = DIRECTION_UP;
        }
        else if (ks & PORT_A_KEY_DOWN)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputVertical = DIRECTION_DOWN;
        }

        if (ks & PORT_A_KEY_LEFT)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputHorizontal= DIRECTION_LEFT;
        }
        else if (ks & PORT_A_KEY_RIGHT)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputHorizontal = DIRECTION_RIGHT;
        }
    }
    else
    {
        // Reset action buttons
        if (!(ks & PORT_B_KEY_1)) players[i].actionOnePressed = 0;
        if (!(ks & PORT_B_KEY_2)) players[i].actionTwoPressed = 0;

        // Start actions
        if (players[i].actionOnePressed == 0 && (ks & PORT_B_KEY_1))
        {
            players[i].actionOnePressed = 1;
            players[i].action = ACTION_ONE;
            players[i].actionCount = PLAYER_ACTION_FRAME_COUNT;
            return;
        }
        else if (players[i].actionTwoPressed == 0 && (ks & PORT_B_KEY_2))
        {
            players[i].actionTwoPressed = 1;
            players[i].action = ACTION_TWO;
            players[i].actionCount = PLAYER_ACTION_FRAME_COUNT;
            return;
        }

        // Read movement keys
        if (ks & PORT_B_KEY_UP)
        { 
            players[i].action = ACTION_MOVE;
            players[i].inputVertical = DIRECTION_UP;
        }
        else if (ks & PORT_B_KEY_DOWN)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputVertical = DIRECTION_DOWN;
        }

        if (ks & PORT_B_KEY_LEFT)
        {
            players[i].action = ACTION_MOVE;
            players[i].inputHorizontal= DIRECTION_LEFT;
        }
        else if (ks & PORT_B_KEY_RIGHT)
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
        for(char i = 0; i < playersSprites[i].speed; ++i) MovePlayer(i);
    }

     // Update total scroll
    if(i == 0)
    {
        scrollXTotal += scrollXOffset;
        scrollYTotal += scrollYOffset;
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
char IsPlayerUpBlocked(char i)          { return ((metatilesMetaLUT[GetTopLeftMetatile(i)] & 1) == PLAYER_COLLISION_VALUE || (metatilesMetaLUT[GetTopRightMetatile(i)] & 1) == PLAYER_COLLISION_VALUE); }
char IsPlayerDownBlocked(char i)        { return (metatilesMetaLUT[GetBottomLeftMetatile(i)] & 1) == PLAYER_COLLISION_VALUE || (metatilesMetaLUT[GetBottomRightMetatile(i)] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerLeftBlocked(char i)        { return (metatilesMetaLUT[GetLowerLeftMetatile(i)] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerRightBlocked(char i)       { return (metatilesMetaLUT[GetLowerRightMetatile(i)] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerUpLeftBlocked(char i)      { return (metatilesMetaLUT[GetLeftUpMetatile(i)] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerUpRightBlocked(char i)     { return (metatilesMetaLUT[GetRightUpMetatile(i)] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerDownLeftBlocked(char i)    { return (metatilesMetaLUT[GetLeftDownMetatile(i)] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerDownRightBlocked(char i)   { return (metatilesMetaLUT[GetRightDownMetatile(i)] & 1) == PLAYER_COLLISION_VALUE; }

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
    if (playersSprites[i].positionY <= SCREEN_EDGE_Y || playersSprites[i].positionY > GSL_getMapHeightInPixels() - SCREEN_EDGE_Y)
    {
        playersSprites[i].spriteY--;
    }
    else if(i == 0)
    {
        scrollYOffset--;
    }
    playersSprites[i].positionY--;
}

void MoveDown(char i)
{
    if (playersSprites[i].positionY < SCREEN_EDGE_Y || playersSprites[i].positionY >= GSL_getMapHeightInPixels() - SCREEN_EDGE_Y)
    {
        playersSprites[i].spriteY++;
    }
    else if(i == 0)
    {
        scrollYOffset++;
    }
    playersSprites[i].positionY++;
}

void MoveLeft(char i)
{
    if (playersSprites[i].positionX <= SCREEN_EDGE_X_OUTER || playersSprites[i].positionX > GSL_getMapWidthInPixels() - SCREEN_EDGE_X_INNER)
    {
        playersSprites[i].spriteX--;
    }
    else if(i == 0)
    {
        scrollXOffset--;
    }
    playersSprites[i].positionX--;
}

void MoveRight(char i)
{
    if (playersSprites[i].positionX < SCREEN_EDGE_X_OUTER || playersSprites[i].positionX >= GSL_getMapWidthInPixels() - SCREEN_EDGE_X_INNER)
    {
        playersSprites[i].spriteX++;
    }
    else if(i == 0)
    {
        scrollXOffset++;
    }
    playersSprites[i].positionX++;
}

void UpdateAction(char i)
{
    if(players[i].actionCount == PLAYER_ACTION_INTERACTION_FRAME)
    {
        if(players[i].action == ACTION_ONE)
        {
            //PSGSFXPlay(&explosion0_psg, 0);
        }
        else if(players[i].action == ACTION_TWO)
        {
            unsigned char *metatile;

            if (playersSprites[i].direction == DIRECTION_UP) metatile = GSL_metatileLookup(playersSprites[i].positionX - 8, playersSprites[i].positionY - 1);
            else if (playersSprites[i].direction == DIRECTION_DOWN) metatile = GSL_metatileLookup(playersSprites[i].positionX - 8, playersSprites[i].positionY + 8);
            else if (playersSprites[i].direction == DIRECTION_LEFT || playersSprites[i].direction == DIRECTION_UP_LEFT || playersSprites[i].direction == DIRECTION_DOWN_LEFT) metatile = GSL_metatileLookup(playersSprites[i].positionX - 9, playersSprites[i].positionY - 8);
            else metatile = GSL_metatileLookup(playersSprites[i].positionX + 8, playersSprites[i].positionY - 8);
            MetatileInteraction(metatile);

            if (playersSprites[i].direction == DIRECTION_UP) metatile = GSL_metatileLookup(playersSprites[i].positionX + 7, playersSprites[i].positionY - 1);
            else if (playersSprites[i].direction == DIRECTION_DOWN) metatile = GSL_metatileLookup(playersSprites[i].positionX + 7, playersSprites[i].positionY + 8);
            else if (playersSprites[i].direction == DIRECTION_LEFT || playersSprites[i].direction == DIRECTION_UP_LEFT || playersSprites[i].direction == DIRECTION_DOWN_LEFT) metatile = GSL_metatileLookup(playersSprites[i].positionX - 9, playersSprites[i].positionY + 7);
            else metatile = GSL_metatileLookup(playersSprites[i].positionX + 8, playersSprites[i].positionY + 7);
            MetatileInteraction(metatile);
        }
    }
}

void MetatileInteraction(unsigned char *metatile)
{
    if (*metatile == METATILE_TRIGGER_OFF)
	{
		*metatile = METATILE_TRIGGER_ON;
		GSL_metatileUpdate();
	}
}