#include "colision.h"

#include "colision.h"

bool CSphereColision::Collision_detection_of_Sphere_and_Sphere(D3DXVECTOR3* hit_position, const SphereInfo sphere_01, const SphereInfo sphere_02)
{
	// 中心間距離と半径の合計による衝突判定( 2乗 )
	D3DXVECTOR3 center_to_center_vector;
	float    two_radius;

	NecessaryLengthCalculation_Sphere_Sphere(&center_to_center_vector, &two_radius, &sphere_02, &sphere_01);

	if (!Is_Sphere_and_Sphere_two_Square_Range_by_Collison_detection(&center_to_center_vector, two_radius)) return false;

	// めり込み具合を算出
	AmountOfSqueezingVectorCalculation_Sphere_Sphere(hit_position, &center_to_center_vector, two_radius);

	return true;
}

//================================================================================
//
// [ 必要な長さを算出関数( 球_球 ) ]
//
//================================================================================
void CSphereColision::NecessaryLengthCalculation_Sphere_Sphere(D3DXVECTOR3 *center_to_center_vector, float *two_radius, const SphereInfo *sphere0, const SphereInfo *sphere1)
{
	*center_to_center_vector = sphere0->modelpos - sphere1->modelpos;
	*two_radius = sphere0->r + sphere1->r;
}

/*******************************************
球と球2乗の距離で衝突判定関数
********************************************/
bool CSphereColision::Is_Sphere_and_Sphere_two_Square_Range_by_Collison_detection(const D3DXVECTOR3 *center_to_center_vector, float two_radius)
{
	return D3DXVec3Length(center_to_center_vector) > (two_radius * two_radius);
}

//================================================================================
//
// [ めり込みベクトル算出関数( 球_球 ) ]
//
//================================================================================
void CSphereColision::AmountOfSqueezingVectorCalculation_Sphere_Sphere(D3DXVECTOR3 *hit_vector, D3DXVECTOR3 *center_to_center_vector, float two_radius)
{
	if (hit_vector)
	{
		float length = two_radius - D3DXVec3Length(center_to_center_vector);

		*hit_vector = D3DXVECTOR3(length, 0.0f, length);
	}
}

D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pout,
	int ScreenX,
	int ScreenY,
	float fZ,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* view,
	D3DXMATRIX* proj
)
{
	D3DXMATRIX InvView, InvPrj, VP, InvViewPort;
	D3DXMatrixInverse(&InvView, NULL, view);
	D3DXMatrixInverse(&InvPrj, NULL, proj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = Screen_h / 2.0f;
	
	D3DXMatrixInverse(&InvViewPort, NULL, &VP);

	D3DXMATRIX tmp = InvViewPort * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3((float)ScreenX, (float)ScreenY, fZ), &tmp);

	return pout;
}

D3DXVECTOR3* CalcScreenToXZ(
	D3DXVECTOR3* pout,
	int ScreenX,
	int ScreenY,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* view,
	D3DXMATRIX* prj
)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	/*
	nearとfar は　-0.5から0.5の間が触っててやり易かった
	-1.0 ~ 1.0も可能（やれるが、判定がちょっとずれるので感覚がつかみづらい）
	0.0 ~ 1.0も可能（但しちょっとやりづらい）
	*/
	CalcScreenToWorld(&nearpos, ScreenX, ScreenY, -0.5f, Screen_w, Screen_h, view, prj);
	CalcScreenToWorld(&farpos, ScreenX, ScreenY, 0.5f, Screen_w, Screen_h, view, prj);

	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	if (ray.y <= 0) {
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray) * ray;
	}
	else
	{
		*pout = farpos;
	}
	return pout;
}