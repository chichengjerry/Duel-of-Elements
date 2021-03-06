//-----------------------------------------------
// [gamemain.h]
// ゲーム処理宣言情報
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__

#define GM_MAX_PLAYER			2
#define GM_MAX_BULLETS			128
#define GM_MAX_ELEMENTS			64
#define GM_MAX_EXPLOSION		GM_MAX_BULLETS
#define GM_TERRAIN_SIZE			16
#define GM_TERRIAN_LEVEL		4
#define GM_TERRAIN_MAG			0.25f
#define GM_TERRAIN_ENR			0.75f
#define GM_MAX_HEIGHT			GM_TERRAIN_MAG * GM_TERRAIN_SIZE * 256

#define GAME_TIME				180


#define GR_FAR_Z				GM_TERRAIN_SIZE * (2 << (GM_TERRIAN_LEVEL * 2))
#define GR_VISIBLE_DIST			(GR_FAR_Z * GR_FAR_Z / 4)
#define GR_NEAR_Z				1
#define GR_VIEWANGLE			90

#define DECLARATION(t)			struct t; ty

struct MAINGAME;
typedef struct MAINGAME MAINGAME;
struct MAINGAMEUI;
typedef struct MAINGAMEUI MAINGAMEUI;

struct BARRIER;
typedef struct BARRIER BARRIER;
struct BULLET;
typedef struct BULLET BULLET;
struct CAMERA;
typedef struct CAMERA CAMERA;
struct ELEMENT;
typedef struct ELEMENT ELEMENT;
enum ELEMENTTYPE : DWORD;
typedef enum ELEMENTTYPE ELEMENTTYPE;
struct EXPLOSION;
typedef struct EXPLOSION EXPLOSION;
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
