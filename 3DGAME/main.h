/***********************************************
 * [main.heap]
 * ���C������
 * GP11A341 24 ���y�t
 ***********************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#define WIN32_LEAN_AND_MEAN             // Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B

#define DIRECTINPUT_VERSION	(0x0800)	/* �x���΍� */

 /***********************************************
  * �C���N���[�h�t�@�C��
  ***********************************************/
#include <windows.h>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

#include "d3dx9.h"
#include "dinput.h"
#include "mmsystem.h"

#include <dsound.h>

/***********************************************
 * �}�N����`
 ***********************************************/

/***********************************************
 * ���C�u�����̃����N
 ***********************************************/

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

#endif // ! _MAIN_H_
