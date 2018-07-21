#include "camera.h"
#include "main.h"
#include "input.h"
#include "manager.h"

D3DXMATRIX CCamera::m_GetView_;
D3DXMATRIX CCamera::m_GetProj_;

CCamera::~CCamera()
{
}

void CCamera::CameraUpdate()
{
	constexpr float SPEED = 1.5f;
	LPDIRECT3DDEVICE9 device = GetDevice();
	CInputKeyboard* pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	if (pInputKeyboard->GetKeyPress(DIK_D))
	{
		// 前ベクトルを保存
		D3DXVECTOR3 temp_vector = axis_vector[2];

		// 地をはう準備
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ += temp_vector * SPEED;
		this->at_ += temp_vector * SPEED;
	}
	if (pInputKeyboard->GetKeyPress(DIK_A))
	{
		// 前ベクトルを保存
		D3DXVECTOR3 temp_vector = axis_vector[2];

		// 地をはう準備
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ -= temp_vector * SPEED;
		this->at_ -= temp_vector * SPEED;
	}
	if (pInputKeyboard->GetKeyPress(DIK_S))
	{
		// 前ベクトルを保存
		D3DXVECTOR3 temp_vector = axis_vector[1];

		// 地をはう準備
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ -= temp_vector * SPEED;
		this->at_ -= temp_vector * SPEED;
	}
	if (pInputKeyboard->GetKeyPress(DIK_W))
	{
		// 前ベクトルを保存
		D3DXVECTOR3 temp_vector = axis_vector[1];

		// 地をはう準備
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ += temp_vector * SPEED;
		this->at_ += temp_vector * SPEED;
	}

	// 右周り
	if (pInputKeyboard->GetKeyPress(DIK_E))
	{
		// 回転行列
		constexpr float rotation_speed_Y = 0.01f;
		D3DXMatrixRotationY(&rotation_Y, rotation_speed_Y);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Y);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Y);

		this->at_ = this->eye_ + orientation_vector;

		// 前ベクトルを作成( 前ベクトル = 視点座標 - カメラ座標 )
		axis_vector[1] = this->at_ - this->eye_;

		// 前ベクトルを単位ベクトルに正規化
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}


	// 左周り
	if (pInputKeyboard->GetKeyPress(DIK_Q))
	{
		// 回転行列
		constexpr float rotation_speed_Y = -0.01f;
		D3DXMatrixRotationY(&rotation_Y, rotation_speed_Y);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Y);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Y);

		this->at_ = this->eye_ + orientation_vector;

		// 前ベクトルを作成( 前ベクトル = 視点座標 - カメラ座標 )
		axis_vector[1] = this->at_ - this->eye_;

		// 前ベクトルを単位ベクトルに正規化
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}

	// 上周り
	if (pInputKeyboard->GetKeyPress(DIK_T))
	{
		// 回転行列
		constexpr float rotation_speed_Right = -0.01f;
		D3DXMatrixRotationAxis(&rotation_Right, &axis_vector[2], rotation_speed_Right);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Right);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Right);

		this->at_ = this->eye_ + orientation_vector;
	}

	// 下周り
	if (pInputKeyboard->GetKeyPress(DIK_G))
	{
		// 回転行列
		constexpr float rotation_speed_Right = 0.01f;
		D3DXMatrixRotationAxis(&rotation_Right, &axis_vector[2], rotation_speed_Right);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Right);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Right);

		this->at_ = this->eye_ + orientation_vector;
	}


	// 右周り( 視点中心 )
	if (pInputKeyboard->GetKeyPress(DIK_Y))
	{
		// 回転行列
		constexpr float rotation_speed_Y = 0.01f;
		D3DXMatrixRotationY(&rotation_Y, rotation_speed_Y);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->eye_ - this->at_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Y);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Y);

		this->eye_ = this->at_ + orientation_vector;

		// 前ベクトルを作成( 前ベクトル = 視点座標 - カメラ座標 )
		axis_vector[1] = this->at_ - this->eye_;

		// 前ベクトルを単位ベクトルに正規化
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}

	// 左周り
	if (pInputKeyboard->GetKeyPress(DIK_H))
	{
		// 回転行列
		constexpr float rotation_speed_Y = -0.01f;
		D3DXMatrixRotationY(&rotation_Y, rotation_speed_Y);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->eye_ - this->at_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Y);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Y);

		this->eye_ = this->at_ + orientation_vector;

		// 前ベクトルを作成( 前ベクトル = 視点座標 - カメラ座標 )
		axis_vector[1] = this->at_ - this->eye_;

		// 前ベクトルを単位ベクトルに正規化
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}
	// カメラの上方向
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	//ビュー行列の作成
	D3DXMatrixLookAtLH(
		&this->matrix_view,
		&this->eye_,
		&this->at_,
		&up
	);

	//プロジェクション行列の作成
	D3DXMatrixPerspectiveFovLH(&this->matrix_projection, D3DX_PI / 3 /*D3DXToRadian(60),*/, (float)windows_rect::SCREEN_WIDTH / windows_rect::SCREEN_HEIGHT, 0.1f, 10000.0f);

	//各種類行列の設定
	device->SetTransform(D3DTS_VIEW, &this->matrix_view);
	device->SetTransform(D3DTS_PROJECTION, &this->matrix_projection);

	this->m_GetView_ = this->matrix_view;
	this->m_GetProj_ = this->matrix_projection;
}

D3DXMATRIX CCamera::SetView()
{
	return m_GetView_;
}

D3DXMATRIX CCamera::SetProj()
{
	return m_GetProj_;
}
