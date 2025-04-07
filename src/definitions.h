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
#define DIRECTION_GRANULAR_COUNT    256

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
void GetDirectionVector(unsigned char direction, DirectionVector* vector);
unsigned char GetDirectionFromVector(signed int dx, signed int dy);

// High bytes (integer part) of X vector components (cosine)
const signed char directionVectorXHigh[DIRECTION_GRANULAR_COUNT] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Low bytes (fractional part) of X vector components (cosine)
const unsigned char directionVectorXLow[DIRECTION_GRANULAR_COUNT] = {
    0, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238, 
    236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185, 
    181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103, 
    97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6, 
    0, 250, 244, 238, 231, 225, 219, 213, 207, 200, 194, 188, 182, 176, 170, 164, 
    159, 153, 147, 141, 136, 130, 125, 120, 114, 109, 104, 99, 94, 89, 85, 80, 
    75, 71, 67, 63, 59, 55, 51, 47, 44, 40, 37, 34, 31, 28, 25, 22, 
    20, 18, 15, 13, 12, 10, 8, 7, 5, 4, 3, 2, 2, 1, 1, 1, 
    0, 1, 1, 1, 2, 2, 3, 4, 5, 7, 8, 10, 12, 13, 15, 18, 
    20, 22, 25, 28, 31, 34, 37, 40, 44, 47, 51, 55, 59, 63, 67, 71, 
    75, 80, 85, 89, 94, 99, 104, 109, 114, 120, 125, 130, 136, 141, 147, 153, 
    159, 164, 170, 176, 182, 188, 194, 200, 207, 213, 219, 225, 231, 238, 244, 250, 
    0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92, 
    97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176, 
    181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234, 
    236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255
};

// High bytes (integer part) of Y vector components (sine)
const signed char directionVectorYHigh[DIRECTION_GRANULAR_COUNT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

// Low bytes (fractional part) of Y vector components (sine)
const unsigned char directionVectorYLow[DIRECTION_GRANULAR_COUNT] = {
    0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92, 
    97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176, 
    181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234, 
    236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255, 
    0, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238, 
    236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185, 
    181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103, 
    97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6, 
    0, 250, 244, 238, 231, 225, 219, 213, 207, 200, 194, 188, 182, 176, 170, 164, 
    159, 153, 147, 141, 136, 130, 125, 120, 114, 109, 104, 99, 94, 89, 85, 80, 
    75, 71, 67, 63, 59, 55, 51, 47, 44, 40, 37, 34, 31, 28, 25, 22, 
    20, 18, 15, 13, 12, 10, 8, 7, 5, 4, 3, 2, 2, 1, 1, 1, 
    0, 1, 1, 1, 2, 2, 3, 4, 5, 7, 8, 10, 12, 13, 15, 18, 
    20, 22, 25, 28, 31, 34, 37, 40, 44, 47, 51, 55, 59, 63, 67, 71, 
    75, 80, 85, 89, 94, 99, 104, 109, 114, 120, 125, 130, 136, 141, 147, 153, 
    159, 164, 170, 176, 182, 188, 194, 200, 207, 213, 219, 225, 231, 238, 244, 250
};

// Flags for each direction to quickly determine properties
const unsigned char directionFlags[DIRECTION_GRANULAR_COUNT] = {
    0, 0, 0, 0, 0, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 0, 0, 0, 0
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
    // For 256 directions, each octant spans 32 values (256/8 = 32)
    return octant << 5; // Multiply by 32
}

// END SUBPIXEL INFO

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

// Environment metatiles
#define METATILE_ROAD_01     1
#define METATILE_ROAD_01_C     1
#define METATILE_ROAD_02     1
#define METATILE_ROAD_02_C     1
#define METATILE_ROAD_03     1
#define METATILE_ROAD_03_C     1
#define METATILE_ROAD_04     1
#define METATILE_ROAD_04_C     1
#define METATILE_ROAD_05     1
#define METATILE_ROAD_05_C     1
#define METATILE_ROAD_06     1
#define METATILE_ROAD_06_C     1
#define METATILE_ROAD_07     1
#define METATILE_ROAD_07_C     1
#define METATILE_ROAD_08     1
#define METATILE_ROAD_08_C     1
#define METATILE_ROAD_09     1
#define METATILE_ROAD_09_C     1
#define METATILE_ROAD_10     1
#define METATILE_ROAD_10_C     1
#define METATILE_ROAD_11     1
#define METATILE_ROAD_11_C     1

#define METATILE_BARREL_01     1
#define METATILE_BARREL_01_C     1
#define METATILE_BARREL_02     1
#define METATILE_BARREL_02_C     1
#define METATILE_BARREL_03     1
#define METATILE_BARREL_03_C     1
#define METATILE_BARREL_04     1
#define METATILE_BARREL_04_C     1
#define METATILE_BARREL_05     1
#define METATILE_BARREL_05_C     1
#define METATILE_BARREL_06     1
#define METATILE_BARREL_06_C     1
#define METATILE_BARREL_07     1
#define METATILE_BARREL_07_C     1
#define METATILE_BARREL_08     1
#define METATILE_BARREL_08_C     1
#define METATILE_BARREL_09     1
#define METATILE_BARREL_09_C     1

#define METATILE_CAR_01     1
#define METATILE_CAR_01_C     1
#define METATILE_CAR_02     1
#define METATILE_CAR_02_C     1
#define METATILE_CAR_03     1
#define METATILE_CAR_03_C     1

#define METATILE_SILO_01     1
#define METATILE_SILO_01_C     1
#define METATILE_SILO_02     1
#define METATILE_SILO_02_C     1
#define METATILE_SILO_03     1
#define METATILE_SILO_03_C     1

#define METATILE_STUMP_01     1
#define METATILE_STUMP_01_C     1
#define METATILE_STUMP_02     1
#define METATILE_STUMP_02_C     1

#define METATILE_HOUSE_01     1
#define METATILE_HOUSE_01_C     1
#define METATILE_HOUSE_02     1
#define METATILE_HOUSE_02_C     1
#define METATILE_HOUSE_03     1
#define METATILE_HOUSE_03_C     1

#define METATILE_WALL_01     1
#define METATILE_WALL_01_C     1
#define METATILE_WALL_02     1
#define METATILE_WALL_02_C     1
#define METATILE_WALL_03     1
#define METATILE_WALL_03_C     1
#define METATILE_WALL_04     1
#define METATILE_WALL_04_C     1
#define METATILE_WALL_05     1
#define METATILE_WALL_05_C     1
#define METATILE_WALL_06     1
#define METATILE_WALL_06_C     1
#define METATILE_WALL_07     1
#define METATILE_WALL_07_C     1
#define METATILE_WALL_08     1
#define METATILE_WALL_08_C     1
#define METATILE_WALL_09     1
#define METATILE_WALL_09_C     1
#define METATILE_WALL_10     1
#define METATILE_WALL_10_C     1
#define METATILE_WALL_11     1
#define METATILE_WALL_11_C     1
#define METATILE_WALL_12     1
#define METATILE_WALL_12_C     1
#define METATILE_WALL_13     1
#define METATILE_WALL_13_C     1
#define METATILE_WALL_14     1
#define METATILE_WALL_14_C     1
#define METATILE_WALL_15     1
#define METATILE_WALL_15_C     1
#define METATILE_WALL_16     1
#define METATILE_WALL_16_C     1

#define METATILE_FOREST_01     1
#define METATILE_FOREST_01_C     1
#define METATILE_FOREST_02     1
#define METATILE_FOREST_02_C     1
#define METATILE_FOREST_03     1
#define METATILE_FOREST_03_C     1
#define METATILE_FOREST_04     1
#define METATILE_FOREST_04_C     1
#define METATILE_FOREST_05     1
#define METATILE_FOREST_05_C     1
#define METATILE_FOREST_06     1
#define METATILE_FOREST_06_C     1
#define METATILE_FOREST_07     1
#define METATILE_FOREST_07_C     1
#define METATILE_FOREST_08     1
#define METATILE_FOREST_08_C     1
#define METATILE_FOREST_09     1
#define METATILE_FOREST_09_C     1
#define METATILE_FOREST_10     1
#define METATILE_FOREST_10_C     1
#define METATILE_FOREST_11     1
#define METATILE_FOREST_11_C     1
#define METATILE_FOREST_12     1
#define METATILE_FOREST_12_C     1
#define METATILE_FOREST_13     1
#define METATILE_FOREST_13_C     1
#define METATILE_FOREST_14     1
#define METATILE_FOREST_14_C     1
#define METATILE_FOREST_15     1
#define METATILE_FOREST_15_C     1
#define METATILE_FOREST_16     1
#define METATILE_FOREST_16_C     1

// Map Data
#define SECTOR_SIZE                 256 // Size in pixels for subdividing the map into sectors
#define MAX_SECTORS                 16 // 1024x1024 map becomes 4x4 sectors
#define MAX_FACTORY_NUM             5
#define MAX_TILE_PAIRS_WALKABLE     30
#define MAX_TILE_PAIRS_SHOOTABLE    37
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
#define GAME_STATE_STORY        1
#define GAME_STATE_GAME         2
#define GAME_STATE_END          3

// Enemy Defines
#define ENEMY_BULLET_COUNT          6
#define ENEMY_BULLET_SPEED_DEFAULT  2
#define MAX_ACTIVE_TURRETS          8
#define TURRET_SHOOT_RATE           30
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