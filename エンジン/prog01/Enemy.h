#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "GameObjCommon.h"
#include "SphereCollider.h"
#include "PlayerObject.h"
#include "Box2DCollider.h"
#include "State.h"

class Enemy :
	public GameObjCommon
{
public:
	//コンストラクタ
	Enemy(XMFLOAT3 startPos,PlayerObject *targetPos);
	//初期化
	void Initialize() override;
	//内部更新
	virtual void Update() override;
	virtual void FinalUpdate() override;

	//衝突時コールバック
	void OnCollision(const CollisionInfo &info) override;

public:
	enum STATE_NAME {
		NOUPDATE,		//画面外等アップデートをしない状態
		STAY,			//何もしない
		WANDERING,		//さまよい状態
		HOMING,			//プレイヤーを見つけたとき
		DEAD,			//死亡状態
						
	}state;


public:		//当たり判定関係

	//ダメージを与える
	int Attack();
	//攻撃フラグ
	STATE attack;


	//与えられるダメージ
	int attackPow = 1;
	
	//追跡対象を決定
	//void HomingObjectCheck(Vector3 targetPos);


	//当たり判定に使用
	Vector3 GetPos() { return pos; }
	void SetPos(Vector3 pos) { this->pos = pos; }


	//State
	bool isInvincible;		//無敵状態か


	//通常の索敵範囲
	const float sarchLength = 500;
	//追跡範囲
	const float holmingLength = 700;
	//攻撃範囲
	const float attackLength = 100;

	bool isAlive;


private: // メンバ変数

	//ダメージを受ける
	void Damage(float damage);


	int deadTimer;

	float searchPlayerLen = 5.0f;
	bool isPlayerContact = false;	//playerを確認したか
	bool isWandering = false;		//さまよい状態か
	int wanderingCount = 0;
	float moveSpeed = 5.0f;
	float maxMoveSpeed = 20.0f;

	//体力
	int maxHP;		//最大HP
	int HP;			//現在HP


	//無敵時間
	int InvincibleTimer;	//残り無敵時間

	//待機関係
	int stayTime = 0;
	const int maxStayTime = 120;

	//うろうろ関係
	//ランダム移動用角度
	float moveRad = 0.0f;
	int moveTime = 0;				//移動中タイマー
	const int maxMoveTime = 180;	//移動終了用

	//ホーミング状態
	PlayerObject *player;
	//ターゲット座標ポインタ
	Vector3 *targetPos;
	//ターゲットへのベクトル
	Vector3 targetVec;
	//対象までの距離
	float targetLength;
	//現在の最近対象への距離
	float minTargetLength;



	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);

	SphereCollider *broadSphereCollider;
	SphereCollider *pushBackCollider;
	Box2DCollider *toMapChipCollider;
};