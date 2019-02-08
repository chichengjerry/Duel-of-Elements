#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__

#define GM_MAX_PLAYER			2
#define GM_MAX_ELEMENTS			64
#define GM_TERRAIN_SIZE			32
#define GM_TERRIAN_LEVEL		3
#define GM_TERRAIN_MAG			1.25f
#define GM_MAX_HEIGHT			GM_TERRAIN_MAG * 128

#define GR_FAR_Z				2048
#define GR_VISIBLE_DIST			(GR_FAR_Z * GR_FAR_Z / 4)
#define GR_NEAR_Z				1
#define GR_VIEWANGLE			90

struct MAINGAME;
typedef struct MAINGAME MAINGAME;

struct BULLET;
typedef struct BULLET BULLET;
struct CAMERA;
typedef struct CAMERA CAMERA;
struct ELEMENT;
typedef struct ELEMENT ELEMENT;
enum ELEMENTTYPE : DWORD;
typedef enum ELEMENTTYPE ELEMENTTYPE;
struct GROUND;
typedef struct GROUND GROUND;
struct PLAYER;
typedef struct PLAYER PLAYER;
struct LIGHT;
typedef struct LIGHT LIGHT;
struct SKY;
typedef struct SKY SKY;
struct TERRAIN;
typedef struct TERRAIN TERRAIN;

#endif // !__GAMEMAIN_H__
