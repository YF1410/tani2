#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "Collision.h"

class Debris
{
public:
	Debris(XMFLOAT3 startPos,Vector3 startVec,float size);
	//内部更新
	void Update();
	//最終更新
	void Reflection();
	//描画
	void Draw();

	static void StaticUpdate();
	static void StaticReflection();
	static void StaticDraw();
public:		//衝突時関係

	//当たり判定
	struct COLLIDER {
		//跳ね返り用おおざっぱ
		Sphere realSphere;
		Sphere absorbedSphere;

	}collider;
	//当たり判定一括更新用
	void UpdateCollider();
	//跳ね返るとき
	void Bounse(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal	//衝突した物との向きベクトル
	);
	//プレイヤーに吸い寄せられたとき
	void SuckedPlayer(const Vector3 &pos, const float &suckedRadius);
	//プレイヤーに吸収されたとき
	float AbsorbedToPlayer();

	//破片同士でくっついたとき
	float AbsorbedToDebri();

	Vector3 GetPos() { return pos; }


	//生存フラグ
	bool isAlive;
	//攻撃状態
	bool isAttack;
	//停止
	bool isStop;

	//コンテナ
	static std::vector<Debris *> debris;
private:
	//破片オブジェクト
	std::unique_ptr<FbxObject3d> debriObj;

	//計算用座標
	Vector3 pos;
	//総移動量
	Vector3 moveVec;
	//移動予想位置
	Vector3 afterPos;

	//空気抵抗
	Vector3 airResistance;

	//数値的な大きさ
	float size;
	//描画用大きさ
	float scale;

};

