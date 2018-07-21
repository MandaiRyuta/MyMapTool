#include "main.h"
#include "render.h"
#include "manager.h"
#include <vector>

/***************************************************************************:***
�R�[�f�B���O���[��
1. class�̍쐬���͐擪��C��K���L��
2. �����̕ύX����������[��Fconstexpr||define]�͐擪��k���L��
3. �����o�[�ϐ��̍Ō��_�i�A���_�[�o�[��K������j
4. �P�̊֐���40�s�������Ȃ��悤�ɂ���B�����߂���ꍇ�͂���ɕ������l����B
5. �R���X�g���N�^�̏��������X�g���P�s�Ŏ��܂�Ȃ��ꍇ�͉��s���č��悤�ɂ���B
6. ���O��Ԃō����֐��A�N���X�̓C���f���g���Ȃ��B
*******************************************************************************/
/*******************************************************************************
�ۑ�

GM31 No.1 4/25���Ɠ�

2DPolygonClass
3DPolygonClass
Model

�ۑ�\���̃R�����g���ɎQ�l�Q�[�������L��
********************************************************************************/

const std::string CLASS_NAME = "Screen Segmentation";
const std::string WINDOW_NAME = "FrameWork";

//********************************************************************************
//
// �v���g�^�C�v�錾
//
//********************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�B���h�E�v���V�[�W��
bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight);
void Uninit(void);
void Update(void);
void Draw(void);

//********************************************************************************
//
//�O���[�o���ϐ�
//
//********************************************************************************
CManager* GameMg;
CRendere* Renderer;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �g���Ă��Ȃ������̏���
	lpCmdLine = lpCmdLine;
	hPrevinstance = hPrevinstance;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	// �E�B���h�E�N���X�̓o�^
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME.c_str();
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);		// WNDCLASSEX�^��wcex�̒��g��o�^

								// �N���C�A���g�T�C�Y�̐ݒ�
	DWORD WStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	RECT wr = { 0, 0, windows_rect::SCREEN_WIDTH, windows_rect::SCREEN_HEIGHT };	// ��`�f�[�^

	AdjustWindowRect(&wr, WStyle, FALSE);

	int nWindowWidth = wr.right - wr.left;
	int nWindowHeight = wr.bottom - wr.top;

	// �E�B���h�E�̏ꏊ�𒆉��ɕύX
	// �f�X�N�g�b�v�T�C�Y�̎擾
	RECT DesktopRect;

	GetWindowRect(GetDesktopWindow(), &DesktopRect);
	int nWindowPosX = (DesktopRect.right - nWindowWidth) / 2;
	int nWindowPosY = (DesktopRect.bottom - nWindowHeight) / 2;

	(DesktopRect.right - DesktopRect.left) < nWindowPosX ? nWindowPosX = 0 : nWindowPosX;
	(DesktopRect.bottom - DesktopRect.top) < nWindowPosY ? nWindowPosY = 0 : nWindowPosY;

	// �E�B���h�E�̍쐬
	HWND hWnd = CreateWindowEx(0,
		CLASS_NAME.c_str(),
		WINDOW_NAME.c_str(),
		WStyle,
		nWindowPosX,
		nWindowPosY,
		nWindowWidth,
		nWindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	// ���b�Z�[�W&�Q�[�����[�v
	MSG msg;
	DWORD dwTime = timeGetTime();
	DWORD dwOldTime = timeGetTime();

	// �^�C�}�[�̐��\���グ��
	timeBeginPeriod(1);
	// ������(�`�F�b�N����)
	if (!Init(hInstance, hWnd, true, nWindowWidth, nWindowHeight))
	{
		MessageBox(hWnd, "�������Ɏ��s���܂���!!", "�x��", MB_OK);

		return -1;
	}

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{// ���b�Z�[�W����
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// �Q�[������
			dwTime = timeGetTime();

			if ((dwTime - dwOldTime) >= (1000 / 60))
			{
				// �X�V
				Update();

				// �`��
				Draw();

				// �O��̎����̍X�V
				dwOldTime = dwTime;
			}
		}
	}

	// �I��
	Uninit();

	timeEndPeriod(1);

	return (int)msg.wParam;
}

//================================================================================
//
// �����������֐�
//
//================================================================================

bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	// �g���Ă��Ȃ������̏���
	hInstance = hInstance;

	GameMg = new CManager(hInstance,hWnd,bWindow,nWindowWidth,nWindowHeight);

	GameMg->Init();

	return true;
}

//================================================================================
//
// �I�������֐�
//
//================================================================================
void Uninit(void)
{
	GameMg->Uninit();
	delete GameMg;
}

//================================================================================
//
// �X�V�֐�
//
//================================================================================
void Update(void)
{
	GameMg->Update();
}

//================================================================================
//
// �`��֐�
//
//================================================================================
void Draw(void)
{
	CRendere::GetDevice()->Clear(0,										// RECT�\���̔z��̋�`�̐�
		NULL,									// RECT�\���̂̐擪�A�h���X(��ʑS�̂�NULL)
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),	// TARGET�͐F�̃N���A�AZBUFFER��Z�o�b�t�@�̃N���A
		D3DCOLOR_RGBA(30, 64, 192, 255),		// �N���A�J���\(TARGET������Ƃ�)
		1.0f,									// Z�o�b�t�@�̃N���A�l
		0);									// �X�e���V���l�̃N���A�l

	GameMg->Draw();

	CRendere::GetDevice()->Present(NULL, NULL, NULL, NULL);
}
//================================================================================
//
// �E�B���h�E�v���V�[�W���֐�
//
//================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY: // �E�B���h�E�j�����b�Z�[�W
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		{// �I�����b�Z�[�W

			int nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_DEFBUTTON2);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
		}
		break;

		}
		break;

	case WM_CLOSE:
	{
		int nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_DEFBUTTON2);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	}

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LPDIRECT3DDEVICE9 GetDevice(void)
{
	return CRendere::GetDevice();
}