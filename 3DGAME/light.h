//-----------------------------------------------
// [light.h]
// 光線の設定
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "main.h"
#include "core.h"

typedef struct LIGHT {
	D3DLIGHT9		lightSun;
	D3DLIGHT9		lightMoon;
	BOOL			isDayLight;

	LIGHT();
	~LIGHT();

	void			Update();
	D3DXCOLOR		GetFogColor();
	D3DXCOLOR		GetSkyColor();
	D3DXCOLOR		GetLightColor();
} LIGHT;

#endif // !__LIGHT_H__
