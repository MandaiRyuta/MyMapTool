#pragma once

#include <Windows.h>
#include <time.h>
#include <crtdbg.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

//必要なライブラリファイルのロード
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#ifndef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#endif
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"XAudio2.lib")

namespace windows_rect
{
	constexpr int D3D_SCREEN_WIDTH = 1800;
	constexpr int D3D_SCREEN_HEIGHT = 1300;
	constexpr int SCREEN_WIDTH = 800;
	constexpr int SCREEN_HEIGHT = 600;
}

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

namespace TEXTURERS
{
	const std::string GROUND = "resource/texture/floor.png";
	//	const std::string NUMBER = "resource/texture/NumberSet.png";
}

namespace XMODELFILEPASS
{
	const std::string SKYDOME =  "resource/xmodel/sky_dome_29.x";
}
namespace MATRIX3DMANAGER
{
	constexpr int XMODEL_MAX = 1000;

	typedef enum {
		MODEL_TEST,
		MODEL_MAX,
	}MODEL_LABEL;

	struct Model
	{
		LPD3DXMESH p_mesh_;
		D3DMATERIAL9* p_meshmaterial_;
		LPDIRECT3DTEXTURE9* p_meshtexture_;
		DWORD dw_material_;
		D3DXVECTOR3 position_;
		D3DXVECTOR3 scall_;
		D3DXCOLOR color_;
		float rotatiion_;
		bool isscall_;
		int  index_;
		D3DXMATRIX world_[XMODEL_MAX];
		Model()
		{
			//this->index_ = 0;
			//this->p_mesh_ = NULL;
			//this->p_meshmaterial_ = NULL;
			//this->p_meshtexture_ = NULL;
			//D3DXMatrixIdentity(this->world_);
			//this->dw_material_ = NULL;
			//this->position_ = { 0.f,0.f,0.f };
			//this->scall_ = { 0.f,0.f,0.f };
			//this->rotatiion_ = 0.0f;
			//this->isscall_ = false;
		}
		~Model()
		{

		}
	};
}
LPDIRECT3DDEVICE9 GetDevice(void);