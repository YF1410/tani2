#pragma once
#include <math.h>
#include "WinApp.h"
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
		*scratchNormalVec = nextVec.Normal();
	}
	return nextVec;
}

//向きベクトルから反射ベクトル
static Vector3 CalcReflectVector(const Vector3 &moveVec, const Vector3 &normal, Vector3 *reflectNormalVec = nullptr) {
	Vector3 nextVec = moveVec - 2.0f *moveVec.VDot(normal) * normal;
	if (reflectNormalVec != nullptr) {
		*reflectNormalVec = nextVec.Normal();
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


//ワールド座標からスクリーン座標
static XMFLOAT2 ConvertWorldToScreen() { return { 0,0 }; }
//スクリーン座標からワールド座標
static XMFLOAT3 ConvertScreenToWorld(
	XMFLOAT2 *screenPos,	//スクリーン上の位置
	float fZ,				//射影空間上のZ座標
	XMFLOAT2 windowSize,	//ウィンドウサイズ
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

	//逆変換
	XMMATRIX tmp = InvViewport * InvPrj * InvView;
	Vector3 worldPos = XMVector3TransformCoord(Vector3(screenPos->x, screenPos->y, fZ), tmp);


	return worldPos;
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