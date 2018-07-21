#include "Scene.h"

CScene *CScene::m_Scene[3][20] = { NULL };

CScene::CScene(int Priority/*,SCENE_TYPE Scene_Type*/)
{
	for (int i = 0; i < 20; i++)
	{
		if (m_Scene[Priority][i] == NULL)
		{
			m_Scene[Priority][i] = this;
			//m_Type = Scene_Type;
			break;
		}
	}
}

//SCENE_TYPE CScene::GetType()
//{
//	return m_Type;
//}

void CScene::Release()
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 20; i++)
		{
			if (m_Scene[t][i] == this)
			{
				m_Scene[t][i]->Uninit();
				delete this;
				break;
			}
		}
	}
}

void CScene::UpdateAll()
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 20; i++)
		{
			if (m_Scene[t][i] != NULL)
			{
				m_Scene[t][i]->Update();
			}
		}
	}
}

void CScene::DrawAll()
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 20; i++)
		{
			if (m_Scene[t][i] != NULL)
			{
				m_Scene[t][i]->Draw();
			}
		}
	}
}

void CScene::ReleaseAll()
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 20; i++)
		{
			if (m_Scene[t][i] != NULL)
			{
				m_Scene[t][i]->Release();
			}
		}
	}
}

void CScene::SetMtxViewAll(D3DXMATRIX view)
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 20; i++)
		{
			if (m_Scene[t][i] != NULL)
			{
				m_Scene[t][i]->SetMtxView(view);
			}
		}
	}
}

void CScene::SetMtxProjAll(D3DXMATRIX proj)
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 20; i++)
		{
			if (m_Scene[t][i] != NULL)
			{
				m_Scene[t][i]->SetMtxProj(proj);
			}
		}
	}
}

POLYGONSIZE CScene::SetPolygonsize()
{
	POLYGONSIZE polysize;

	polysize.dx_ = 0.f;
	polysize.dy_ = 300.f;
	polysize.dw_ = 184.f;
	polysize.dh_ = 100.f;
	polysize.tcx_ = 0;
	polysize.tcy_ = 2;
	polysize.tcw_ = 184;
	polysize.tch_ = 500;
	polysize.color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
	polysize.affine_ = false;
	return polysize;
}
