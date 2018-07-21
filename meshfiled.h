#pragma once
#include "main.h"
#include "Scene.h"

#define FVF_MESHFILED3D ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 Normal;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}MESHFILED_VERTEX3D;

struct MESHFILED_INFOMATION {
	int filed_x;
	int filed_y;
	float size_x;
	float size_z;
};

class MeshFiled : public CScene {
public:
	MeshFiled(int Priority, std::string filename, MESHFILED_INFOMATION mesh_info) : filename_(filename), mesh_info_(mesh_info), CScene(Priority) {}
	~MeshFiled() {}
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;
	static MeshFiled* Create(MESHFILED_INFOMATION mesh_info, std::string filename);

	static float GetHeight(D3DXVECTOR3 position);
	D3DXVECTOR3 AddHeight(D3DXVECTOR3 position, D3DXVECTOR3 mousepos);
private:
	std::string filename_;
	LPDIRECT3DVERTEXBUFFER9 meshfiled_vertexbuffer;
	LPDIRECT3DINDEXBUFFER9  meshfiled_indexbuffer;
	LPDIRECT3DTEXTURE9      meshfiled_texture;
	MESHFILED_INFOMATION mesh_info_;
	D3DMATERIAL9 MeshMaterial_;
	D3DXMATRIX world_;
	D3DXMATRIX pos;
	static MESHFILED_VERTEX3D* mpv;
	static MESHFILED_VERTEX3D* dmy;
	int number_of_vertices_;
	int number_of_indices_;
	int number_of_primities_;

	LPD3DXEFFECT effect_;
	D3DXHANDLE technique_;
	D3DXVECTOR3 vx;
	D3DXVECTOR3 vz;
	D3DXVECTOR3 n;

	int nCx;
	int nCy;
	int nCount;
	float fStartX;
	float fStartY;
	float fStartZ;
	float addheight;
};