#pragma once
#include <math.h>
#include "Vector3.h"

//サイズからスケールへと変換
static float ConvertSizeToScale(float size) {
	float scale = (log(size+1) / log(4)) * (log(2) / log(10));
	return abs(scale);
}

//向きベクトルから壁ずりベクトルへ変換
static Vector3 CalcWallScratchVector(const Vector3 &moveVec,const Vector3 &normal, Vector3 *scratchNormalVec = nullptr) {
	Vector3 nextVec = moveVec - moveVec.VDot(normal) * normal;
	if (scratchNormalVec != nullptr) {
		*scratchNormalVec = nextVec.Normalize();
	}
	return nextVec;
}

//向きベクトルから反射ベクトル
static Vector3 CalcReflectVector(const Vector3 &moveVec, const Vector3 &normal, Vector3 *reflectNormalVec = nullptr) {
	Vector3 nextVec = moveVec - 2.0f *moveVec.VDot(normal) * normal;
	if (reflectNormalVec != nullptr) {
		*reflectNormalVec = nextVec.Normalize();
	}
	return nextVec;
}

//数値を範囲内に収める
static void Clamp(int *value, int min, int max)
{
	if (*value < min)
	{
		*value = min;
	}
	else if (*value > max)
	{
		*value = max;
	}
}


static Vector3 ConvertNormalToDeg(
	const Vector3 &normalVec,	//向きベクトル
	const Vector3 &baseAxis		//軸
)
{

	double PI = 3.141593;

	double theta = 0.0f;			//弧度法の値を入れる
	Vector3 angle = { 0,0,0 };	//計算して出た度数法の角度を入れる

	if (normalVec.VDot(baseAxis) < 0) {
		angle = normalVec.VCross(baseAxis);
		//外積を求める
		//x		
		theta = asin(angle.x);
		angle.x = theta * (180.0f / PI);
		//y
		theta = asin(angle.y);
		angle.y = theta * (180.0f / PI);
		//z
		theta = asin(angle.z);
		angle.z = theta * (180.0f / PI);
	}
	else {
		//外積を求める
		angle = normalVec.VCross(baseAxis);
		//x		
		theta = asin(angle.x);
		angle.x = theta * (180.0f / PI);
		//y
		theta = asin(angle.y);
		angle.y = -theta * (180.0f / PI) + 180;
		//zw
		theta = asin(angle.z);
		angle.z = -theta * (180.0f / PI);
	}

	return angle;
}