#include "light.h"

void CLight::light_Init()
{
	D3DMATERIAL9 mat;
	ZeroMemory(&mat, sizeof(mat));
	MaterialSetLight(mat);

	ZeroMemory(&this->settinglight_, sizeof(this->settinglight_));
	LightSet(settinglight_);
}

void CLight::MaterialSetLight(D3DMATERIAL9 mat)
{
	LPDIRECT3DDEVICE9 device = GetDevice();
	mat.Diffuse.r = 1.0f;
	mat.Diffuse.g = 1.0f;
	mat.Diffuse.b = 1.0f;
	mat.Diffuse.a = 1.0f;

	mat.Ambient.r = 1.0f;
	mat.Ambient.g = 1.0f;
	mat.Ambient.b = 1.0f;
	mat.Ambient.a = 1.0f;

	device->SetMaterial(&mat);
}

void CLight::LightSet(D3DLIGHT9 light)
{
	LPDIRECT3DDEVICE9 device = GetDevice();
	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DXVECTOR3 vecDirection(0.0f, 0.0f, 0.0f);

	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirection);

	light.Diffuse = { 1.f,1.f,1.f,1.0f };
	light.Ambient = { 1.f,1.f,1.f,1.0f };
	light.Specular = { 1.f, 1.f, 1.f,1.0f };
	//light.Ambient.r = 0.5f;
	//light.Ambient.g = 0.5f;
	//light.Ambient.b = 1.0f;
	//light.Ambient.a = 1.0f;

	device->SetLight(0, &light);
	device->LightEnable(0, true);
	device->SetRenderState(D3DRS_AMBIENT, this->color_);
}