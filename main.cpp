#include "main.h"
#include "render.h"
#include "manager.h"
#include <vector>

/***************************************************************************:***
コーディングルール
1. classの作成時は先頭にCを必ず記入
2. 数字の変更が無いもの[例：constexpr||define]は先頭にkを記入
3. メンバー変数の最後に_（アンダーバーを必ずつける）
4. １つの関数で40行をすぎないようにする。もし過ぎる場合はさらに分割を考える。
5. コンストラクタの初期化リストが１行で収まらない場合は改行して作るようにする。
6. 名前空間で作られる関数、クラスはインデントしない。
*******************************************************************************/
/*******************************************************************************
課題

GM31 No.1 4/25授業内

2DPolygonClass
3DPolygonClass
Model

課題表紙のコメント欄に参考ゲーム名を記入
********************************************************************************/

const std::string CLASS_NAME = "Screen Segmentation";
const std::string WINDOW_NAME = "FrameWork";

//********************************************************************************
//
// プロトタイプ宣言
//
//********************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ
bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight);
void Uninit(void);
void Update(void);
void Draw(void);

//********************************************************************************
//
//グローバル変数
//
//********************************************************************************
CManager* GameMg;
CRendere* Renderer;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 使っていない引数の処理
	lpCmdLine = lpCmdLine;
	hPrevinstance = hPrevinstance;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	// ウィンドウクラスの登録
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

	RegisterClassEx(&wcex);		// WNDCLASSEX型のwcexの中身を登録

								// クライアントサイズの設定
	DWORD WStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	RECT wr = { 0, 0, windows_rect::SCREEN_WIDTH, windows_rect::SCREEN_HEIGHT };	// 矩形データ

	AdjustWindowRect(&wr, WStyle, FALSE);

	int nWindowWidth = wr.right - wr.left;
	int nWindowHeight = wr.bottom - wr.top;

	// ウィンドウの場所を中央に変更
	// デスクトップサイズの取得
	RECT DesktopRect;

	GetWindowRect(GetDesktopWindow(), &DesktopRect);
	int nWindowPosX = (DesktopRect.right - nWindowWidth) / 2;
	int nWindowPosY = (DesktopRect.bottom - nWindowHeight) / 2;

	(DesktopRect.right - DesktopRect.left) < nWindowPosX ? nWindowPosX = 0 : nWindowPosX;
	(DesktopRect.bottom - DesktopRect.top) < nWindowPosY ? nWindowPosY = 0 : nWindowPosY;

	// ウィンドウの作成
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
	// メッセージ&ゲームループ
	MSG msg;
	DWORD dwTime = timeGetTime();
	DWORD dwOldTime = timeGetTime();

	// タイマーの性能を上げる
	timeBeginPeriod(1);
	// 初期化(チェックあり)
	if (!Init(hInstance, hWnd, true, nWindowWidth, nWindowHeight))
	{
		MessageBox(hWnd, "初期化に失敗しました!!", "警告", MB_OK);

		return -1;
	}

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{// メッセージ処理
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
		{// ゲーム処理
			dwTime = timeGetTime();

			if ((dwTime - dwOldTime) >= (1000 / 60))
			{
				// 更新
				Update();

				// 描画
				Draw();

				// 前回の時刻の更新
				dwOldTime = dwTime;
			}
		}
	}

	// 終了
	Uninit();

	timeEndPeriod(1);

	return (int)msg.wParam;
}

//================================================================================
//
// 初期化処理関数
//
//================================================================================

bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	// 使っていない引数の処理
	hInstance = hInstance;

	GameMg = new CManager(hInstance,hWnd,bWindow,nWindowWidth,nWindowHeight);

	GameMg->Init();

	return true;
}

//================================================================================
//
// 終了処理関数
//
//================================================================================
void Uninit(void)
{
	GameMg->Uninit();
	delete GameMg;
}

//================================================================================
//
// 更新関数
//
//================================================================================
void Update(void)
{
	GameMg->Update();
}

//================================================================================
//
// 描画関数
//
//================================================================================
void Draw(void)
{
	CRendere::GetDevice()->Clear(0,										// RECT構造体配列の矩形の数
		NULL,									// RECT構造体の先頭アドレス(画面全体はNULL)
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),	// TARGETは色のクリア、ZBUFFERはZバッファのクリア
		D3DCOLOR_RGBA(30, 64, 192, 255),		// クリアカラ―(TARGETがあるとき)
		1.0f,									// Zバッファのクリア値
		0);									// ステンシル値のクリア値

	GameMg->Draw();

	CRendere::GetDevice()->Present(NULL, NULL, NULL, NULL);
}
//================================================================================
//
// ウィンドウプロシージャ関数
//
//================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY: // ウィンドウ破棄メッセージ
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		{// 終了メッセージ

			int nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2);

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
		int nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2);

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