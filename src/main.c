#include "..\lib\SMSlib.h"
#include "..\lib\PSGlib.h"
#include "..\lib\GSLib.h"

#include "..\banks\bank2.h"

#include "resources.c"

void RenderPlayer();
void RenderPlayerUnsafe();
void UpdatePlayer();
void MovePlayerNoCollision();
void MovePlayer();
void MoveUp();
void MoveDown();
void MoveLeft();
void MoveRight();
void StartActionOne();
void StartActionTwo();
void UpdateAction();
void MetatileInteraction(unsigned char * metatile);
char GetTopLeftMetatile();
char GetTopRightMetatile();
char GetBottomLeftMetatile();
char GetBottomRightMetatile();
char GetLowerLeftMetatile();
char GetLowerRightMetatile();
char GetLeftUpMetatile();
char GetLeftDownMetatile();
char GetRightUpMetatile();
char GetRightDownMetatile();
char IsPlayerUpBlocked();
char IsPlayerDownBlocked();
char IsPlayerLeftBlocked();
char IsPlayerRightBlocked();
char IsPlayerUpLeftBlocked();
char IsPlayerUpRightBlocked();
char IsPlayerDownLeftBlocked();
char IsPlayerDownRightBlocked();

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

const unsigned char PLAYER_SPEED_DEFAULT = 2;
const unsigned char PLAYER_SPEED_DIAGONAL = 1;

const char SCREEN_EDGE_Y       = 96;
const char SCREEN_EDGE_X_INNER = 120;
const char SCREEN_EDGE_X_OUTER = 136;

unsigned int playerX = 0;
unsigned int playerY = 0;
unsigned char playerSpriteX = 0;
unsigned char playerSpriteY = 0;

unsigned char actionCount = 0;
unsigned char action = 0;
unsigned char actionOnePressed = 0;
unsigned char actionTwoPressed = 0;

unsigned char scrollXTotal = 0;
unsigned char scrollYTotal = 0;
unsigned char scrollXOffset = 0;
unsigned char scrollYOffset = 0;
unsigned char playerSpeed = 0;
unsigned char playerDirection = 0;
unsigned char playerDirectionVertical = 0;
unsigned char playerDirectionHorizontal = 0;
unsigned char playerAnimCounter = 0;
unsigned char playerAnimFrame = 0;

unsigned char scrolltable[ugtbatch_scrolltable_bin_size];

// Player animations
unsigned int playerAnim[4];

// Header
SMS_EMBED_SEGA_ROM_HEADER(999, 0);
SMS_EMBED_SDSC_HEADER(1, 0, 2024, 9, 24, "Samuli", "SMSTest", "Test");

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
	SMS_loadSpritePalette(spritepalette_pal_bin);
    SMS_loadTiles(&ugtbatch_tiles_bin, 0, ugtbatch_tiles_bin_size);
    //SMS_loadTiles(&font_tiles_bin, FONT_VRAM_OFFSET, font_tiles_bin_size);
    //SMS_configureTextRenderer(FONT_VRAM_OFFSET - 32);
    SMS_displayOn();

    // Setup scrolltable
    for (int i = 0; i < ugtbatch_scrolltable_bin_size; i++) scrolltable[i] = *(ugtbatch_scrolltable_bin + i);

    // Setup GSL
    GSL_initializeMap(&scrolltable, &ugtbatch_metatiles_bin);
    GSL_positionWindow(START_WOLRD_OFFSET_X, START_WOLRD_OFFSET_Y);
    scrollXTotal = START_WOLRD_OFFSET_X;
    scrollYTotal = START_WOLRD_OFFSET_Y;
    GSL_refreshVDP();

    // Play music
    PSGPlay(&music_psg);

    // Init player variables
    action = ACTION_STATIONARY;
    actionCount = 0;
    actionOnePressed = 0;
    actionTwoPressed = 0;
    playerX = PLAYER_START_X;
    playerY = PLAYER_START_Y;
    playerSpriteX = PLAYER_SPRITE_START_X;
    playerSpriteY = PLAYER_SPRITE_START_Y;
    playerDirection = DIRECTION_DOWN;
    playerDirectionVertical = DIRECTION_NONE;
    playerDirectionHorizontal = DIRECTION_NONE;
    for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimIdleUp[i];

    while(1)
    {
        // Player actions take n steps, 0 means new actions can be started
        if (actionCount == 0) UpdatePlayer();
        if (actionCount != 0) { actionCount--; UpdateAction(); }

        // Render (be extra careful if reordering)
        GSL_scroll(scrollXOffset, scrollYOffset);
        SMS_waitForVBlank();
        UNSAFE_SMS_copySpritestoSAT();
        SMS_initSprites();
        RenderPlayerUnsafe();
        RenderPlayer();
        GSL_VBlank();

        // Play audio
        PSGFrame();
    }
}

void RenderPlayer()
{
    // Add player sprites, tall mode is in use
    SMS_addSprite(playerSpriteX + 248, playerSpriteY + 248, 0);
    SMS_addSprite(playerSpriteX, playerSpriteY + 248, 2);

    // Update animation frames
    if(playerAnimCounter > PLAYER_ANIMATION_HOLD_DURATION)
    {
        playerAnimCounter = 0;
        playerAnimFrame++;
        if(playerAnimFrame == PLAYER_ANIMATION_FRAME_COUNT) playerAnimFrame = 0;
    }
    playerAnimCounter++;

    // Update which animation to play
    switch (playerDirection)
    {
        case DIRECTION_UP:
            if(action == ACTION_MOVE) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimMoveUp[i];
            else if(action == ACTION_ONE) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimOne[i];
            else if(action == ACTION_TWO) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimTwo[i];
            else for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimIdleUp[i];
            break;

        case DIRECTION_DOWN:
            if(action == ACTION_MOVE) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimMoveDown[i];
            else if(action == ACTION_ONE) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimOne[i];
            else if(action == ACTION_TWO) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimTwo[i];
            else for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimIdleDown[i];
            break;

        case DIRECTION_LEFT:
        case DIRECTION_UP_LEFT:
        case DIRECTION_DOWN_LEFT:
            if(action == ACTION_MOVE) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimMoveLeft[i];
            else if(action == ACTION_ONE) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimOne[i];
            else if(action == ACTION_TWO) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimTwo[i];
            else for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimIdleLeft[i];
            break;

        case DIRECTION_RIGHT:
        case DIRECTION_UP_RIGHT:
        case DIRECTION_DOWN_RIGHT:
            if(action == ACTION_MOVE) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimMoveRight[i];
            else if(action == ACTION_ONE) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimOne[i];
            else if(action == ACTION_TWO) for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimTwo[i];
            else for(char i = 0; i< 4; ++i) playerAnim[i] = playerAnimIdleRight[i];
            break;
    }
}

void RenderPlayerUnsafe()
{
    UNSAFE_SMS_VRAMmemcpy128(8192, &player_tiles_bin[playerAnim[playerAnimFrame]]);
}

void UpdatePlayer()
{
	// Reset movement values every frame
	action = ACTION_STATIONARY;
	scrollXOffset = 0;
	scrollYOffset = 0;
    playerDirectionVertical = DIRECTION_NONE;
    playerDirectionHorizontal = DIRECTION_NONE;

    // Read gamepad
    unsigned int ks = SMS_getKeysStatus();

    // Reset action buttons
    if (!(ks & PORT_A_KEY_1)) actionOnePressed = 0;
    if (!(ks & PORT_A_KEY_2)) actionTwoPressed = 0;

    // Start actions
    if (actionOnePressed == 0 && (ks & PORT_A_KEY_1))
	{
		StartActionOne();
		return;
	}
	else if (actionTwoPressed == 0 && (ks & PORT_A_KEY_2))
	{
		StartActionTwo();
		return;
	}

    // Read movement keys
	if (ks & PORT_A_KEY_UP)
	{ 
        action = ACTION_MOVE;
        playerDirectionVertical = DIRECTION_UP;
	}
	else if (ks & PORT_A_KEY_DOWN)
	{
        action = ACTION_MOVE;
        playerDirectionVertical = DIRECTION_DOWN;
	}

	if (ks & PORT_A_KEY_LEFT)
	{
        action = ACTION_MOVE;
        playerDirectionHorizontal= DIRECTION_LEFT;
	}
	else if (ks & PORT_A_KEY_RIGHT)
	{
        action = ACTION_MOVE;
        playerDirectionHorizontal = DIRECTION_RIGHT;
	}

    // Update player direction
    if(playerDirectionVertical == DIRECTION_UP)
    {
        if(playerDirectionHorizontal == DIRECTION_LEFT) playerDirection = DIRECTION_UP_LEFT;
        else if(playerDirectionHorizontal == DIRECTION_RIGHT) playerDirection = DIRECTION_UP_RIGHT;
        else playerDirection = DIRECTION_UP;
    }
    else if(playerDirectionVertical == DIRECTION_DOWN)
    {
        if(playerDirectionHorizontal == DIRECTION_LEFT) playerDirection = DIRECTION_DOWN_LEFT;
        else if(playerDirectionHorizontal == DIRECTION_RIGHT) playerDirection = DIRECTION_DOWN_RIGHT;
        else playerDirection = DIRECTION_DOWN;
    }
    else
    {
        if(playerDirectionHorizontal == DIRECTION_LEFT) playerDirection = DIRECTION_LEFT;
        else if(playerDirectionHorizontal == DIRECTION_RIGHT) playerDirection = DIRECTION_RIGHT;
    }

    // Update movement speed
    if(action == ACTION_MOVE)
    {
        switch (playerDirection)
        {
            case DIRECTION_UP_LEFT:
            case DIRECTION_DOWN_LEFT:
            case DIRECTION_UP_RIGHT:
            case DIRECTION_DOWN_RIGHT:
                playerSpeed = PLAYER_SPEED_DIAGONAL;
            break;

            default:
                playerSpeed = PLAYER_SPEED_DEFAULT;
            break;
        }

        // Move player
        for(char i = 0; i < playerSpeed; ++i) MovePlayerNoCollision(); //MovePlayer();
    }

    // Update total scroll
    scrollXTotal += scrollXOffset;
    scrollYTotal += scrollYOffset;
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

char GetTopLeftMetatile() { return *(GSL_metatileLookup(playerX - 8, playerY - 1)); }
char GetTopRightMetatile() { return *(GSL_metatileLookup(playerX + 7, playerY - 1)); }
char GetBottomLeftMetatile() { return *(GSL_metatileLookup(playerX - 8, playerY + 8)); }
char GetBottomRightMetatile() { return *(GSL_metatileLookup(playerX + 7, playerY + 8)); }
char GetLowerLeftMetatile() { return *(GSL_metatileLookup(playerX - 9, playerY + 7)); }
char GetLowerRightMetatile() { return *(GSL_metatileLookup(playerX + 8, playerY + 7)); }
char GetLeftUpMetatile() { return *(GSL_metatileLookup(playerX - 9, playerY - 1)); }
char GetLeftDownMetatile() { return *(GSL_metatileLookup(playerX - 9, playerY + 8)); }
char GetRightUpMetatile() { return *(GSL_metatileLookup(playerX + 8, playerY - 1)); }
char GetRightDownMetatile() { return *(GSL_metatileLookup(playerX + 8, playerY + 8)); }

char IsPlayerUpBlocked() { return ((metatilesMetaLUT[GetTopLeftMetatile()] & 1) == PLAYER_COLLISION_VALUE || (metatilesMetaLUT[GetTopRightMetatile()] & 1) == PLAYER_COLLISION_VALUE); }
char IsPlayerDownBlocked() { return (metatilesMetaLUT[GetBottomLeftMetatile()] & 1) == PLAYER_COLLISION_VALUE || (metatilesMetaLUT[GetBottomRightMetatile()] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerLeftBlocked() { return (metatilesMetaLUT[GetLowerLeftMetatile()] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerRightBlocked() { return (metatilesMetaLUT[GetLowerRightMetatile()] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerUpLeftBlocked() { return (metatilesMetaLUT[GetLeftUpMetatile()] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerUpRightBlocked() { return (metatilesMetaLUT[GetRightUpMetatile()] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerDownLeftBlocked() { return (metatilesMetaLUT[GetLeftDownMetatile()] & 1) == PLAYER_COLLISION_VALUE; }
char IsPlayerDownRightBlocked() { return (metatilesMetaLUT[GetRightDownMetatile()] & 1) == PLAYER_COLLISION_VALUE; }

void MovePlayerNoCollision()
{
    switch (playerDirection)
    {
        case DIRECTION_UP:
            MoveUp();
        break;

        case DIRECTION_DOWN:
            MoveDown();
        break;

        case DIRECTION_LEFT:
            MoveLeft();
        break;

        case DIRECTION_RIGHT:
            MoveRight();
        break;

        case DIRECTION_UP_LEFT:
            MoveUp();
            MoveLeft();
        break;

        case DIRECTION_UP_RIGHT:
            MoveUp();
            MoveRight();
        break;

        case DIRECTION_DOWN_LEFT:
            MoveDown();
            MoveLeft();
        break;

        case DIRECTION_DOWN_RIGHT:
            MoveDown();
            MoveRight();
        break;
    }
}

void MovePlayer()
{
    switch (playerDirection)
    {
        case DIRECTION_UP:
         if (!IsPlayerUpBlocked()) MoveUp();
        break;

        case DIRECTION_DOWN:
            if (!IsPlayerDownBlocked()) MoveDown();
        break;

        case DIRECTION_LEFT:
            if (!IsPlayerLeftBlocked()) MoveLeft();
        break;

        case DIRECTION_RIGHT:
            if (!IsPlayerRightBlocked()) MoveRight();
        break;

        case DIRECTION_UP_LEFT:
            if (IsPlayerUpLeftBlocked())
            {
                if(!IsPlayerUpBlocked()) MoveUp();
                else if(!IsPlayerLeftBlocked()) MoveLeft();
            }
            else
            {
                if(!IsPlayerUpBlocked()) MoveUp();
                if(!IsPlayerLeftBlocked()) MoveLeft();
            }
        break;

        case DIRECTION_UP_RIGHT:
            if (IsPlayerUpRightBlocked())
            {
                if(!IsPlayerUpBlocked()) MoveUp();
                else if(!IsPlayerRightBlocked()) MoveRight();
            }
            else
            {
                if(!IsPlayerUpBlocked()) MoveUp();
                if(!IsPlayerRightBlocked()) MoveRight();
            }
        break;

        case DIRECTION_DOWN_LEFT:
            if (IsPlayerDownLeftBlocked())
            {
                if(!IsPlayerDownBlocked()) MoveDown();
                else if(!IsPlayerLeftBlocked()) MoveLeft();
            }
            else
            {
                if(!IsPlayerDownBlocked()) MoveDown();
                if(!IsPlayerLeftBlocked()) MoveLeft();
            }
        break;

        case DIRECTION_DOWN_RIGHT:
            if (IsPlayerDownRightBlocked())
            {
                if(!IsPlayerDownBlocked()) MoveDown();
                else if(!IsPlayerRightBlocked()) MoveRight();
            }
            else
            {
                if(!IsPlayerDownBlocked()) MoveDown();
                if(!IsPlayerRightBlocked()) MoveRight();
            }
        break;
    }
}

void MoveUp()
{
    if (playerY <= SCREEN_EDGE_Y || playerY > GSL_getMapHeightInPixels() - SCREEN_EDGE_Y)
    {
        playerSpriteY--;
    }
    else
    {
        scrollYOffset--;
    }
    playerY--;
}

void MoveDown()
{
    if (playerY < SCREEN_EDGE_Y || playerY >= GSL_getMapHeightInPixels() - SCREEN_EDGE_Y)
    {
        playerSpriteY++;
    }
    else
    {
        scrollYOffset++;
    }
    playerY++;
}

void MoveLeft()
{
    if (playerX <= SCREEN_EDGE_X_OUTER || playerX > GSL_getMapWidthInPixels() - SCREEN_EDGE_X_INNER)
    {
        playerSpriteX--;
    }
    else
    {
        scrollXOffset--;
    }
    playerX--;
}

void MoveRight()
{
    if (playerX < SCREEN_EDGE_X_OUTER || playerX >= GSL_getMapWidthInPixels() - SCREEN_EDGE_X_INNER)
    {
        playerSpriteX++;
    }
    else
    {
        scrollXOffset++;
    }
    playerX++;
}

void StartActionOne()
{
    actionOnePressed = 1;
	action = ACTION_ONE;
    actionCount = PLAYER_ACTION_FRAME_COUNT;
}

void StartActionTwo()
{
    actionTwoPressed = 1;
	action = ACTION_TWO;
    actionCount = PLAYER_ACTION_FRAME_COUNT;
}

void UpdateAction()
{
    if(action == ACTION_ONE)
    {
        //SMS_setNextTileatXY(playerX/8, playerY/8);
        //SMS_print("AAA!");
    }
    if(action == ACTION_TWO)
    {
        if (actionCount == PLAYER_ACTION_INTERACTION_FRAME)
        {
            unsigned char *metatile;

            if (playerDirection == DIRECTION_UP) metatile = GSL_metatileLookup(playerX - 8, playerY - 1);
            else if (playerDirection == DIRECTION_DOWN) metatile = GSL_metatileLookup(playerX - 8, playerY + 8);
            else if (playerDirection == DIRECTION_LEFT || playerDirection == DIRECTION_UP_LEFT || playerDirection == DIRECTION_DOWN_LEFT) metatile = GSL_metatileLookup(playerX - 9, playerY - 8);
            else metatile = GSL_metatileLookup(playerX + 8, playerY - 8);
            MetatileInteraction(metatile);

            if (playerDirection == DIRECTION_UP) metatile = GSL_metatileLookup(playerX + 7, playerY - 1);
            else if (playerDirection == DIRECTION_DOWN) metatile = GSL_metatileLookup(playerX + 7, playerY + 8);
            else if (playerDirection == DIRECTION_LEFT || playerDirection == DIRECTION_UP_LEFT || playerDirection == DIRECTION_DOWN_LEFT) metatile = GSL_metatileLookup(playerX - 9, playerY + 7);
            else metatile = GSL_metatileLookup(playerX + 8, playerY + 7);
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











