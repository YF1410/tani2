#pragma once
#include <math.h>
#include "Vector3.h"

//�T�C�Y����X�P�[���ւƕϊ�
static float ConvertSizeToScale(float size) {
	float scale = (log(size+1) / log(4)) * (log(2) / log(10));
	return abs(scale);
}

//�����x�N�g������ǂ���x�N�g���֕ϊ�
static Vector3 CalcWallScratchVector(const Vector3 &moveVec,const Vector3 &normal, Vector3 *scratchNormalVec = nullptr) {
	Vector3 nextVec = moveVec - moveVec.VDot(normal) * normal;
	if (scratchNormalVec != nullptr) {
		*scratchNormalVec = nextVec.Normalize();
	}
	return nextVec;
}

//�����x�N�g�����甽�˃x�N�g��
static Vector3 CalcReflectVector(const Vector3 &moveVec, const Vector3 &normal, Vector3 *reflectNormalVec = nullptr) {
	Vector3 nextVec = moveVec - 2.0f *moveVec.VDot(normal) * normal;
	if (reflectNormalVec != nullptr) {
		*reflectNormalVec = nextVec.Normalize();
	}
	return nextVec;
}

//���l��͈͓��Ɏ��߂�
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
	const Vector3 &normalVec,	//�����x�N�g��
	const Vector3 &baseAxis		//��
)
{

	double PI = 3.141593;

	double theta = 0.0f;			//�ʓx�@�̒l������
	Vector3 angle = { 0,0,0 };	//�v�Z���ďo���x���@�̊p�x������

	if (normalVec.VDot(baseAxis) < 0) {
		angle = normalVec.VCross(baseAxis);
		//�O�ς����߂�
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
		//�O�ς����߂�
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