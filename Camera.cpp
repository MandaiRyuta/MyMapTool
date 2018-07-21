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
		// �O�x�N�g����ۑ�
		D3DXVECTOR3 temp_vector = axis_vector[2];

		// �n���͂�����
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ += temp_vector * SPEED;
		this->at_ += temp_vector * SPEED;
	}
	if (pInputKeyboard->GetKeyPress(DIK_A))
	{
		// �O�x�N�g����ۑ�
		D3DXVECTOR3 temp_vector = axis_vector[2];

		// �n���͂�����
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ -= temp_vector * SPEED;
		this->at_ -= temp_vector * SPEED;
	}
	if (pInputKeyboard->GetKeyPress(DIK_S))
	{
		// �O�x�N�g����ۑ�
		D3DXVECTOR3 temp_vector = axis_vector[1];

		// �n���͂�����
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ -= temp_vector * SPEED;
		this->at_ -= temp_vector * SPEED;
	}
	if (pInputKeyboard->GetKeyPress(DIK_W))
	{
		// �O�x�N�g����ۑ�
		D3DXVECTOR3 temp_vector = axis_vector[1];

		// �n���͂�����
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ += temp_vector * SPEED;
		this->at_ += temp_vector * SPEED;
	}

	// �E����
	if (pInputKeyboard->GetKeyPress(DIK_E))
	{
		// ��]�s��
		constexpr float rotation_speed_Y = 0.01f;
		D3DXMatrixRotationY(&rotation_Y, rotation_speed_Y);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Y);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Y);

		this->at_ = this->eye_ + orientation_vector;

		// �O�x�N�g�����쐬( �O�x�N�g�� = ���_���W - �J�������W )
		axis_vector[1] = this->at_ - this->eye_;

		// �O�x�N�g����P�ʃx�N�g���ɐ��K��
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}


	// ������
	if (pInputKeyboard->GetKeyPress(DIK_Q))
	{
		// ��]�s��
		constexpr float rotation_speed_Y = -0.01f;
		D3DXMatrixRotationY(&rotation_Y, rotation_speed_Y);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Y);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Y);

		this->at_ = this->eye_ + orientation_vector;

		// �O�x�N�g�����쐬( �O�x�N�g�� = ���_���W - �J�������W )
		axis_vector[1] = this->at_ - this->eye_;

		// �O�x�N�g����P�ʃx�N�g���ɐ��K��
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}

	// �����
	if (pInputKeyboard->GetKeyPress(DIK_T))
	{
		// ��]�s��
		constexpr float rotation_speed_Right = -0.01f;
		D3DXMatrixRotationAxis(&rotation_Right, &axis_vector[2], rotation_speed_Right);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Right);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Right);

		this->at_ = this->eye_ + orientation_vector;
	}

	// ������
	if (pInputKeyboard->GetKeyPress(DIK_G))
	{
		// ��]�s��
		constexpr float rotation_speed_Right = 0.01f;
		D3DXMatrixRotationAxis(&rotation_Right, &axis_vector[2], rotation_speed_Right);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Right);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Right);

		this->at_ = this->eye_ + orientation_vector;
	}


	// �E����( ���_���S )
	if (pInputKeyboard->GetKeyPress(DIK_Y))
	{
		// ��]�s��
		constexpr float rotation_speed_Y = 0.01f;
		D3DXMatrixRotationY(&rotation_Y, rotation_speed_Y);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->eye_ - this->at_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Y);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Y);

		this->eye_ = this->at_ + orientation_vector;

		// �O�x�N�g�����쐬( �O�x�N�g�� = ���_���W - �J�������W )
		axis_vector[1] = this->at_ - this->eye_;

		// �O�x�N�g����P�ʃx�N�g���ɐ��K��
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}

	// ������
	if (pInputKeyboard->GetKeyPress(DIK_H))
	{
		// ��]�s��
		constexpr float rotation_speed_Y = -0.01f;
		D3DXMatrixRotationY(&rotation_Y, rotation_speed_Y);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->eye_ - this->at_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &rotation_Y);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &rotation_Y);

		this->eye_ = this->at_ + orientation_vector;

		// �O�x�N�g�����쐬( �O�x�N�g�� = ���_���W - �J�������W )
		axis_vector[1] = this->at_ - this->eye_;

		// �O�x�N�g����P�ʃx�N�g���ɐ��K��
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}
	// �J�����̏����
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	//�r���[�s��̍쐬
	D3DXMatrixLookAtLH(
		&this->matrix_view,
		&this->eye_,
		&this->at_,
		&up
	);

	//�v���W�F�N�V�����s��̍쐬
	D3DXMatrixPerspectiveFovLH(&this->matrix_projection, D3DX_PI / 3 /*D3DXToRadian(60),*/, (float)windows_rect::SCREEN_WIDTH / windows_rect::SCREEN_HEIGHT, 0.1f, 10000.0f);

	//�e��ލs��̐ݒ�
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
