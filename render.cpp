#include "render.h"

LPDIRECT3D9 CRendere::pD3D_;
LPDIRECT3DDEVICE9 CRendere::device_;

bool CRendere::Init(HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D_ == NULL)
	{
		MessageBox(hWnd, "Direct3Dインターフェースの取得に失敗しました。", NULL, MB_OK);
		return false;
	}

	D3DDISPLAYMODE d3ddm;

	if (FAILED(pD3D_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(hWnd, "ディスプレイモードの取得に失敗しました。", NULL, MB_OK);
		return false;
	}

	// デバイスのプレゼンテーションパラメータ(デバイスを作成するためのデータ)
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));							//　中のものを全てを初期化する(初期化する先頭アドレス, 初期化するサイズ)
	d3dpp.BackBufferWidth = nWindowWidth;				//　裏画面(バックバッファ)の幅
	d3dpp.BackBufferHeight = nWindowHeight;				//	裏画面(バックバッファ)の高さ
	d3dpp.BackBufferFormat = d3ddm.Format;				//	色数
	d3dpp.BackBufferCount = 1;							//	バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//	映像信号の同期(DISCARDは垂直同期を気にする)
	d3dpp.EnableAutoDepthStencil = TRUE;						//	TRUEで良い
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//	今はこれで良い上のフォーマット
	d3dpp.Windowed = bWindow;						//	TRUEでウィンドウモード,FALSEでフルスクリーンモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//	フルスクリーンモードの時のリフレッシュレートのHz数を変更
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//	モニターのHz数に合わせて描画される(IMMEDIATEだと垂直同期を待たずに描画する)

	if (FAILED(pD3D_->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&device_
	)))
	{
		MessageBox(hWnd, "Direct3DDeviceの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	device_->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device_->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device_->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device_->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	device_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device_->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return true;
}
void CRendere::Uninit()
{
	if (pD3D_ != NULL)
	{
		pD3D_->Release();
		pD3D_ = NULL;
	}
	if (device_ != NULL)
	{
		device_->Release();
		device_ = NULL;
	}
}

bool CRendere::DrawBegin()
{
	return SUCCEEDED(device_->BeginScene());
}

void CRendere::DrawEnd()
{
	device_->EndScene();
}

LPDIRECT3DDEVICE9 CRendere::GetDevice()
{
	return device_;
}
