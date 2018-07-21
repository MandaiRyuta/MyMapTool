#include "skydome.h"
#include "manager.h"
void CSkydome::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX(this->filename_.c_str(), D3DXMESH_SYSTEMMEM, pDevice, NULL, &pD3DXMtrlBuffer, NULL, &this->skydomemodel_->dw_material_, &this->skydomemodel_->p_mesh_)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました。", this->filename_.c_str(), MB_OK);
		PostQuitMessage(0);
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	this->skydomemodel_->p_meshmaterial_ = new D3DMATERIAL9[this->skydomemodel_->dw_material_];
	this->skydomemodel_->p_meshtexture_ = new LPDIRECT3DTEXTURE9[this->skydomemodel_->dw_material_];

	for (DWORD i = 0; i < this->skydomemodel_->dw_material_; i++)
	{
		this->skydomemodel_->p_meshmaterial_[i] = d3dxMaterials[i].MatD3D;
		this->skydomemodel_->p_meshmaterial_[i].Ambient = this->skydomemodel_->p_meshmaterial_->Diffuse;
		this->skydomemodel_->p_meshtexture_[i] = NULL;

		if (d3dxMaterials[i].pTextureFilename != NULL &&strlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice, d3dxMaterials[i].pTextureFilename, &this->skydomemodel_->p_meshtexture_[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				PostQuitMessage(0);
			}
		}
	}

	pD3DXMtrlBuffer->Release();
	////シェーダーを読み込む
	if (FAILED(D3DXCreateEffectFromFile(pDevice, "Min.fx", NULL, NULL, 0, NULL, &effect_, NULL)))
	{
		MessageBox(NULL, "シェーダーファイル読み込み失敗", NULL, MB_OK);
		PostQuitMessage(0);
	}

	this->technique_ = this->effect_->GetTechniqueByName("TShader");
	this->technique_ = this->effect_->GetParameterByName(NULL, "mWVP");
	this->technique_ = this->effect_->GetParameterByName(NULL, "mWLP");
	this->technique_ = this->effect_->GetParameterByName(NULL, "mWLPB");
	this->technique_ = this->effect_->GetParameterByName(NULL, "vCol");
	this->technique_ = this->effect_->GetParameterByName(NULL, "vLightDir");
	this->shadowmap_ = this->effect_->GetParameterByName(NULL, "ShadowMap");
	this->srcmap_ = this->effect_->GetParameterByName(NULL, "SrcMap");

	constexpr int SHADOW_MAP_SIZE = 512;
	if (FAILED(pDevice->CreateDepthStencilSurface(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, true, &this->pshadowmapz_, NULL)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->pshadowmap_, NULL)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(this->pshadowmap_->GetSurfaceLevel(0, &this->pshadowmapsurf_)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->pedgemap_, NULL)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(this->pedgemap_->GetSurfaceLevel(0, &this->pedgemapsurf_)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->psoftmap_[0], NULL)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(this->psoftmap_[0]->GetSurfaceLevel(0, &this->psoftmapsurf_[0])))
	{
		PostQuitMessage(0);
	}
	if (FAILED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->psoftmap_[1], NULL)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(this->psoftmap_[1]->GetSurfaceLevel(0, &this->psoftmapsurf_[1])))
	{
		PostQuitMessage(0);
	}
	this->effect_->OnResetDevice();
}

void CSkydome::Update()
{
	this->position_ = { 0.f,2.f,0.f };
	D3DXMatrixIdentity(&this->skydomemodel_->world_[this->skydomemodel_->index_]);
	D3DXMatrixTranslation(&this->mtx_translation_, this->position_.x, this->position_.y, this->position_.z);

	if (this->skydomemodel_->isscall_)
	{
		D3DXMatrixScaling(&this->skydomemodel_->world_[this->skydomemodel_->index_], this->skydomemodel_->scall_.x, this->skydomemodel_->scall_.y, this->skydomemodel_->scall_.z);
	}
	/*回転をまだ含めていない*/
	this->skydomemodel_->world_[this->skydomemodel_->index_] = this->mtx_translation_ * this->skydomemodel_->world_[this->skydomemodel_->index_];

	this->skydomemodel_->world_[this->skydomemodel_->index_] = this->skydomemodel_->world_[this->skydomemodel_->index_] * this->mtx_view_ * this->mtx_proj_;
}

void CSkydome::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetTransform(D3DTS_WORLD, &this->skydomemodel_->world_[this->skydomemodel_->index_]);
	// レンダリング
	pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	pDevice->SetRenderState(D3DRS_ZENABLE, true);

	this->effect_->SetTechnique("tecMinimum");
	this->effect_->SetMatrix("mWVP", (D3DXMATRIX*)&this->skydomemodel_->world_[this->skydomemodel_->index_]);
	this->effect_->Begin(NULL, 0);
	this->effect_->BeginPass(0);
	for (DWORD i = 0; i < this->skydomemodel_->dw_material_; i++)
	{
		this->effect_->SetTexture("texDecal", this->skydomemodel_->p_meshtexture_[i]);
		this->skydomemodel_->p_mesh_->DrawSubset(i);
	}
	this->effect_->EndPass();
	this->effect_->End();
}

void CSkydome::Uninit()
{
	if (this->skydomemodel_->p_mesh_ != NULL)
	{
		this->skydomemodel_->p_mesh_->Release();
		this->skydomemodel_->p_mesh_ = NULL;
	}
	delete[] this->skydomemodel_->p_meshtexture_;
	delete[] this->skydomemodel_->p_meshmaterial_;
}

void CSkydome::SetMtxView(D3DXMATRIX view)
{
	this->mtx_view_ = view;
}

void CSkydome::SetMtxProj(D3DXMATRIX proj)
{
	this->mtx_proj_ = proj;
}

CSkydome * CSkydome::Create(MATRIX3DMANAGER::Model * model, std::string filename)
{
	CSkydome* CreateSkydome = new CSkydome(0, model, filename);
	CreateSkydome->Init();
	return CreateSkydome;
}
