#include "manager.h"
#include "render.h"

CLight   *CManager::g_light;
CCamera  *CManager::g_camera;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;		// �L�[�{�[�h�ւ̃|�C���^
CInputMouse *CManager::m_pInputMouse = NULL;			// �}�E�X�ւ̃|�C���^
CInputJoypad *CManager::m_pInputJoypad = NULL;			// �W���C�p�b�h�ւ̃|�C���^

MATRIX3DMANAGER::Model skydomemodel;

CManager::CManager(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	// �L�[�{�[�h����������
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	// �}�E�X����������
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);

	// �W���C�p�b�h����������
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

	// �L�[�{�[�h�̊J������
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �}�E�X�̊J������
	if (m_pInputMouse)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// �W���C�p�b�h�̊J������
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
	�V�F�[�_���g�����߂�g_SceneInfo[2]->Set�֐����Ăяo���Ă��邪�A
	���̏�Ԃł͍s�������������ł���A�����Ɗȗ��������邽�߂ɂ�
	�V�F�[�_�[�N���X���쐬�����̒���World��View��Proj���|�����킹��
	�������߂�HLSL���ɖ��߂�����悤�ȕ����̂ق����s���͌���B
	*/

	// �L�[�{�[�h�X�V����
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Update();
	}

	// �}�E�X�X�V����
	if (m_pInputMouse)
	{
		m_pInputMouse->Update();
	}

	// �W���C�p�b�h�X�V����
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