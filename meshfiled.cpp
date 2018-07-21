#include "meshfiled.h"
#include "manager.h"
#include "csvdata.h"
#include "input.h"
#include "colision.h"
#include "Noise.h"
#include <stdio.h>
#include <time.h>
float g_FiledHeight[MeshFiled_X + 1][MeshFiled_Z + 1] = {};
MESHFILED_VERTEX3D *MeshFiled::mpv;

void MeshFiled::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	addheight = 0.0f;
	std::vector<std::vector<std::string>> table;
	CsvConnect::CsvRead("meshfiled.csv", table);

	fStartX = -this->mesh_info_.size_x * (this->mesh_info_.filed_x / 2), fStartY = 0.0f, fStartZ = this->mesh_info_.size_z * (this->mesh_info_.filed_y / 2);

	int nCx = this->mesh_info_.filed_x + 1, nCy = this->mesh_info_.filed_y + 1;
	
	this->number_of_vertices_ = nCx * nCy;														//頂点数
	this->number_of_indices_ = (nCx * 2 + 1) * (nCy - 1) + ((nCy - 2) * 1);				//インデックス数
	this->number_of_primities_ = this->number_of_indices_ - 2;								//Primitive数
	float frequency = 3.0f;
	int octaves = 8;
	std::uint32_t seed = 32;
	const siv::PerlinNoise perlin(seed);
	const float fx = nCx / frequency;
	const float fy = nCy / frequency;
	float ypos;
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(MESHFILED_VERTEX3D) * this->number_of_vertices_,
		D3DUSAGE_WRITEONLY,
		FVF_MESHFILED3D,
		D3DPOOL_MANAGED,
		&this->meshfiled_vertexbuffer,
		NULL))) {
		PostQuitMessage(0);
	}
	this->meshfiled_vertexbuffer->Lock(0, 0, (void**)&mpv, D3DLOCK_DISCARD);
	/*
	フィールドの端は見せない、もし見る場合があるなら、最後の法線を１にセットしておく
	*/

	for (int nZ = 0, nCount = 0; nZ < nCy - 1; nZ++) {
		for (int nX = 0; nX < nCx; nX++) {
			const sRGB color(perlin.octaveNoise0_1(nX / fx, nZ / fy, octaves));
			ypos = (color.r + color.b + color.g)*frequency*octaves - 25;
			mpv[nCount].pos = D3DXVECTOR3(fStartX + (this->mesh_info_.size_x * nX), ypos, fStartZ - (this->mesh_info_.size_z * nZ));

			vx = mpv[nZ * nCy + (nX + 1)].pos - mpv[nZ * nCx + (nX - 1)].pos;
			vz = mpv[(nZ + 1) * nCy + nX].pos - mpv[nZ * nCx + nX].pos;

			D3DXVec3Cross(&n, &vx, &vz);
			D3DXVec3Normalize(&n, &n);
			mpv[nZ * nCx + nX].Normal = n;

			mpv[nCount].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			mpv[nCount].texcoord = D3DXVECTOR2((this->mesh_info_.size_x * nX) / this->mesh_info_.size_x, (this->mesh_info_.size_z * nZ) / this->mesh_info_.size_z);

			nCount++;

		}
	}
	this->meshfiled_vertexbuffer->Unlock();
	LPWORD ppIndex;
	int nS = 0, nF = 0, nC = 2 * nCx, nD = 2 * nCx + 1;

	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD)* this->number_of_indices_,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&this->meshfiled_indexbuffer,
		NULL))) {
		PostQuitMessage(0);
	}

	this->meshfiled_indexbuffer->Lock(0, 0, (void**)&ppIndex, D3DLOCK_DISCARD);

	for (int nCount = 0; nCount < this->number_of_indices_; nCount++)
	{
		//インデックス偶数番の縮退
		if (nCount == nC)
		{
			ppIndex[nCount] = ppIndex[nCount - 1];
			nC += (2 * (nCx + 1));
			continue;
		}
		//インデックス偶数番
		if (nCount % 2 == 0)
		{
			ppIndex[nCount] = nCx + nS;
			nS++;
		}
		//インデックス奇数番の縮退
		if (nCount == nD)
		{
			ppIndex[nCount] = nCx + nS;
			nD += (2 * (nCx + 1));
			continue;
		}
		//インデックス奇数番
		if (nCount % 2 == 1)
		{
			ppIndex[nCount] = nF;
			if (nCount == ((nC * nCx) - 1))
			{
				ppIndex[nCount] = nF;
				continue;
			}
			else
			{
				nF++;
			}
		}
	}

	this->meshfiled_indexbuffer->Unlock();

	D3DXCreateTextureFromFile(pDevice, this->filename_.c_str(), &this->meshfiled_texture);

	ZeroMemory(&MeshMaterial_, sizeof(MeshMaterial_));

	MeshMaterial_.Diffuse.a = 1.0f;
	MeshMaterial_.Diffuse.r = 0.8f;
	MeshMaterial_.Diffuse.g = 0.7f;
	MeshMaterial_.Diffuse.b = 0.8f;
	MeshMaterial_.Ambient.a = 1.0f;
	MeshMaterial_.Ambient.r = 0.7f;
	MeshMaterial_.Ambient.g = 0.7f;
	MeshMaterial_.Ambient.b = 0.7f;


	D3DXMatrixIdentity(&this->world_);
}
void MeshFiled::Update()
{

	CInputMouse* mouse;
	mouse = CManager::GetInputMouse();
	D3DXVECTOR3 mousepos;
	mousepos.x = (float)mouse->GetAxisX();
	mousepos.y = (float)mouse->GetAxisY();
	mousepos.z = (float)mouse->GetAxisZ();
	CalcScreenToXZ(&mousepos, 0, 0, windows_rect::D3D_SCREEN_WIDTH, windows_rect::D3D_SCREEN_WIDTH, &CCamera::SetView(), &CCamera::SetProj());
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	fStartX = -this->mesh_info_.size_x * (this->mesh_info_.filed_x / 2), fStartY = 0.0f, fStartZ = this->mesh_info_.size_z * (this->mesh_info_.filed_y / 2);

	int nCx = this->mesh_info_.filed_x + 1, nCy = this->mesh_info_.filed_y + 1;

	this->number_of_vertices_ = nCx * nCy;														//頂点数
	this->number_of_indices_ = (nCx * 2 + 1) * (nCy - 1) + ((nCy - 2) * 1);				//インデックス数
	this->number_of_primities_ = this->number_of_indices_ - 2;								//Primitive数

	this->meshfiled_vertexbuffer->Lock(0, 0, (void**)&mpv, D3DLOCK_DISCARD);
	for (int nZ = 0, nCount = 0; nZ < nCy - 1; nZ++) {
		for (int nX = 0; nX < nCx; nX++) {
			//y = g_FiledHeight[nZ][nX];
			/*g_FiledHeight[nZ][nX] = strtof(table[nZ][nX].c_str(), &end[0]);*/
			mpv[nCount].pos = AddHeight(mpv[nCount].pos, mousepos);

			vx = mpv[nZ * nCy + (nX + 1)].pos - mpv[nZ * nCx + (nX - 1)].pos;
			vz = mpv[(nZ + 1) * nCy + nX].pos - mpv[nZ * nCx + nX].pos;

			D3DXVec3Cross(&n, &vx, &vz);
			D3DXVec3Normalize(&n, &n);
			mpv[nZ * nCx + nX].Normal = n;

			mpv[nCount].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			mpv[nCount].texcoord = D3DXVECTOR2((this->mesh_info_.size_x * nX) / this->mesh_info_.size_x, (this->mesh_info_.size_z * nZ) / this->mesh_info_.size_z);

			nCount++;

		}
	}

	this->meshfiled_vertexbuffer->Unlock();

}

void MeshFiled::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CInputKeyboard* KeyInputState = CManager::GetInputKeyboard();

	pDevice->SetFVF(FVF_MESHFILED3D);
	pDevice->SetStreamSource(0, this->meshfiled_vertexbuffer, 0, sizeof(MESHFILED_VERTEX3D));
	pDevice->SetIndices(this->meshfiled_indexbuffer);
	pDevice->SetTexture(0, this->meshfiled_texture);
	pDevice->SetMaterial(&this->MeshMaterial_);
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	if (KeyInputState->GetKeyTrigger(DIK_1))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	if (KeyInputState->GetKeyTrigger(DIK_2))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	pDevice->SetTransform(D3DTS_WORLD, &this->world_);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this->number_of_vertices_, 0, this->number_of_primities_);
}

void MeshFiled::Uninit()
{
	if (this->meshfiled_vertexbuffer != NULL)
	{
		this->meshfiled_vertexbuffer->Release();
		this->meshfiled_vertexbuffer = NULL;
	}
	if (this->meshfiled_indexbuffer != NULL)
	{
		this->meshfiled_indexbuffer->Release();
		this->meshfiled_indexbuffer = NULL;
	}
	if (this->meshfiled_texture != NULL)
	{
		this->meshfiled_texture->Release();
		this->meshfiled_texture = NULL;
	}
}

MeshFiled * MeshFiled::Create(MESHFILED_INFOMATION mesh_info, std::string filename)
{
	MeshFiled* CreateMeshFiled = new MeshFiled(SCENE_3D_POLYGON, filename, mesh_info);
	CreateMeshFiled->Init();
	return CreateMeshFiled;
}

float MeshFiled::GetHeight(D3DXVECTOR3 position)
{
	D3DXVECTOR3 V01 = {};
	D3DXVECTOR3 V12 = {};
	D3DXVECTOR3 V20 = {};

	D3DXVECTOR3 V0p = {};
	D3DXVECTOR3 V1p = {};
	D3DXVECTOR3 V2p = {};
	/*2次元の外積値*/
	float C0 = 0.0f;
	float C1 = 0.0f;
	float C2 = 0.0f;

	float C3 = 0.0f;
	float C4 = 0.0f;
	float C5 = 0.0f;

	D3DXVECTOR3 n;

	int MaxFiledZ = (MeshFiled_Z)+1;
	int MaxFiledX = (MeshFiled_X)+1;

	for (int nZ = 0; nZ < MeshFiled_Z; nZ++)
	{
		for (int nX = 0; nX < MeshFiled_X; nX++)
		{
			V01 = mpv[(nZ + 1)* MaxFiledZ + nX].pos - mpv[nZ * MaxFiledZ + nX].pos;
			V12 = mpv[(nZ + 1) * MaxFiledZ + (nX + 1)].pos - mpv[(nZ + 1) * MaxFiledZ + nX].pos;
			V20 = mpv[nZ * MaxFiledZ + nX].pos - mpv[(nZ + 1) * MaxFiledZ + (nX + 1)].pos;

			V0p = position - mpv[nZ *MaxFiledZ + nX].pos;
			V1p = position - mpv[(nZ + 1) * MaxFiledZ + nX].pos;
			V2p = position - mpv[(nZ + 1) * MaxFiledZ + (nX + 1)].pos;

			C0 = V01.x * V0p.z - V01.z * V0p.x;
			C1 = V12.x * V1p.z - V12.z * V1p.x;
			C2 = V20.x * V2p.z - V20.z * V2p.x;

			if (C0 >= 0.0f && C1 >= 0.0f && C2 >= 0.0f)
			{
				D3DXVec3Cross(&n, &V01, &V12);
				position.y = mpv[nZ * MaxFiledZ + nX].pos.y - (n.x * (position.x - mpv[nZ * MaxFiledZ + nX].pos.x) + n.z * (position.z - mpv[nZ * MaxFiledZ + nX].pos.z)) / n.y;
				break;
			}

			V01 = mpv[nZ * MaxFiledZ + nX].pos - mpv[(nZ + 1) * MaxFiledZ + (nX + 1)].pos;
			V12 = mpv[nZ * MaxFiledZ + (nX + 1)].pos - mpv[nZ * MaxFiledZ + nX].pos;
			V20 = mpv[(nZ + 1) * MaxFiledZ + (nX + 1)].pos - mpv[nZ * MaxFiledZ + (nX + 1)].pos;

			V0p = position - mpv[(nZ + 1) * MaxFiledZ + (nX + 1)].pos;
			V1p = position - mpv[nZ * MaxFiledZ + nX].pos;
			V2p = position - mpv[nZ * MaxFiledZ + (nX + 1)].pos;

			C3 = V01.x * V0p.z - V01.z * V0p.x;
			C4 = V12.x * V1p.z - V12.z * V1p.x;
			C5 = V20.x * V2p.z - V20.z * V2p.x;

			if (C3 <= 0.0f && C4 <= 0.0f && C5 <= 0.0f)
			{
				D3DXVec3Cross(&n, &V01, &V12);
				position.y = mpv[(nZ + 1) * MaxFiledZ + (nX + 1)].pos.y - (n.x * (position.x - mpv[(nZ + 1) * MaxFiledZ + (nX + 1)].pos.x) + n.z * (position.z - mpv[(nZ + 1) * MaxFiledZ + (nX + 1)].pos.z)) / n.y;
				break;
			}
		}
	}
	return position.y;
}

D3DXVECTOR3 MeshFiled::AddHeight(D3DXVECTOR3 position, D3DXVECTOR3 mousepos)
{
	CInputMouse* mouse;

	mouse = CManager::GetInputMouse();
	SphereInfo mouseInfo;
	SphereInfo meshInfo;

	mouseInfo.modelpos = mousepos;
	mouseInfo.r = 5.f;
	meshInfo.modelpos = position;
	meshInfo.r = 3.f;

	if (mouse->GetLeftPress())
	{
		D3DXVECTOR3 hit;
		if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hit, mouseInfo, meshInfo))
		{
			D3DXVec3Normalize(&hit, &hit);
			position.y += -hit.y;
			return position;
		}
		addheight = 0.0f;
	}
	if (mouse->GetRightPress())
	{
		D3DXVECTOR3 hit;
		if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hit, mouseInfo, meshInfo))
		{
			D3DXVec3Normalize(&hit, &hit);
			position.y += hit.y;
			return position;
		}
		addheight = 0.0f;
	}
	return position;
}
