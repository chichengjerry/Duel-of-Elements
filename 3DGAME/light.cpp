#include "light.h"
#include "d3d.h"

LIGHT::LIGHT()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9�������0�ǥ��ꥢ����
	ZeroMemory(&light[0], sizeof(D3DLIGHT9));

	// �饤�ȤΥ����פ��O��
	light[0].Type = D3DLIGHT_DIRECTIONAL;

	// ��ɢ��
	light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �h����
	light[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// �饤�Ȥη�����O��
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light[0].Direction, &vecDir);

	// �饤�Ȥ������󥰥ѥ��ץ饤����O��
	pDevice->SetLight(0, &light[0]);

	// �饤�Ȥ��O��
	pDevice->LightEnable(0, TRUE);


	// D3DLIGHT9�������0�ǥ��ꥢ����
	ZeroMemory(&light[1], sizeof(D3DLIGHT9));

	// �饤�ȤΥ����פ��O��
	light[1].Type = D3DLIGHT_DIRECTIONAL;

	// ��ɢ��
	light[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// �h����
	light[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// �饤�Ȥη�����O��
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light[1].Direction, &vecDir);

	// �饤�Ȥ������󥰥ѥ��ץ饤����O��
	pDevice->SetLight(1, &light[1]);

	// �饤�Ȥ��O��
	pDevice->LightEnable(1, TRUE);

	// D3DLIGHT9�������0�ǥ��ꥢ����
	ZeroMemory(&light[2], sizeof(D3DLIGHT9));

	// �饤�ȤΥ����פ��O��
	light[2].Type = D3DLIGHT_DIRECTIONAL;

	// ��ɢ��
	light[2].Diffuse = D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);

	// �h����
	light[2].Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

	// �饤�Ȥη�����O��
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light[2].Direction, &vecDir);

	// �饤�Ȥ������󥰥ѥ��ץ饤����O��
	pDevice->SetLight(2, &light[2]);

	// �饤�Ȥ��O��
	pDevice->LightEnable(2, TRUE);


	// �饤�ƥ��󥰥�`��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

LIGHT::~LIGHT()
{
}
