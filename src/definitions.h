#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Game meta info
#define VERSION_MAJOR  	    1
#define VERSION_MINOR  	    2
#define RELEASE_YEAR  	    2025
#define RELEASE_MONTH  	    4
#define RELEASE_DAY  	    12


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
#define DIRECTION_GRID_SIZE 32

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
    1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
 };
 
 // Low bytes (fractional part) of X vector components (cosine)
 const unsigned char directionVectorXLow[DIRECTION_GRANULAR_COUNT] = {
    0, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238,
  236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185,
  181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103,
   97,  92,  86,  80,  74,  68,  62,  56,  49,  43,  37,  31,  25,  18,  12,   6,
    0, 250, 244, 238, 231, 225, 219, 213, 207, 200, 194, 188, 182, 176, 170, 164,
  159, 153, 147, 141, 136, 130, 125, 120, 114, 109, 104,  99,  94,  89,  85,  80,
   75,  71,  67,  63,  59,  55,  51,  47,  44,  40,  37,  34,  31,  28,  25,  22,
   20,  18,  15,  13,  12,  10,   8,   7,   5,   4,   3,   2,   2,   1,   1,   1,
    0,   1,   1,   1,   2,   2,   3,   4,   5,   7,   8,  10,  12,  13,  15,  18,
   20,  22,  25,  28,  31,  34,  37,  40,  44,  47,  51,  55,  59,  63,  67,  71,
   75,  80,  85,  89,  94,  99, 104, 109, 114, 120, 125, 130, 136, 141, 147, 153,
  159, 164, 170, 176, 182, 188, 194, 200, 207, 213, 219, 225, 231, 238, 244, 250,
    0,   6,  12,  18,  25,  31,  37,  43,  49,  56,  62,  68,  74,  80,  86,  92,
   97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176,
  181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234,
  236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255
 };
 
 // High bytes (integer part) of Y vector components (sine)
 const signed char directionVectorYHigh[DIRECTION_GRANULAR_COUNT] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
 };
 
 // Low bytes (fractional part) of Y vector components (sine)
 const unsigned char directionVectorYLow[DIRECTION_GRANULAR_COUNT] = {
    0,   6,  12,  18,  25,  31,  37,  43,  49,  56,  62,  68,  74,  80,  86,  92,
   97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176,
  181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234,
  236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255,
    0, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238,
  236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185,
  181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103,
   97,  92,  86,  80,  74,  68,  62,  56,  49,  43,  37,  31,  25,  18,  12,   6,
    0, 250, 244, 238, 231, 225, 219, 213, 207, 200, 194, 188, 182, 176, 170, 164,
  159, 153, 147, 141, 136, 130, 125, 120, 114, 109, 104,  99,  94,  89,  85,  80,
   75,  71,  67,  63,  59,  55,  51,  47,  44,  40,  37,  34,  31,  28,  25,  22,
   20,  18,  15,  13,  12,  10,   8,   7,   5,   4,   3,   2,   2,   1,   1,   1,
    0,   1,   1,   1,   2,   2,   3,   4,   5,   7,   8,  10,  12,  13,  15,  18,
   20,  22,  25,  28,  31,  34,  37,  40,  44,  47,  51,  55,  59,  63,  67,  71,
   75,  80,  85,  89,  94,  99, 104, 109, 114, 120, 125, 130, 136, 141, 147, 153,
  159, 164, 170, 176, 182, 188, 194, 200, 207, 213, 219, 225, 231, 238, 244, 250
 };

// Flags for each direction to quickly determine properties
const unsigned char directionFlags[DIRECTION_GRANULAR_COUNT] = {
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, 
    DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL, DIRECTION_FLAG_DIAGONAL
};

const unsigned char directionLookup[DIRECTION_GRID_SIZE][DIRECTION_GRID_SIZE] = {
    {160, 161, 162, 164, 165, 167, 169, 171, 173, 175, 177, 179, 182, 184, 186, 189, 192, 194, 197, 199, 201, 204, 206, 208, 210, 212, 214, 216, 218, 219, 221, 222},
    {158, 160, 161, 162, 164, 166, 168, 169, 172, 174, 176, 178, 181, 183, 186, 189, 192, 194, 197, 200, 202, 205, 207, 209, 211, 214, 215, 217, 219, 221, 222, 224},
    {157, 158, 160, 161, 163, 164, 166, 168, 170, 173, 175, 178, 180, 183, 186, 189, 192, 194, 197, 200, 203, 205, 208, 210, 213, 215, 217, 219, 220, 222, 224, 225},
    {155, 157, 158, 160, 161, 163, 165, 167, 169, 171, 174, 177, 179, 182, 185, 188, 192, 195, 198, 201, 204, 206, 209, 212, 214, 216, 218, 220, 222, 224, 225, 226},
    {154, 155, 156, 158, 160, 161, 163, 165, 168, 170, 173, 175, 178, 182, 185, 188, 192, 195, 198, 201, 205, 208, 210, 213, 215, 218, 220, 222, 224, 225, 227, 228},
    {152, 153, 155, 156, 158, 160, 161, 164, 166, 168, 171, 174, 177, 181, 184, 188, 192, 195, 199, 202, 206, 209, 212, 215, 217, 219, 222, 224, 225, 227, 228, 230},
    {150, 151, 153, 154, 156, 158, 160, 162, 164, 167, 169, 173, 176, 180, 183, 187, 192, 196, 200, 203, 207, 210, 214, 216, 219, 221, 224, 225, 227, 229, 230, 232},
    {148, 150, 151, 152, 154, 155, 157, 160, 162, 165, 168, 171, 174, 178, 183, 187, 192, 196, 200, 205, 209, 212, 215, 218, 221, 224, 226, 228, 229, 231, 232, 233},
    {146, 147, 149, 150, 151, 153, 155, 157, 160, 162, 165, 169, 173, 177, 182, 186, 192, 197, 201, 206, 210, 214, 218, 221, 224, 226, 228, 230, 232, 233, 234, 236},
    {144, 145, 146, 148, 149, 151, 152, 154, 157, 160, 163, 166, 170, 175, 180, 186, 192, 197, 203, 208, 213, 217, 220, 224, 226, 229, 231, 232, 234, 235, 237, 238},
    {142, 143, 144, 145, 146, 148, 150, 151, 154, 156, 160, 163, 168, 173, 178, 185, 192, 198, 205, 210, 215, 220, 224, 227, 229, 232, 233, 235, 237, 238, 239, 240},
    {140, 141, 141, 142, 144, 145, 146, 148, 150, 153, 156, 160, 164, 169, 176, 183, 192, 200, 207, 214, 219, 224, 227, 230, 233, 235, 237, 238, 239, 241, 242, 242},
    {137, 138, 139, 140, 141, 142, 143, 145, 146, 149, 151, 155, 160, 165, 173, 182, 192, 201, 210, 218, 224, 228, 232, 234, 237, 238, 240, 241, 242, 243, 244, 245},
    {135, 136, 136, 137, 137, 138, 139, 141, 142, 144, 146, 150, 154, 160, 168, 178, 192, 205, 215, 224, 229, 233, 237, 239, 241, 242, 244, 245, 246, 246, 247, 247},
    {133, 133, 133, 134, 134, 135, 136, 136, 137, 139, 141, 143, 146, 151, 160, 173, 192, 210, 224, 232, 237, 240, 242, 244, 246, 247, 247, 248, 249, 249, 250, 250},
    {130, 130, 130, 131, 131, 131, 132, 132, 133, 133, 134, 136, 137, 141, 146, 160, 192, 224, 237, 242, 246, 247, 249, 250, 250, 251, 251, 252, 252, 252, 253, 253},
    {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {125, 125, 125, 124, 124, 124, 123, 123, 122, 122, 121, 119, 118, 114, 109,  96,  64,  32,  18,  13,   9,   8,   6,   5,   5,   4,   4,   3,   3,   3,   2,   2},
    {122, 122, 122, 121, 121, 120, 119, 119, 118, 116, 114, 112, 109, 104,  96,  82,  64,  45,  32,  23,  18,  15,  13,  11,   9,   8,   8,   7,   6,   6,   5,   5},
    {120, 119, 119, 118, 118, 117, 116, 114, 113, 111, 109, 105, 101,  96,  87,  77,  64,  50,  40,  32,  26,  22,  18,  16,  14,  13,  11,  10,   9,   9,   8,   8},
    {118, 117, 116, 115, 114, 113, 112, 110, 109, 106, 104, 100,  96,  90,  82,  73,  64,  54,  45,  37,  32,  27,  23,  21,  18,  17,  15,  14,  13,  12,  11,  10},
    {115, 114, 114, 113, 111, 110, 109, 107, 105, 102,  99,  96,  91,  86,  79,  72,  64,  55,  48,  41,  36,  32,  28,  25,  22,  20,  18,  17,  16,  14,  13,  13},
    {113, 112, 111, 110, 109, 107, 105, 104, 101,  99,  96,  92,  87,  82,  77,  70,  64,  57,  50,  45,  40,  35,  32,  28,  26,  23,  22,  20,  18,  17,  16,  15},
    {111, 110, 109, 107, 106, 104, 103, 101,  98,  96,  92,  89,  85,  80,  75,  69,  64,  58,  52,  47,  42,  38,  35,  32,  29,  26,  24,  23,  21,  20,  18,  17},
    {109, 108, 106, 105, 104, 102, 100,  98,  96,  93,  90,  86,  82,  78,  73,  69,  64,  58,  54,  49,  45,  41,  37,  34,  32,  29,  27,  25,  23,  22,  21,  19},
    {107, 105, 104, 103, 101, 100,  98,  96,  93,  90,  87,  84,  81,  77,  72,  68,  64,  59,  55,  50,  46,  43,  40,  37,  34,  32,  29,  27,  26,  24,  23,  22},
    {105, 104, 102, 101,  99,  97,  96,  93,  91,  88,  86,  82,  79,  75,  72,  68,  64,  59,  55,  52,  48,  45,  41,  39,  36,  34,  32,  30,  28,  26,  25,  23},
    {103, 102, 100,  99,  97,  96,  94,  91,  89,  87,  84,  81,  78,  74,  71,  67,  64,  60,  56,  53,  49,  46,  43,  40,  38,  36,  33,  32,  30,  28,  27,  25},
    {101, 100,  99,  97,  96,  94,  92,  90,  87,  85,  82,  80,  77,  73,  70,  67,  64,  60,  57,  54,  50,  47,  45,  42,  40,  37,  35,  33,  32,  30,  28,  27},
    {100,  98,  97,  96,  94,  92,  90,  88,  86,  84,  81,  78,  76,  73,  70,  67,  64,  60,  57,  54,  51,  49,  46,  43,  41,  39,  37,  35,  33,  32,  30,  29},
    { 98,  97,  96,  94,  92,  91,  89,  87,  85,  82,  80,  77,  75,  72,  69,  66,  64,  61,  58,  55,  52,  50,  47,  45,  42,  40,  38,  36,  35,  33,  32,  30},
    { 97,  96,  94,  93,  91,  89,  87,  86,  83,  81,  79,  77,  74,  72,  69,  66,  64,  61,  58,  55,  53,  50,  48,  46,  44,  41,  40,  38,  36,  34,  33,  32}
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
#define METATILE_FACTORY            252
#define METATILE_FACTORY_CLAIMED    37
#define METATILE_TURRET             253
#define METATILE_TURRET_DEAD        89
#define METATILE_GRASS              150
#define METATILE_GRASS_EXTRA_1     153
#define METATILE_FLOWERS            205
#define METATILE_MINE               52
#define METATILE_MINE_CONVERTED     158
#define METATILE_MINE_HOLE          217

// Environment metatiles
#define METATILE_ASPHALT_01     105
#define METATILE_ASPHALT_01_C     107
#define METATILE_ASPHALT_02     68
#define METATILE_ASPHALT_02_C     73
#define METATILE_ASPHALT_03     48
#define METATILE_ASPHALT_03_C     137
#define METATILE_ASPHALT_04     174 
#define METATILE_ASPHALT_04_C     121
#define METATILE_ASPHALT_05     67
#define METATILE_ASPHALT_05_C     94

#define METATILE_ROAD_01     27
#define METATILE_ROAD_01_C     213
#define METATILE_ROAD_02     131
#define METATILE_ROAD_02_C     211
#define METATILE_ROAD_03     136
#define METATILE_ROAD_03_C     104
#define METATILE_ROAD_04     235
#define METATILE_ROAD_04_C     69
#define METATILE_ROAD_05     185
#define METATILE_ROAD_05_C     78
#define METATILE_ROAD_06     75
#define METATILE_ROAD_06_C     236
#define METATILE_ROAD_07     155
#define METATILE_ROAD_07_C     5
#define METATILE_ROAD_08     251
#define METATILE_ROAD_08_C     6
#define METATILE_ROAD_09     198
#define METATILE_ROAD_09_C     141
#define METATILE_ROAD_10     44
#define METATILE_ROAD_10_C     248
#define METATILE_ROAD_11     210
#define METATILE_ROAD_11_C     237
#define METATILE_ROAD_EXTRA_08     26
#define METATILE_ROAD_EXTRA_05A     97
#define METATILE_ROAD_EXTRA_05B     139
#define METATILE_ROAD_EXTRA_04     204
#define METATILE_ROAD_EXTRA_11     54

#define METATILE_BARREL_01     62
#define METATILE_BARREL_01_C     154
#define METATILE_BARREL_02     164
#define METATILE_BARREL_02_C     208
#define METATILE_BARREL_03     229
#define METATILE_BARREL_03_C     242
#define METATILE_BARREL_04     60
#define METATILE_BARREL_04_C     51
#define METATILE_BARREL_05     193
#define METATILE_BARREL_05_C     233
#define METATILE_BARREL_06     25
#define METATILE_BARREL_06_C     125
#define METATILE_BARREL_07     219
#define METATILE_BARREL_07_C     182
#define METATILE_BARREL_08     201
#define METATILE_BARREL_08_C     169
#define METATILE_BARREL_09     128
#define METATILE_BARREL_09_C     38

#define METATILE_CAR_01     88
#define METATILE_CAR_01_C     170
#define METATILE_CAR_02     140
#define METATILE_CAR_02_C     86
#define METATILE_CAR_03     13
#define METATILE_CAR_03_C     30

#define METATILE_SILO_01     152
#define METATILE_SILO_01_C     118
#define METATILE_SILO_02     58
#define METATILE_SILO_02_C     126
#define METATILE_SILO_03     74
#define METATILE_SILO_03_C     192
#define METATILE_SILO_04     2
#define METATILE_SILO_04_C     181
#define METATILE_SILO_05     123
#define METATILE_SILO_05_C     59

#define METATILE_STUMP_01     49
#define METATILE_STUMP_01_C     106
#define METATILE_STUMP_02     1
#define METATILE_STUMP_02_C     14

#define METATILE_HOUSE_01     11
#define METATILE_HOUSE_01_C     108
#define METATILE_HOUSE_02     24
#define METATILE_HOUSE_02_C     200
#define METATILE_HOUSE_03     226
#define METATILE_HOUSE_03_C     163

#define METATILE_WALL_01     202
#define METATILE_WALL_01_C     41
#define METATILE_WALL_02     46
#define METATILE_WALL_02_C     34
#define METATILE_WALL_03     76
#define METATILE_WALL_03_C     66
#define METATILE_WALL_04     165
#define METATILE_WALL_04_C     244
#define METATILE_WALL_05     16
#define METATILE_WALL_05_C     206
#define METATILE_WALL_06     188
#define METATILE_WALL_06_C     234
#define METATILE_WALL_07     190
#define METATILE_WALL_07_C     184
#define METATILE_WALL_08     195
#define METATILE_WALL_08_C     168
#define METATILE_WALL_09     19
#define METATILE_WALL_09_C     180
#define METATILE_WALL_10     33
#define METATILE_WALL_10_C     56
#define METATILE_WALL_11     112
#define METATILE_WALL_11_C     93
#define METATILE_WALL_12     171
#define METATILE_WALL_12_C     138
#define METATILE_WALL_13     101
#define METATILE_WALL_13_C     220
#define METATILE_WALL_14     84
#define METATILE_WALL_14_C     57
#define METATILE_WALL_15     17
#define METATILE_WALL_15_C     157
#define METATILE_WALL_16     45
#define METATILE_WALL_16_C     122

#define METATILE_FOREST_01     130
#define METATILE_FOREST_01_C     130
#define METATILE_FOREST_02     12 
#define METATILE_FOREST_02_C     96
#define METATILE_FOREST_03     114
#define METATILE_FOREST_03_C     115
#define METATILE_FOREST_04     9
#define METATILE_FOREST_04_C     65
#define METATILE_FOREST_05     82
#define METATILE_FOREST_05_C     149
#define METATILE_FOREST_06     161
#define METATILE_FOREST_06_C     161
#define METATILE_FOREST_07     99
#define METATILE_FOREST_07_C     221
#define METATILE_FOREST_08     162
#define METATILE_FOREST_08_C     250
#define METATILE_FOREST_09     156
#define METATILE_FOREST_09_C     245
#define METATILE_FOREST_10     228
#define METATILE_FOREST_10_C     85
#define METATILE_FOREST_11     80
#define METATILE_FOREST_11_C     177
#define METATILE_FOREST_12     18
#define METATILE_FOREST_12_C     92
#define METATILE_FOREST_13     10
#define METATILE_FOREST_13_C     227
#define METATILE_FOREST_14     194
#define METATILE_FOREST_14_C     32
#define METATILE_FOREST_15     243
#define METATILE_FOREST_15_C     64
#define METATILE_FOREST_16     160
#define METATILE_FOREST_16_C     145
#define METATILE_FOREST_EXTRA_12     18

// Map Data
#define SECTOR_SIZE                 256 // Size in pixels for subdividing the map into sectors
#define MAX_SECTORS                 16 // 1024x1024 map becomes 4x4 sectors
#define MAX_FACTORY_NUM             18
#define MAX_TILE_PAIRS_WALKABLE     37
#define MAX_TILE_PAIRS_SHOOTABLE    43
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
#define PLAYER_ROAR_FRAME_COUNT         20
#define PLAYER_ROAR_FRAME               19
#define PLAYER_STUN_FRAME_COUNT         20
#define PLAYER_COLLISION_VALUE  	    1
#define PLAYER_ANIMATION_HOLD_DURATION  8
#define PLAYER_BULLET_ANIM_HOLD         8
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
#define ENEMY_BULLET_ANIM_HOLD      4
#define ENEMY_BULLET_COUNT          4
#define ENEMY_BULLET_SPEED_DEFAULT  2
#define MAX_ACTIVE_TURRETS          16
#define TURRET_SHOOT_RATE           50
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
#define SFX_TEXT                7

// Data loading offsets
#define FONT_VRAM_OFFSET 320
#define SPRITE_VRAM_OFFSET 352

const unsigned char *emptyChar = " ";
const unsigned char *story[10][24] = {
    {"M","a","n"," ","h","a","s"," ","u","p","s","e","t"," ","n","a","t","u","r","e"," "," "," "," "},
    {"c","o","v","e","r","i","n","g"," ","t","h","e"," ","l","a","n","d"," ","i","n"," "," "," "," "},
    {"s","t","e","e","l"," ","a","n","d"," ","a","s","p","h","a","l","t","."," "," "," "," "," "," "},
    {"T","h","i","s"," ","h","a","s"," ","t","h","r","o","w","n"," ","o","f","f"," "," "," "," "," "},
    {"t","h","e"," ","n","a","t","u","r","a","l"," ","v","i","b","e","."," "," "," "," "," "," "," "},
    {"S","o"," ","f","r","o","m"," ","t","h","e"," ","d","e","p","t","h","s"," ","r","i","s","e"," "},
    {"N","a","t","u","r","e","'","s"," ","i","m","m","u","n","e"," ","s","y","s","t","e","m"," "," "},
    {"i","n"," ","t","h","e"," ","f","o","r","m"," ","o","f"," ","a"," ","w","a","r","r","i","o","r"},
    {"t","o"," ","m","e","n","d"," ","t","h","e"," ","v","i","b","e","."," "," "," "," "," "," "," "},
    {"F","r","o","m"," ","t","h","e"," ","d","e","p","t","h","s"," ","r","i","s","e",".",".","."," "}
};

#endif /* DEFINITIONS_H */