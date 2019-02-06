#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__

#define GM_MAX_PLAYER			2
#define GM_MAX_ELEMENTS			32
#define GM_TERRAIN_SIZE			16
#define GM_TERRIAN_LEVEL		3
#define GM_TERRAIN_MAG			0.5f
#define GM_MAX_HEIGHT			(GM_TERRAIN_SIZE * (1 << GM_TERRIAN_LEVEL) * GM_TERRIAN_LEVEL * GM_TERRAIN_MAG)

#define GR_FAR_Z				1536
#define GR_NEAR_Z				1
#define GR_VIEWANGLE			90

struct MAINGAME;
typedef struct MAINGAME MAINGAME;

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
