#pragma once
#include "main.h"

class CRendere
{
private:
	static LPDIRECT3D9 pD3D_;
	static LPDIRECT3DDEVICE9 device_;
public:
	CRendere() {}
	~CRendere() {}
public:
	static bool Init(HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight);
	static void Uninit();
	static bool DrawBegin();
	static void DrawEnd();

	static LPDIRECT3DDEVICE9 GetDevice();
};