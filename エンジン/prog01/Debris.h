#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "Collision.h"
#include "GameObjCommon.h"

class Debris :
	public GameObjCommon
{
public:
	//コンストラクタ
	Debris(Vector3 startPos,Vector3 startVec,float size);
	//内部更新
	void Update();
	//移動量リセットをオーバーライド
	void VelocityReset() override;
	//コンテナ更新
	static void StaticUpdate();
	static void StaticAdaptation();
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

	bool isAttackFlame;
private:
	//空気抵抗
	Vector3 airResistance;

	//数値的な大きさ
	float size;

};

