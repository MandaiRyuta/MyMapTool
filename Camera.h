#pragma once
#include "main.h"

class CCamera
{
private:
	// ビュー変換行列( 4×4のfloat行列 )
	D3DXMATRIX matrix_view;


	// プロジェクション変換行列( 4×4のfloat行列 )
	D3DXMATRIX matrix_projection;


	// カメラの前に物を動かすのに便利
	D3DXVECTOR3 axis_vector[3] = {
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),	// 上ベクトル
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),	// 前ベクトル
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),	// 右ベクトル
	};

	// カメラ座標
	D3DXVECTOR3 eye_;

	// 視点座標
	D3DXVECTOR3 at_;


	// 前ベクトル
	D3DXVECTOR3 previous_vector;

	float translation_speed = 0.0f;
	float radian_speed = 0.0f;

	// 回転行列
	D3DXMATRIX rotation_Y;
	D3DXMATRIX rotation_Right;

	static D3DXMATRIX m_GetProj_;
	static D3DXMATRIX m_GetView_;
public:
	CCamera(D3DXVECTOR3 at, D3DXVECTOR3 eye)
		: at_(at), eye_(eye)
	{
		this->previous_vector = this->at_ - this->eye_;
	}
	~CCamera();
public:
	void CameraInit() {}
	void CameraUpdate();
	void CameraDraw() {}
	void CameraUninit() {};
public:
	static D3DXMATRIX SetView();

	static D3DXMATRIX SetProj();
};

