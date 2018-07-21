#pragma once
#include "main.h"
#include "Scene.h"

/*プレイヤー＆インプット操作可能にさせる*/
/*敵＆弾の生成*/

class CSkydome : public CScene {
public:
	CSkydome(int Priority, MATRIX3DMANAGER::Model* model, std::string filename)
		: skydomemodel_(model), filename_(filename), CScene(SCENE_3D_XMODEL) {}
	~CSkydome() {}
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;
	void SetMtxView(D3DXMATRIX view) override;
	void SetMtxProj(D3DXMATRIX proj) override;
	static void SkydomeSetRotation(float rot);
	static CSkydome* Create(MATRIX3DMANAGER::Model* model, std::string filename);
private:
	std::string filename_;
	MATRIX3DMANAGER::Model* skydomemodel_;
	D3DXMATRIX mtx_world_;
	D3DXMATRIX mtx_view_;
	D3DXMATRIX mtx_proj_;
	D3DXMATRIX mtx_translation_;
	D3DXMATRIX mtx_Scale_;
	static D3DXMATRIX mtx_rotation_;
	D3DXVECTOR3 position_;
	D3DXVECTOR3 scale_;
	LPDIRECT3DVERTEXBUFFER9 vertexbuffer_;
	LPDIRECT3DINDEXBUFFER9 indexbuffer_;
	LPDIRECT3DTEXTURE9 texture_;
	float rotation_;

	LPD3DXEFFECT effect_;
	D3DXHANDLE	 technique_;
	D3DXHANDLE   wvp_;
	D3DXHANDLE	 wlp_;
	D3DXHANDLE	 wlpb_;
	D3DXHANDLE	 color_;
	D3DXHANDLE	 direction_;
	D3DXHANDLE	 shadowmap_;
	D3DXHANDLE	 srcmap_;

	LPDIRECT3DSURFACE9 pshadowmapz_;
	LPDIRECT3DTEXTURE9 pshadowmap_;
	LPDIRECT3DSURFACE9 pshadowmapsurf_;
	LPDIRECT3DTEXTURE9 pedgemap_;
	LPDIRECT3DSURFACE9 pedgemapsurf_;
	LPDIRECT3DTEXTURE9 psoftmap_[2];
	LPDIRECT3DSURFACE9 psoftmapsurf_[2];
};