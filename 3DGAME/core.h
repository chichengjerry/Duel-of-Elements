/***********************************************
 * [main.h]
 * メイン処理
 * GP11A341 24 張馳騁
 ***********************************************/
#ifndef __CORE_H__
#define __CORE_H__

 /***********************************************
  * インクルードファイル
  ***********************************************/
#include "main.h"

/***********************************************
 * マクロ定義
 ***********************************************/

#define CL_DEVMODE				FALSE

#define CL_HUD					TRUE
#define CL_FULLSCREEN			FALSE
#define CL_HEIGHT				720
#define CL_WIDTH				1280

//	#define CM_CHEAT				FALSE
//	#define CM_GODMODE				FALSE	// GM_CHEATがTRUEのみ有効
//	#define CM_SHOWBB				FALSE	// GM_CHEATがTRUEのみ有効
//	#define CM_SKY					TRUE	// GM_CHEATがTRUEのみ変更できる

#define GM_GRAVITY				800
#define GM_FOG					1000

#define GR_FAR_Z				1024
#define GR_NEAR_Z				1
#define GR_MAXPARTICLES			256
#define GR_VIEWANGLE			60
#define GR_VSYNC				FALSE
#define GR_SHOWFPS				FALSE

struct CAMERA;
struct CAMERAS;
struct MAINGAME;

#endif // !__CORE_H__
