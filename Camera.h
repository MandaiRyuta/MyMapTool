#pragma once
#include "main.h"

class CCamera
{
private:
	// �r���[�ϊ��s��( 4�~4��float�s�� )
	D3DXMATRIX matrix_view;


	// �v���W�F�N�V�����ϊ��s��( 4�~4��float�s�� )
	D3DXMATRIX matrix_projection;


	// �J�����̑O�ɕ��𓮂����̂ɕ֗�
	D3DXVECTOR3 axis_vector[3] = {
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),	// ��x�N�g��
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),	// �O�x�N�g��
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),	// �E�x�N�g��
	};

	// �J�������W
	D3DXVECTOR3 eye_;

	// ���_���W
	D3DXVECTOR3 at_;


	// �O�x�N�g��
	D3DXVECTOR3 previous_vector;

	float translation_speed = 0.0f;
	float radian_speed = 0.0f;

	// ��]�s��
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

