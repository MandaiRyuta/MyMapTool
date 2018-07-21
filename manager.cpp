#include "manager.h"
#include "render.h"

CLight   *CManager::g_light;
CCamera  *CManager::g_camera;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;		// キーボードへのポインタ
CInputMouse *CManager::m_pInputMouse = NULL;			// マウスへのポインタ
CInputJoypad *CManager::m_pInputJoypad = NULL;			// ジョイパッドへのポインタ

MATRIX3DMANAGER::Model skydomemodel;

CManager::CManager(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	// キーボード処理初期化
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	// マウス処理初期化
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);

	// ジョイパッド処理初期化
	m_pInputJoypad = new CInputJoypad;
	m_pInputJoypad->Init(hInstance, hWnd);

	if (FAILED(CRendere::Init(hWnd, bWindow, nWindowWidth, nWindowHeight)))
	{
		PostQuitMessage(0);
	}
}

void CManager::Init()
{
	g_camera = new CCamera({ 0.0f,0.0f,0.0f }, { 0.0f,100.0f,-50.0f });
	g_camera->CameraInit();

	g_light = new CLight();
	g_light->light_Init();

	skydomemodel.position_ = { 0.f,0.f,0.f };
	skydomemodel.isscall_ = true;
	skydomemodel.scall_ = { 25.0f,25.0f,25.0f };
	CSkydome::Create(&skydomemodel, XMODELFILEPASS::SKYDOME);

	MESHFILED_INFOMATION meshinfo;
	meshinfo.filed_x = MeshFiled_X;
	meshinfo.filed_y = MeshFiled_Z;
	meshinfo.size_x = MeshFiled_XSize;
	meshinfo.size_z = MeshFiled_ZSize;

	MeshFiled::Create(meshinfo, TEXTURERS::GROUND);
}

void CManager::Uninit()
{
	g_camera->CameraUninit();
	delete g_camera;

	g_light->light_Uninit();
	delete g_light;

	// キーボードの開放処理
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// マウスの開放処理
	if (m_pInputMouse)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// ジョイパッドの開放処理
	if (m_pInputJoypad)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	CScene::ReleaseAll();
}

void CManager::Update()
{
	g_camera->CameraUpdate();
	/*
	シェーダを使うためにg_SceneInfo[2]->Set関数を呼び出しているが、
	この状態では行数が増える一方である、もっと簡略化させるためには
	シェーダークラスを作成しその中でWorldとViewとProjを掛け合わせた
	物を求めてHLSL側に命令させるような方式のほうが行数は減る。
	*/

	// キーボード更新処理
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Update();
	}

	// マウス更新処理
	if (m_pInputMouse)
	{
		m_pInputMouse->Update();
	}

	// ジョイパッド更新処理
	if (m_pInputJoypad)
	{
		m_pInputJoypad->Update();
	}

	CScene::SetMtxProjAll(CCamera::SetProj());
	CScene::SetMtxViewAll(CCamera::SetView());

	CScene::UpdateAll();

	//billboard->Update();
}

void CManager::Draw()
{
	CRendere::DrawBegin();

	//billboard->Draw();

	CScene::DrawAll();

	CRendere::DrawEnd();
}