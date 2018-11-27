/***********************************************
 * [main.heap]
 * ���C������
 * GP11A341 24 ���y�t
 ***********************************************/
#ifndef _CORE_H_
#define _CORE_H_

 /***********************************************
  * �C���N���[�h�t�@�C��
  ***********************************************/
#include "main.h"

/***********************************************
 * �}�N����`
 ***********************************************/

#define CL_DEVMODE				FALSE

#define CL_HUD					TRUE
#define CL_FULLSCREEN			FALSE
#define CL_HEIGHT				480
#define CL_WIDTH				640

#define CM_CHEAT				FALSE
#define CM_GODMODE				FALSE	/* GM_CHEAT��TRUE�̂ݗL�� */
#define CM_SHOWBB				FALSE	/* GM_CHEAT��TRUE�̂ݗL�� */
#define CM_SKY					TRUE	/* GM_CHEAT��TRUE�̂ݕύX�ł��� */

#define GM_GRAVITY				800
#define GM_FOG					1000

#define GR_FAR_Z				1024
#define GR_NEAR_Z				1
#define GR_MAXPARTICLES			256
#define GR_VIEWANGLE			60
#define GR_VSYNC				FALSE
#define GR_SHOWFPS				FALSE

#endif // ! _CORE_H_
