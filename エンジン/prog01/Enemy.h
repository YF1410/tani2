#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "GameObjCommon.h"
#include "SphereCollider.h"
#include "PlayerObject.h"
#include "Box2DCollider.h"
#include "State.h"
#include "Debris.h"


class Enemy :
	public GameObjCommon
{
public:
	//コンストラクタ
	Enemy(XMFLOAT3 startPos,PlayerObject *targetPos);
	~Enemy();
	//初期化
	void Initialize() override;
	//内部更新
	virtual void Update() override;
	//最終更新
	virtual void LustUpdate() override;
	//衝突時コールバック
	virtual void OnCollision(const CollisionInfo &info) override;

	bool GetIsAlive() { return isAlive; }
public:		//当たり判定関係
	//ダメージを与える
	int CauseDamage();
	//攻撃フラグ
	STATE attack;
	//与えられるダメージ
	int attackPow = 1;

	//State
	bool isInvincible;		//無敵状態か

	bool isAlive;

protected: // メンバ変数
	//移動処理
	virtual void Move();
	virtual void Attack();
	
	//ダメージを受ける
	virtual void Damage(float damage);

	//移動系
	float moveSpeed = 5.0f;			//移動量
	float maxMoveSpeed = 15.0f;		//最大移動量

	//体力
	float maxHP;		//最大HP
	float HP;			//現在HP


	//無敵時間
	int InvincibleTimer;	//残り無敵時間

	//プレイヤーのポインタ
	PlayerObject *player;
	//ターゲットへのベクトル
	Vector3 targetVec;
	//対象までの距離
	float targetLength;

	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);

	//コライダー
	SphereCollider *broadSphereCollider;	//予測用コライダー
	SphereCollider *pushBackCollider;		//厳密なコライダー（現状未使用）
	Box2DCollider *toMapChipCollider;		//マップチップ用コライダー
};