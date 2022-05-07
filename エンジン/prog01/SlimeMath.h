#pragma once
#include <math.h>
#include "WinApp.h"
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
		*scratchNormalVec = nextVec.Normal();
	}
	return nextVec;
}

//�����x�N�g�����甽�˃x�N�g��
static Vector3 CalcReflectVector(const Vector3 &moveVec, const Vector3 &normal, Vector3 *reflectNormalVec = nullptr) {
	Vector3 nextVec = moveVec - 2.0f *moveVec.VDot(normal) * normal;
	if (reflectNormalVec != nullptr) {
		*reflectNormalVec = nextVec.Normal();
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


//���[���h���W����X�N���[�����W
static XMFLOAT2 ConvertWorldToScreen() { return { 0,0 }; }
//�X�N���[�����W���烏�[���h���W
static XMFLOAT3 ConvertScreenToWorld(
	XMFLOAT2 *screenPos,	//�X�N���[����̈ʒu
	float fZ,				//�ˉe��ԏ��Z���W
	XMFLOAT2 windowSize,	//�E�B���h�E�T�C�Y
	Camera *camera

	) {
	XMMATRIX InvView, InvPrj, Vp, InvViewport;
	Vp = XMMatrixIdentity();
	XMFLOAT4X4 mat;
	XMStoreFloat4x4(&mat, Vp);
	InvView = XMMatrixInverse(nullptr, camera->GetViewMatrix());
	InvPrj = XMMatrixInverse(nullptr, camera->GetProjectionMatrix());
	mat._11 = WinApp::window_width / 2.0f;
	mat._22 = -WinApp::window_height / 2.0f;
	mat._41 = WinApp::window_width / 2.0f;
	mat._42 = WinApp::window_height / 2.0f;
	Vp = XMLoadFloat4x4(&mat);
	InvViewport = XMMatrixInverse(nullptr, Vp);

	//�t�ϊ�
	XMMATRIX tmp = InvViewport * InvPrj * InvView;
	Vector3 worldPos = XMVector3TransformCoord(Vector3(screenPos->x, screenPos->y, fZ), tmp);


	return worldPos;
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