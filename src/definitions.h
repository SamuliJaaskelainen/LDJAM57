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


// Granular direction system
#define DIRECTION_GRANULAR_COUNT    128

// Special flags for directions
#define DIRECTION_FLAG_DIAGONAL     0x80    // Set if direction is diagonal

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

// High bytes (integer part) of X vector components (cosine)
const signed char directionVectorXHigh[DIRECTION_GRANULAR_COUNT] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Low bytes (fractional part) of X vector components (cosine)
const unsigned char directionVectorXLow[DIRECTION_GRANULAR_COUNT] = {
    0, 255, 254, 253, 251, 248, 244, 241, 236, 231, 225, 219, 212, 205, 197, 189, 
    181, 171, 162, 152, 142, 131, 120, 109, 97, 86, 74, 62, 49, 37, 25, 12, 
    0, 244, 231, 219, 207, 194, 182, 170, 159, 147, 136, 125, 114, 104, 94, 85, 
    75, 67, 59, 51, 44, 37, 31, 25, 20, 15, 12, 8, 5, 3, 2, 1, 
    0, 1, 2, 3, 5, 8, 12, 15, 20, 25, 31, 37, 44, 51, 59, 67, 
    75, 85, 94, 104, 114, 125, 136, 147, 159, 170, 182, 194, 207, 219, 231, 244, 
    0, 12, 25, 37, 49, 62, 74, 86, 97, 109, 120, 131, 142, 152, 162, 171, 
    181, 189, 197, 205, 212, 219, 225, 231, 236, 241, 244, 248, 251, 253, 254, 255
};

// High bytes (integer part) of Y vector components (sine)
const signed char directionVectorYHigh[DIRECTION_GRANULAR_COUNT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

// Low bytes (fractional part) of Y vector components (sine)
const unsigned char directionVectorYLow[DIRECTION_GRANULAR_COUNT] = {
    0, 12, 25, 37, 49, 62, 74, 86, 97, 109, 120, 131, 142, 152, 162, 171, 
    181, 189, 197, 205, 212, 219, 225, 231, 236, 241, 244, 248, 251, 253, 254, 255, 
    0, 255, 254, 253, 251, 248, 244, 241, 236, 231, 225, 219, 212, 205, 197, 189, 
    181, 171, 162, 152, 142, 131, 120, 109, 97, 86, 74, 62, 49, 37, 25, 12, 
    0, 244, 231, 219, 207, 194, 182, 170, 159, 147, 136, 125, 114, 104, 94, 85, 
    75, 67, 59, 51, 44, 37, 31, 25, 20, 15, 12, 8, 5, 3, 2, 1, 
    0, 1, 2, 3, 5, 8, 12, 15, 20, 25, 31, 37, 44, 51, 59, 67, 
    75, 85, 94, 104, 114, 125, 136, 147, 159, 170, 182, 194, 207, 219, 231, 244
};

// Flags for each direction to quickly determine properties
const unsigned char directionFlags[DIRECTION_GRANULAR_COUNT] = {
    0, 0, 0, 0, 0, 0, 0, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, // 0-15
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 0, 0, 0, 0, 0, 0, // 16-31
    0, 0, 0, 0, 0, 0, 0, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, // 32-47
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 0, 0, 0, 0, 0, 0, // 48-63
    0, 0, 0, 0, 0, 0, 0, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, // 64-79
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 0, 0, 0, 0, 0, 0, // 80-95
    0, 0, 0, 0, 0, 0, 0, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, // 96-111
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 0, 0, 0, 0, 0, 0   // 112-127
};

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

const unsigned char directionTo256[9] = {
    255,   // DIRECTION_NONE maps to 255 (special value)
    64,    // DIRECTION_UP maps to 64 (90 degrees)
    192,   // DIRECTION_DOWN maps to 192 (270 degrees)
    128,   // DIRECTION_LEFT maps to 128 (180 degrees)
    0,     // DIRECTION_RIGHT maps to 0 (0 degrees)
    96,    // DIRECTION_UP_LEFT maps to 96 (135 degrees)
    32,    // DIRECTION_UP_RIGHT maps to 32 (45 degrees)
    160,   // DIRECTION_DOWN_LEFT maps to 160 (225 degrees)
    224,   // DIRECTION_DOWN_RIGHT maps to 224 (315 degrees)
};



// Function to convert from old 8-direction system to new 128-direction system
unsigned char ConvertOldDirectionToNew(unsigned char oldDir) {
    if (oldDir > 8) return 255; // Invalid direction, return "none"
    return directionTo128[oldDir];
}

void GetDirectionVector(unsigned char direction, DirectionVector* vector) {
    if (direction >= DIRECTION_GRANULAR_COUNT) {
        // Invalid direction, return zero vector
        vector->x = 0;
        vector->y = 0;
        return;
    }
    
    // Create vectors - using 16-bit signed ints with proper two's complement
    vector->x = ((signed int)directionVectorXHigh[direction] << 8) | directionVectorXLow[direction];
    vector->y = ((signed int)directionVectorYHigh[direction] << 8) | directionVectorYLow[direction];
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
#define METATILE_FACTORY            21
#define METATILE_FACTORY_CLAIMED    85
#define METATILE_TURRET             94
#define METATILE_TURRET_DEAD        89
#define METATILE_GRASS              190
#define METATILE_FLOWERS            14
#define METATILE_MINE               92
#define METATILE_MINE_CONVERTED     142


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
#define ACTIVATION_BOX_HALF_WIDTH  128//128  // Half-width of activation area
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

// Data loading offsets
#define FONT_VRAM_OFFSET 320
#define SPRITE_VRAM_OFFSET 352

#endif /* DEFINITIONS_H */