
struct SpriteObject
{
    unsigned int positionX;
    unsigned int positionY;
    unsigned int spriteX;
    unsigned int spriteY;
    unsigned char isVisible;
    unsigned char size;
    unsigned char speed;
    unsigned char animationFrameCounter;
    unsigned char currentAnimationFrame;
    unsigned char animationFrameDataCount;
    unsigned int animationFrameData[8];
    unsigned char direction;
    unsigned char bank;
    unsigned int ramDataAddress;
    char spriteOneIndex;
    char spriteTwoIndex;
};

struct PlayerObject
{
    unsigned char action;
    unsigned char actionCount;
    unsigned char actionOnePressed;
    unsigned char actionTwoPressed;
    unsigned char inputVertical;
    unsigned char inputHorizontal;
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