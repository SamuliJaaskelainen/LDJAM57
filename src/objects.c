#include "definitions.h"

struct SpriteObject
{
    // Existing fields
    unsigned int positionX;
    unsigned int positionY;
    unsigned char spriteX;
    unsigned char spriteY;
    unsigned char isVisible;
    unsigned char size;
    unsigned char speed;
    unsigned char animationFrameCounter;
    unsigned char currentAnimationFrame;
    unsigned char animationFrameDataCount;
    unsigned int animationFrameData[8];
    unsigned char direction;
    unsigned int spriteOneIndex;
    unsigned int spriteTwoIndex;
    
    // New fields for subpixel movement (enemy bullets only)
    unsigned char subpixelX;        // Fractional part of X position (0-255)
    unsigned char subpixelY;        // Fractional part of Y position (0-255)
    signed char velocityX;          // Integer part of X velocity 
    unsigned char velocityX_frac;   // Fractional part of X velocity
    signed char velocityY;          // Integer part of Y velocity
    unsigned char velocityY_frac;   // Fractional part of Y velocity
};

struct PlayerObject
{
    unsigned int ramDataAddress;
    unsigned char action;
    unsigned char actionCount;
    unsigned char actionFrame;
    unsigned char actionOnePressed; // Action one cannot be hold, this variable is for checking presses
    unsigned char inputVertical;
    unsigned char inputHorizontal;
    struct SpriteObject bullets[PLAYER_BULLET_COUNT];
};

// Stores information for turret activation and shooting calculations
struct TurretSector {
    unsigned char sector_x;    // Sector X coordinate
    unsigned char sector_y;    // Sector Y coordinate
    unsigned char count;       // Number of turrets in this sector
    struct {
        unsigned char rel_x;   // Relative X within sector (0-127)
        unsigned char rel_y;   // Relative Y within sector (0-127)
        unsigned char mode;    // Firing mode
    } turrets[MAX_TURRETS_PER_SECTOR];
};

struct TurretSector turretSectors[MAX_SECTORS];
unsigned char sectorCount = 0;

// Stores information for turret activation and shooting calculations
struct TurretInfo {
    // Absolute position (needed for shooting calculations)
    unsigned int positionX;
    unsigned int positionY;
    
    // State
    unsigned char isActive;         // 1 if currently active (shooting), 0 otherwise
    unsigned char isDestroyed;      // 1 if permanently destroyed
    unsigned char shootTimer;
    unsigned char fireMode;         // 0=random, 1=player targeted
    unsigned char lastDirectionFired; // Tracks the last direction fired (0-255)
}; 

// Array to track active turrets
struct TurretInfo turrets[MAX_ACTIVE_TURRETS];

// const Array for pseudo-random turret fire mode
const unsigned char turretFireModeDistro[10] =
{
    0, 3, 3, 1, 3, 3, 3, 0, 3, 2
};

// struct for tile normal/flower versions
struct TilePair {
    unsigned char normalTile;
    unsigned char flowerTile;
};

// Array of walkable tile pairs - add all tile pairs here
const struct TilePair walkableTilePairs[MAX_TILE_PAIRS_WALKABLE] =
{
    {METATILE_GRASS, METATILE_FLOWERS}, 

    {METATILE_ROAD_01, METATILE_ROAD_01_C},
    {METATILE_ROAD_02, METATILE_ROAD_02_C},
    {METATILE_ROAD_03, METATILE_ROAD_03_C},
    {METATILE_ROAD_04, METATILE_ROAD_04_C},
    {METATILE_ROAD_05, METATILE_ROAD_05_C},
    {METATILE_ROAD_06, METATILE_ROAD_06_C},
    {METATILE_ROAD_07, METATILE_ROAD_07_C},
    {METATILE_ROAD_08, METATILE_ROAD_08_C},
    {METATILE_ROAD_09, METATILE_ROAD_09_C},
    {METATILE_ROAD_10, METATILE_ROAD_10_C},
    {METATILE_ROAD_11, METATILE_ROAD_11_C},

    {METATILE_BARREL_01, METATILE_BARREL_01_C},
    {METATILE_BARREL_02, METATILE_BARREL_02_C},
    {METATILE_BARREL_03, METATILE_BARREL_03_C},
    {METATILE_BARREL_04, METATILE_BARREL_04_C},
    {METATILE_BARREL_05, METATILE_BARREL_05_C},
    {METATILE_BARREL_06, METATILE_BARREL_06_C},
    {METATILE_BARREL_07, METATILE_BARREL_07_C},
    {METATILE_BARREL_08, METATILE_BARREL_08_C},
    {METATILE_BARREL_09, METATILE_BARREL_09_C},

    {METATILE_CAR_01, METATILE_CAR_01_C},
    {METATILE_CAR_02, METATILE_CAR_02_C},
    {METATILE_CAR_03, METATILE_CAR_03_C},

    {METATILE_SILO_01, METATILE_SILO_01_C},
    {METATILE_SILO_02, METATILE_SILO_02_C},
    {METATILE_SILO_03, METATILE_SILO_03_C},

    {METATILE_STUMP_01, METATILE_STUMP_01_C},
    {METATILE_STUMP_02, METATILE_STUMP_02_C}
};

const struct TilePair shootableTilePairs[MAX_TILE_PAIRS_SHOOTABLE] =
{
    {METATILE_TURRET, METATILE_FACTORY_CLAIMED},  
    {METATILE_MINE, METATILE_MINE_CONVERTED},

    {METATILE_HOUSE_01, METATILE_HOUSE_01_C},
    {METATILE_HOUSE_02, METATILE_HOUSE_02_C},
    {METATILE_HOUSE_03, METATILE_HOUSE_03_C},

    {METATILE_WALL_01, METATILE_WALL_01_C},
    {METATILE_WALL_02, METATILE_WALL_02_C},
    {METATILE_WALL_03, METATILE_WALL_03_C},
    {METATILE_WALL_04, METATILE_WALL_04_C},
    {METATILE_WALL_05, METATILE_WALL_05_C},
    {METATILE_WALL_06, METATILE_WALL_06_C},
    {METATILE_WALL_07, METATILE_WALL_07_C},
    {METATILE_WALL_08, METATILE_WALL_08_C},
    {METATILE_WALL_09, METATILE_WALL_09_C},
    {METATILE_WALL_10, METATILE_WALL_10_C},
    {METATILE_WALL_11, METATILE_WALL_11_C},
    {METATILE_WALL_11, METATILE_WALL_12_C},
    {METATILE_WALL_11, METATILE_WALL_13_C},
    {METATILE_WALL_11, METATILE_WALL_14_C},
    {METATILE_WALL_11, METATILE_WALL_15_C},
    {METATILE_WALL_11, METATILE_WALL_16_C},

    {METATILE_FOREST_01, METATILE_FOREST_01_C},
    {METATILE_FOREST_02, METATILE_FOREST_02_C},
    {METATILE_FOREST_03, METATILE_FOREST_03_C},
    {METATILE_FOREST_04, METATILE_FOREST_04_C},
    {METATILE_FOREST_05, METATILE_FOREST_05_C},
    {METATILE_FOREST_06, METATILE_FOREST_06_C},
    {METATILE_FOREST_07, METATILE_FOREST_07_C},
    {METATILE_FOREST_08, METATILE_FOREST_08_C},
    {METATILE_FOREST_09, METATILE_FOREST_09_C},
    {METATILE_FOREST_10, METATILE_FOREST_10_C},
    {METATILE_FOREST_11, METATILE_FOREST_11_C},
    {METATILE_FOREST_11, METATILE_FOREST_12_C},
    {METATILE_FOREST_11, METATILE_FOREST_13_C},
    {METATILE_FOREST_11, METATILE_FOREST_14_C},
    {METATILE_FOREST_11, METATILE_FOREST_15_C},
    {METATILE_FOREST_11, METATILE_FOREST_16_C}
};

// Sprite Animation and Collision Functions Start
void setSpriteAnimation(struct SpriteObject *spriteObject, const unsigned int *data)
{
    for(char i = 0; i < spriteObject->animationFrameDataCount; ++i)
    {
        spriteObject->animationFrameData[i] = data[i];
    }

    if(spriteObject->currentAnimationFrame >= spriteObject->animationFrameDataCount)
    {
        spriteObject->currentAnimationFrame = 0;
    }
}

// Return 0 if no collision
char spriteToSpriteCollision(struct SpriteObject *a, struct SpriteObject *b)
{
   unsigned char leftA = a->spriteX;
   unsigned char rightA = a->spriteX + a->size;
   unsigned char topA = a->spriteY;
   unsigned char bottomA = a->spriteY + a->size;

   unsigned char leftB = b->spriteX;
   unsigned char rightB = b->spriteX + b->size;
   unsigned char topB = b->spriteY;
   unsigned char bottomB = b->spriteY + b->size;

   if (rightA < leftB || leftA > rightB || bottomA < topB || topA > bottomB)  return 0;
   else return 1;
}

// Return 0 if no collision
char spriteToSpriteCollisionInt(struct SpriteObject *a, struct SpriteObject *b)
{
   unsigned int leftA = a->positionX;
   unsigned int rightA = a->positionX + a->size;
   unsigned int topA = a->positionY;
   unsigned int bottomA = a->positionY + a->size;

   unsigned int leftB = b->positionX;
   unsigned int rightB = b->positionX + b->size;
   unsigned int topB = b->positionY;
   unsigned int bottomB = b->positionY + b->size;

   if (rightA < leftB || leftA > rightB || bottomA < topB || topA > bottomB)  return 0;
   else return 1;
}

// Return 0 if no collision
char boxCollisionToPoint(unsigned char aPosX, unsigned char aPosY, unsigned char aSize, unsigned char bPosX, unsigned char bPosY)
{
   unsigned char leftA = aPosX;
   unsigned char rightA = aPosX + aSize;
   unsigned char topA = aPosY;
   unsigned char bottomA = aPosY + aSize;

   if (rightA < bPosX || leftA > bPosX || bottomA < bPosY || topA > bPosY)  return 0;
   else return 1;
}

// Return 0 if no collision
char boxCollisionToPointInt(unsigned int aPosX, unsigned int aPosY, unsigned int aSize, unsigned int bPosX, unsigned int bPosY)
{
   unsigned int leftA = aPosX;
   unsigned int rightA = aPosX + aSize;
   unsigned int topA = aPosY;
   unsigned int bottomA = aPosY + aSize;

   if (rightA < bPosX || leftA > bPosX || bottomA < bPosY || topA > bPosY)  return 0;
   else return 1;
}
// Sprite Animation and Collision Functions End
