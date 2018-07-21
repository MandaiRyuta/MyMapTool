#pragma once
#include "Scene.h"
#include <d3dx9.h>
#include <math.h>
class CSphereColision
{
public:
	CSphereColision() {}
	~CSphereColision() {}

public:
	static bool Collision_detection_of_Sphere_and_Sphere(D3DXVECTOR3* hit_position, const SphereInfo sphere_01, const SphereInfo sphere_02);
	static void NecessaryLengthCalculation_Sphere_Sphere(D3DXVECTOR3 *center_to_center_vector, float *two_radius, const SphereInfo *sphere0, const SphereInfo *sphere1);
	static bool Is_Sphere_and_Sphere_two_Square_Range_by_Collison_detection(const D3DXVECTOR3 *center_to_center_vector, float two_radius);
	static void AmountOfSqueezingVectorCalculation_Sphere_Sphere(D3DXVECTOR3 *hit_vector, D3DXVECTOR3 *center_to_center_vector, float two_radius);
};

D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pout,
	int ScreenX,
	int ScreenY,
	float fZ,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* view,
	D3DXMATRIX* proj
);
D3DXVECTOR3* CalcScreenToXZ(
	D3DXVECTOR3* pout,
	int ScreenX,
	int ScreenY,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* view,
	D3DXMATRIX* prj
);