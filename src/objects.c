#include "definitions.h"

struct SpriteObject
{
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
    unsigned char spriteOneIndex;
    unsigned char spriteTwoIndex;
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
    // Absolute position (needed for shooting calculations)
    unsigned int positionX;
    unsigned int positionY;
    
    // Sector information
    unsigned char sector_x;
    unsigned char sector_y;
    unsigned char rel_x;      // Relative position within sector (0-255)
    unsigned char rel_y;      // Relative position within sector (0-255)
    
    // State
    unsigned char isActive;   // 1 if currently active (shooting), 0 otherwise
    unsigned char isDestroyed; // 1 if permanently destroyed
    unsigned char shootTimer;
    unsigned char fireMode;   // 0=random, 1=player targeted
};

// Array to track active turrets
struct TurretInfo turrets[MAX_ACTIVE_TURRETS];

// struct for tile normal/flower versions
struct TilePair {
    unsigned char normalTile;
    unsigned char flowerTile;
};

// Array of walkable tile pairs - add all tile pairs here
const struct TilePair walkableTilePairs[MAX_TILE_PAIRS] = {
    {8, 104},    // Example: Normal ground = 8, Flowered ground = 104
    {56, 112},   // Example: Another tile type
    // Add more tile pairs as needed
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
char boxCollisionToPoint(char aPosX, char aPosY, char aSize, char bPosX, char bPosY)
{
   unsigned int leftA = aPosX;
   unsigned int rightA = aPosX + aSize;
   unsigned int topA = aPosY;
   unsigned int bottomA = aPosY + aSize;

   if (rightA < bPosX || leftA > bPosX || bottomA < bPosY || topA > bPosY)  return 0;
   else return 1;
}
// Sprite Animation and Collision Functions End
