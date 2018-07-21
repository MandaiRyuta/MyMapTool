#pragma once
#include "main.h"

class CLight
{
private:
	D3DLIGHT9 settinglight_;
	D3DLIGHT9    light;
	D3DXCOLOR color_ = { 0.3f, 0.3f, 0.3f, 0.3f };
public:
	CLight()
	{
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;
		light.Direction = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	~CLight() {}
public:
	void light_Init();
	void light_Uninit() {};
	void light_Draw() {};
	void light_Update() {};
	void MaterialSetLight(D3DMATERIAL9 mat);
	void LightSet(D3DLIGHT9 light);

	void setuplight() {
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DMATERIAL9 mtrl;
		//D3DLIGHT9    light;

		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
		mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
		pDevice->SetMaterial(&mtrl);


		light.Type = D3DLIGHT_DIRECTIONAL;

		light.Direction.y -= 0.1f;

		light.Range = 1000.0f;
		pDevice->SetLight(0, &light);
		pDevice->LightEnable(0, TRUE);

		pDevice->SetRenderState(D3DRS_AMBIENT, 0x00444444);
	}
};