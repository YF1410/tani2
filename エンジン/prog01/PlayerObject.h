#pragma once
#include "FbxObject3d.h"
#include "Object3d.h"
#include "Vector3.h"
#include "SphereCollider.h"
#include "Box2DCollider.h"
#include "GameObjCommon.h"
#include "State.h"


class PlayerObject :
	public GameObjCommon
{
public:
	PlayerObject(XMFLOAT3 startPos);
	~PlayerObject();
	//初期化
	void Initialize() override;
	//毎フレーム処理
	void Update() override;
	void Draw() const override;
	void LustUpdate() override;
	//衝突時コールバック
	void OnCollision(const CollisionInfo &info) override;
	

	//ゲッター
	Vector3 GetPos() { return pos; }
	Vector3 *GetPosPointer() { return &pos; }
	float GetSpeed() { return velocity.Length(); }

	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);

	int GetHp() { return energy; }


	//回収フラグ
	STATE collect;
	//攻撃フラグ
	STATE attack;
	//攻撃力
	float attackPow;

private:


	//被ダメージ
	void Damage(float damage);
	//無敵
	bool isInvincible;
	//時間
	int invincibleCounter;

private: // メンバ変数
	

	//キーボード移動用
	float moveSpead;
	
	//スタート位置
	XMFLOAT3 startPos;
	//スケールに対する吸引比率
	const float suctionRatio = 600.0f;

	//反射フラグ
	bool isBounce;
	
	//エネルギー
	float energy;	//質量
	//自爆で飛ばす数
	const int DESTRUCT_POW = 5;
	//爆発に使う合計エネルギー
	const float SHOT_ENERGY = 100;


	//コライダー
	SphereCollider *broadSphereCollider;	//ブロード
	SphereCollider * pushBackCollider;	//押し返し用
	SphereCollider *attackCollider;	//攻撃用

	Box2DCollider *toMapChipCollider;


	GameObjCommon *coreUp;
};

