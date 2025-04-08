// All player animations have same amount of frames (4)
const unsigned int playerAnimIdleRight[] = { 0, 128, 128*2, 128*3 };
const unsigned int playerAnimIdleLeft[] = { 128*8, 128*9, 128*10, 128*11 };
const unsigned int playerAnimIdleUp[] = { 128*16, 128*17, 128*18, 128*19 };
const unsigned int playerAnimIdleDown[] = { 128*24, 128*25, 128*26, 128*27 };
const unsigned int playerAnimMoveRight[] = { 128*4, 128*5, 128*6, 128*7 };
const unsigned int playerAnimMoveLeft[] = { 128*12, 128*13, 128*14, 128*15 };
const unsigned int playerAnimMoveUp[] = { 128*20, 128*21, 128*22, 128*23 };
const unsigned int playerAnimMoveDown[] = { 128*28, 128*29, 128*30, 128*31 };


const unsigned int playerAnimStunRight[] = { 128*43, 128*44, 128*43, 128*44 };
const unsigned int playerAnimStunLeft[] = { 128*34, 128*35, 128*34, 128*35 };
const unsigned int playerAnimRoarRight[] = { 128*41, 128*42, 128*41, 128*42 };
const unsigned int playerAnimRoarLeft[] = { 128*32, 128*33, 128*32, 128*33 };

const unsigned int playerAnimShootUp[] = { 128*38, 128*38, 128*38, 128*38 };
const unsigned int playerAnimShootDown[] = { 128*39, 128*39, 128*39, 128*39 };
const unsigned int playerAnimShootLeft[] = { 128*37, 128*37, 128*37, 128*37 };
const unsigned int playerAnimShootRight[] = { 128*36, 128*36, 128*36, 128*36 };

// Metatile lut requires updating every time the map is exported
// PLAYER_COLLISION_VALUE == SOLID
const unsigned char metatilesMetaLUT[] = 
{
	0x00,	//0
	0x00,	//1
	0x01,	//2
	0x00,	//3
	0x00,	//4
	0x00,	//5
	0x00,	//6
	0x00,	//7
	0x00,	//8
	0x00,	//9
	0x01,	//10
	0x01,	//11
	0x00,	//12
	0x00,	//13
	0x00,	//14
	0x00,	//15
	0x01,	//16
	0x01,	//17
	0x01,	//18
	0x01,	//19
	0x00,	//20
	0x00,	//21
	0x01,	//22
	0x00,	//23
	0x01,	//24
	0x00,	//25
	0x00,	//26
	0x00,	//27
	0x01,	//28
	0x01,	//29
	0x00,	//30
	0x00,	//31
	0x01,	//32
	0x01,	//33
	0x01,	//34
	0x00,	//35
	0x01,	//36
	0x01,	//37
	0x00,	//38
	0x00,	//39
	0x00,	//40
	0x01,	//41
	0x00,	//42
	0x00,	//43
	0x00,	//44
	0x01,	//45
	0x01,	//46
	0x00,	//47
	0x00,	//48
	0x00,	//49
	0x00,	//50
	0x00,	//51
	0x00,	//52
	0x00,	//53
	0x00,	//54
	0x00,	//55
	0x01,	//56
	0x01,	//57
	0x01,	//58
	0x01,	//59
	0x00,	//60
	0x01,	//61
	0x00,	//62
	0x00,	//63
	0x01,	//64
	0x01,	//65
	0x01,	//66
	0x00,	//67
	0x00,	//68
	0x00,	//69
	0x00,	//70
	0x00,	//71
	0x01,	//72
	0x00,	//73
	0x01,	//74
	0x00,	//75
	0x01,	//76
	0x00,	//77
	0x00,	//78
	0x00,	//79
	0x01,	//80
	0x00,	//81
	0x01,	//82
	0x00,	//83
	0x01,	//84
	0x01,	//85
	0x00,	//86
	0x00,	//87
	0x00,	//88
	0x00,	//89
	0x00,	//90
	0x00,	//91
	0x01,	//92
	0x01,	//93
	0x00,	//94
	0x00,	//95
	0x00,	//96
	0x00,	//97
	0x00,	//98
	0x01,	//99
	0x00,	//100
	0x01,	//101
	0x01,	//102
	0x00,	//103
	0x00,	//104
	0x00,	//105
	0x00,	//106
	0x00,	//107
	0x01,	//108
	0x00,	//109
	0x00,	//110
	0x00,	//111
	0x01,	//112
	0x00,	//113
	0x00,	//114
	0x00,	//115
	0x00,	//116
	0x00,	//117
	0x01,	//118
	0x00,	//119
	0x01,	//120
	0x00,	//121
	0x01,	//122
	0x01,	//123
	0x00,	//124
	0x00,	//125
	0x01,	//126
	0x00,	//127
	0x00,	//128
	0x00,	//129
	0x00,	//130
	0x00,	//131
	0x00,	//132
	0x01,	//133
	0x00,	//134
	0x00,	//135
	0x00,	//136
	0x00,	//137
	0x01,	//138
	0x00,	//139
	0x00,	//140
	0x00,	//141
	0x00,	//142
	0x00,	//143
	0x01,	//144
	0x01,	//145
	0x00,	//146
	0x01,	//147
	0x01,	//148
	0x01,	//149
	0x00,	//150
	0x00,	//151
	0x01,	//152
	0x00,	//153
	0x00,	//154
	0x00,	//155
	0x01,	//156
	0x01,	//157
	0x00,	//158
	0x00,	//159
	0x01,	//160
	0x00,	//161
	0x01,	//162
	0x01,	//163
	0x00,	//164
	0x01,	//165
	0x00,	//166
	0x00,	//167
	0x01,	//168
	0x00,	//169
	0x00,	//170
	0x01,	//171
	0x00,	//172
	0x00,	//173
	0x00,	//174
	0x00,	//175
	0x00,	//176
	0x01,	//177
	0x01,	//178
	0x01,	//179
	0x01,	//180
	0x01,	//181
	0x00,	//182
	0x00,	//183
	0x01,	//184
	0x00,	//185
	0x00,	//186
	0x00,	//187
	0x00,	//188
	0x00,	//189
	0x00,	//190
	0x00,	//191
	0x01,	//192
	0x00,	//193
	0x01,	//194
	0x01,	//195
	0x00,	//196
	0x00,	//197
	0x00,	//198
	0x00,	//199
	0x01,	//200
	0x00,	//201
	0x01,	//202
	0x00,	//203
	0x00,	//204
	0x00,	//205
	0x01,	//206
	0x00,	//207
	0x00,	//208
	0x00,	//209
	0x00,	//210
	0x00,	//211
	0x01,	//212
	0x00,	//213
	0x00,	//214
	0x00,	//215
	0x00,	//216
	0x00,	//217
	0x01,	//218
	0x00,	//219
	0x01,	//220
	0x01,	//221
	0x00,	//222
	0x00,	//223
	0x00,	//224
	0x00,	//225
	0x01,	//226
	0x01,	//227
	0x01,	//228
	0x00,	//229
	0x00,	//230
	0x00,	//231
	0x00,	//232
	0x00,	//233
	0x01,	//234
	0x00,	//235
	0x00,	//236
	0x00,	//237
	0x00,	//238
	0x00,	//239
	0x01,	//240
	0x00,	//241
	0x00,	//242
	0x01,	//243
	0x01,	//244
	0x01,	//245
	0x00,	//246
	0x00,	//247
	0x00,	//248
	0x01,	//249
	0x01,	//250
	0x00,	//251
	0x01,	//252
	0x01,	//253
	0x00,	//254
	0x00	//255
};