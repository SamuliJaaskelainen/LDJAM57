#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Player Direction Defines
#define DIRECTION_NONE  	    255
#define DIRECTION_UP 			1
#define DIRECTION_DOWN 			2
#define DIRECTION_LEFT  		3
#define DIRECTION_RIGHT  		4
#define DIRECTION_UP_LEFT		5
#define DIRECTION_UP_RIGHT 		6
#define DIRECTION_DOWN_LEFT  	7
#define DIRECTION_DOWN_RIGHT  	8


// Granular direction system
#define DIRECTION_GRANULAR_COUNT    128

// Special flags for directions
#define DIRECTION_FLAG_DIAGONAL     0x80    // Set if direction is diagonal

// In a new file (e.g., direction.c):
// Mapping table from 8-dir to 128-dir
const unsigned char directionTo128[9] = {
    255,   // DIRECTION_NONE maps to 255 (special value)
    32,    // DIRECTION_UP maps to 32 (90 degrees)
    96,    // DIRECTION_DOWN maps to 96 (270 degrees)
    64,    // DIRECTION_LEFT maps to 64 (180 degrees)
    0,     // DIRECTION_RIGHT maps to 0 (0 degrees)
    48,    // DIRECTION_UP_LEFT maps to 48 (135 degrees)
    16,    // DIRECTION_UP_RIGHT maps to 16 (45 degrees)
    80,    // DIRECTION_DOWN_LEFT maps to 80 (225 degrees)
    112,   // DIRECTION_DOWN_RIGHT maps to 112 (315 degrees)
};

// Fixed-point representation for direction vectors
// Format: 8.8 fixed point (1 byte integer, 1 byte fraction)
typedef struct {
    signed int x;    // Fixed-point X component
    signed int y;    // Fixed-point Y component
} DirectionVector;

// Pre-calculated vector components lookup table
// For 8-bit systems, we'll split these into high and low bytes
extern const signed char directionVectorXHigh[DIRECTION_GRANULAR_COUNT];
extern const signed char directionVectorYHigh[DIRECTION_GRANULAR_COUNT];
extern const unsigned char directionVectorXLow[DIRECTION_GRANULAR_COUNT];
extern const unsigned char directionVectorYLow[DIRECTION_GRANULAR_COUNT];

// Direction flags for quick testing of direction properties
extern const unsigned char directionFlags[DIRECTION_GRANULAR_COUNT];

// Function prototypes
unsigned char ConvertOldDirectionToNew(unsigned char oldDir);
void GetDirectionVector(unsigned char direction, DirectionVector* vector);
unsigned char GetDirectionFromVector(signed int dx, signed int dy);

// Pre-calculated sine/cosine values for 128 directions
// Values are in 8.8 fixed-point format
// Full circle is divided into 128 parts (0-127)

// High bytes (integer part) of X vector components
const signed char directionVectorXHigh[DIRECTION_GRANULAR_COUNT] = {
    2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,  // 0-15
    1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 16-31
    0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, // 32-47
    -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, // 48-63
    -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1, -1, // 64-79
    -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80-95
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, // 96-111
    1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2  // 112-127
};

// Low bytes (fractional part) of X vector components
const unsigned char directionVectorXLow[DIRECTION_GRANULAR_COUNT] = {
    0, 25, 50, 75, 99, 124, 148, 172, 195, 218, 240, 250, 10, 25, 40, 56,  // 0-15
    71, 86, 101, 115, 130, 143, 156, 169, 181, 193, 204, 214, 224, 233, 241, 249, // 16-31
    255, 5, 10, 15, 20, 24, 28, 31, 33, 36, 37, 39, 39, 40, 40, 39, // 32-47
    38, 36, 33, 30, 26, 22, 17, 12, 6, 0, 249, 241, 233, 224, 214, 204, // 48-63
    193, 181, 169, 156, 143, 130, 115, 101, 86, 71, 56, 40, 25, 10, 250, 240, // 64-79
    218, 195, 172, 148, 124, 99, 75, 50, 25, 0, 231, 206, 181, 156, 132, 107, // 80-95
    83, 59, 35, 12, 246, 223, 201, 178, 156, 134, 113, 92, 71, 50, 30, 10, // 96-111
    246, 231, 217, 204, 190, 177, 164, 152, 139, 127, 115, 103, 92, 81, 70, 60 // 112-127
};

// High bytes (integer part) of Y vector components
const signed char directionVectorYHigh[DIRECTION_GRANULAR_COUNT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0-15
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16-31
    -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1, -1, // 32-47
    -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, // 48-63
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, // 64-79
    1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, // 80-95
    2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, // 96-111
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0  // 112-127
};

// Low bytes (fractional part) of Y vector components
const unsigned char directionVectorYLow[DIRECTION_GRANULAR_COUNT] = {
    0, 239, 223, 206, 190, 172, 154, 136, 117, 98, 79, 60, 40, 21, 1, 237, // 0-15
    217, 196, 176, 155, 134, 113, 92, 71, 50, 29, 8, 242, 221, 200, 179, 158, // 16-31
    137, 115, 94, 72, 51, 29, 8, 242, 221, 199, 178, 156, 134, 112, 90, 68, // 32-47
    46, 24, 2, 235, 213, 191, 169, 146, 124, 101, 78, 55, 32, 9, 242, 218, // 48-63
    195, 171, 148, 124, 101, 77, 54, 30, 7, 239, 216, 192, 168, 145, 121, 98, // 64-79
    74, 51, 27, 4, 237, 213, 190, 166, 143, 120, 97, 73, 50, 27, 3, 236, // 80-95
    213, 189, 166, 142, 119, 95, 72, 49, 25, 2, 234, 211, 187, 163, 140, 116, // 96-111
    93, 69, 46, 22, 254, 230, 206, 182, 158, 134, 111, 87, 63, 40, 16, 248 // 112-127
};

// Flags for each direction to quickly determine properties
const unsigned char directionFlags[DIRECTION_GRANULAR_COUNT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0-15
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,  // 16-31
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 32-47
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,  // 48-63
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 64-79
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,  // 80-95
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 96-111
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL,
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL   // 112-127
};

// Function to convert from old 8-direction system to new 128-direction system
unsigned char ConvertOldDirectionToNew(unsigned char oldDir) {
    if (oldDir > 8) return 255; // Invalid direction, return "none"
    return directionTo128[oldDir];
}

// Get direction vector components for a given direction
void GetDirectionVector(unsigned char direction, DirectionVector* vector) {
    if (direction >= DIRECTION_GRANULAR_COUNT) {
        // Invalid direction, return zero vector
        vector->x = 0;
        vector->y = 0;
        return;
    }
    
    // Combine high and low bytes to form fixed-point value
    vector->x = (directionVectorXHigh[direction] << 8) | directionVectorXLow[direction];
    vector->y = (directionVectorYHigh[direction] << 8) | directionVectorYLow[direction];
}

// Get direction from a vector (dx, dy)
// This is an approximation using octants for performance
unsigned char GetDirectionFromVector(signed int dx, signed int dy) {
    // Handle special cases
    if (dx == 0 && dy == 0) return 255; // No movement
    
    // Determine octant using signs and relative magnitudes
    unsigned char octant = 0;
    
    // Check quadrant first
    if (dy < 0) {
        if (dx < 0) octant = 3; // Upper-left
        else octant = 0;        // Upper-right
    } else {
        if (dx < 0) octant = 5; // Lower-left
        else octant = 6;        // Lower-right
    }
    
    // Fine-tune within quadrant (divide each quadrant into 2 octants)
    if (octant == 0) {
        // Upper-right quadrant
        if ((-dy) > dx) octant = 1; // More up than right
    } else if (octant == 3) {
        // Upper-left quadrant
        if ((-dy) > (-dx)) octant = 2; // More up than left
    } else if (octant == 5) {
        // Lower-left quadrant
        if (dy > (-dx)) octant = 4; // More down than left
    } else if (octant == 6) {
        // Lower-right quadrant
        if (dy > dx) octant = 7; // More down than right
    }
    
    // Map octant to corresponding direction (middle of the octant section)
    return octant * 16;
}

// Player Action Defines
#define ACTION_STATIONARY 		0
#define ACTION_ONE  			1
#define ACTION_TWO  			2
#define ACTION_MOVE  			3
#define ACTION_STUN  			4

// Metatile Defines (update these when map is exported)
#define METATILE_FACTORY            26
#define METATILE_FACTORY_CLAIMED    50
#define METATILE_TURRET             17
#define METATILE_TURRET_DEAD        3
#define METATILE_GRASS              105
#define METATILE_FLOWERS            234

// Map Data
#define SECTOR_SIZE                 256 // Size in pixels for subdividing the map into sectors
#define MAX_SECTORS                 16 // 1024x1024 map becomes 4x4 sectors
#define MAX_FACTORY_NUM             5
#define MAX_TILE_PAIRS_WALKABLE     2
#define MAX_TILE_PAIRS_SHOOTABLE    2
#define MAX_TURRETS_PER_SECTOR      8

// Player Defines
#define PLAYER_COUNT            2
#define PLAYER_ONE              0
#define PLAYER_TWO              1
#define PLAYER_START_X          904
#define PLAYER_START_Y          928
#define PLAYER_SPRITE_START_X   136
#define PLAYER_SPRITE_START_Y   96
#define PLAYER_ACTION_FRAME_COUNT       15
#define PLAYER_ACTION_INTERACTION_FRAME 14
#define PLAYER_ROAR_FRAME_COUNT         30
#define PLAYER_ROAR_FRAME               25
#define PLAYER_STUN_FRAME_COUNT         20
#define PLAYER_COLLISION_VALUE  	    1
#define PLAYER_ANIMATION_HOLD_DURATION  4
#define PLAYER_BULLET_COUNT             3
#define PLAYER_SPEED_DEFAULT            2
#define PLAYER_SPEED_DIAGONAL           1
#define PLAYER_BULLET_SPEED_DEFAULT     4
#define PLAYER_BULLET_SPEED_DIAGONAL    2
#define UI_X                            183
#define UI_Y                            156

// Screen Space Defines
#define SCREEN_WIDTH            256
#define SCREEN_HEIGHT           192
#define FONT_VRAM_OFFSET        320
#define START_WOLRD_OFFSET_X    768
#define START_WOLRD_OFFSET_Y    832
#define SCREEN_EDGE_Y           96
#define SCREEN_EDGE_X_INNER     120
#define SCREEN_EDGE_X_OUTER     136

// Game State Defines
#define GAME_STATE_TITLE        0
#define GAME_STATE_GAME         1
#define GAME_STATE_END          2

// Enemy Defines
#define ENEMY_BULLET_COUNT          6
#define ENEMY_BULLET_SPEED_DEFAULT  2
#define MAX_ACTIVE_TURRETS          8
#define TURRET_SHOOT_RATE           20
#define TURRET_SCAN_INTERVAL        60  // Scan every 60 frames

// Constants for the player's bounding box
#define ACTIVATION_BOX_HALF_WIDTH  96//128  // Half-width of activation area
#define ACTIVATION_BOX_HALF_HEIGHT 96//96   // Half-height of activation area

// Enemy Type Defines
#define ENEMY_TYPE_BASIC        0
#define ENEMY_TYPE_ADVANCED     1
#define ENEMY_TYPE_RANDOM       2

// SFX
#define SFX_EXPLOSION           0
#define SFX_CONVERT             1
#define SFX_CONVERT_FACTORY     2
#define SFX_ENEMY_SHOOT         3
#define SFX_MAP                 4
#define SFX_ROAR                5
#define SFX_PLAYER_SHOOT        6

// Menus
#define FONT_VRAM_OFFSET 320

#endif /* DEFINITIONS_H */