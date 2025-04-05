#define PLAYER_BULLET_COUNT 3

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
    unsigned char actionOnePressed; // Action one cannot be hold, this variable is for checking presses
    unsigned char inputVertical;
    unsigned char inputHorizontal;
    struct SpriteObject bullets[PLAYER_BULLET_COUNT];
};

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

   if (rightA <= leftB || leftA >= rightB || bottomA <= topB || topA >= bottomB)  return 0;
   else return 1;
}