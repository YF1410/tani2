#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "Collision.h"

class Debris
{
public:
	//コンストラクタ
	Debris(XMFLOAT3 startPos,Vector3 startVec,float size);
	//内部更新
	void Update();
	//最終更新
	void Adaptation();
	//描画
	void Draw();
	//コンテナ更新
	static void StaticUpdate();
	static void StaticReflection();
	static void StaticDraw();

public:		//衝突時関係

	//当たり判定
	struct COLLIDER {
		//見た目に近い判定
		Sphere realSphere;
		//攻撃判定
		Sphere attackSphere;
		//当たり判定
		Sphere hitSphere;
	}collider;
	//当たり判定一括更新用
	void UpdateCollider();
	//跳ね返るとき
	void Bounse(
		const Vector3 &hitPos,		//衝突位置
		const Vector3 &normal	//衝突した物との向きベクトル
	);
	//プレイヤーに吸い寄せられたとき
	void SuckedPlayer(const Vector3 &pos, const float &suckedRadius);
	//プレイヤーに吸収されたとき
	float AbsorbedToPlayer();

	//ダメージを受ける
	void Damage(float damage);

	Vector3 GetPos() { return pos; }


	//生存フラグ
	bool isAlive;
	//攻撃状態
	bool isAttack;
	
	//コンテナ
	static std::vector<Debris *> debris;

	Vector3 pos;
	Vector3 moveVec;
	

	bool isAttackFlame;
private:
	//破片オブジェクト
	std::unique_ptr<FbxObject3d> debriObj;

	//計算用座標
	//総移動量
	//移動予想位置
	Vector3 afterPos;

	//空気抵抗
	Vector3 airResistance;

	//数値的な大きさ
	float size;
	//描画用大きさ
	float scale;

};

