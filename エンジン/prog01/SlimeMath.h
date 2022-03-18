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
