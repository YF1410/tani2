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
#include "EnemyHp.h"

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
	//描画
	virtual void Draw() const override;
	//衝突時コールバック
	virtual void OnCollision(const CollisionInfo &info) override;

public:		//当たり判定関係
	//ダメージを与える
	int CauseDamage();
	//攻撃フラグ
	STATE attack;
	//与えられるダメージ
	int attackPow = 10;

	//State
	bool isInvincible;		//無敵状態か

	//プレイヤーが跳ね返るか否か
	bool isBounce;

	bool isAlive;


	//コライダー
	SphereCollider *broadSphereCollider;	//予測用コライダー
	Box2DCollider *toMapChipCollider;		//マップチップ用コライダー

protected: // メンバ変数
	EnemyHp *hpBer;

	//移動処理
	virtual void Move();
	virtual void Attack();
	
	//ダメージを受ける
	virtual void Damage(float damage);

	//移動系
	float moveSpeed = 5.0f;			//移動量
	float maxMoveSpeed = 15.0f;		//最大移動量
	//通常時の大きさ
	float defScale;

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

	virtual void HitPlayer(const CollisionInfo &info);
	virtual void HitDebri(const CollisionInfo &info);
};