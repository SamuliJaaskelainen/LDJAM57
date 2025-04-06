#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Player Direction Defines
#define DIRECTION_NONE  	    0
#define DIRECTION_UP 			1
#define DIRECTION_DOWN 			2
#define DIRECTION_LEFT  		3
#define DIRECTION_RIGHT  		4
#define DIRECTION_UP_LEFT		5
#define DIRECTION_UP_RIGHT 		6
#define DIRECTION_DOWN_LEFT  	7
#define DIRECTION_DOWN_RIGHT  	8

// Player Action Defines
#define ACTION_STATIONARY 		0
#define ACTION_ONE  			1
#define ACTION_TWO  			2
#define ACTION_MOVE  			3
#define ACTION_STUN  			4

// Metatile Defines (update these when map is exported)
#define METATILE_TRIGGER_OFF    26
#define METATILE_TRIGGER_ON     50
#define METATILE_TURRET         17
#define METATILE_TURRET_DEAD    3

// Map Data
#define SECTOR_SIZE 256        // Size in pixels for subdividing the map into sectors
#define MAX_SECTORS 16 // 1024x1024 map becomes 4x4 sectors
#define MAX_FACTORY_NUM         5
#define MAX_TILE_PAIRS 2 // currently just for testing factory and turret
#define MAX_TURRETS_PER_SECTOR 8


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
#define PLAYER_BULLET_COUNT     3
#define PLAYER_SPEED_DEFAULT    2
#define PLAYER_SPEED_DIAGONAL   1
#define PLAYER_BULLET_SPEED_DEFAULT 4
#define PLAYER_BULLET_SPEED_DIAGONAL 2

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
#define TURRET_SHOOT_RATE           120
#define TURRET_SCAN_INTERVAL 60  // Scan every 60 frames

// Constants for the player's bounding box
#define ACTIVATION_BOX_HALF_WIDTH  128  // Half-width of activation area
#define ACTIVATION_BOX_HALF_HEIGHT 96   // Half-height of activation area


// Enemy Type Defines
#define ENEMY_TYPE_BASIC        0
#define ENEMY_TYPE_ADVANCED     1
#define ENEMY_TYPE_RANDOM       2

// SFX
#define SFX_EXPLOSION           0

#endif /* DEFINITIONS_H */