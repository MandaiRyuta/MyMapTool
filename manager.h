#pragma once
#include "Scene.h"
#include "camera.h"
#include "light.h"
#include "skydome.h"
#include "main.h"
#include "input.h"
#include "meshfiled.h"
//ëOï˚êÈåæ
//class CCamera;

constexpr int MeshFiled_X = 100;
constexpr int MeshFiled_Z = 100;
constexpr float MeshFiled_XSize = 10.0f;
constexpr float MeshFiled_ZSize = 10.0f;
class CManager {
private:
	static CCamera*   g_camera;
	static CLight*   g_light;
	static CInputJoypad* m_pInputJoypad;
	static CInputKeyboard* m_pInputKeyboard;
	static CInputMouse* m_pInputMouse;
public:
	CManager(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight);
	~CManager() {}

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
};