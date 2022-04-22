#pragma once
#include "FbxObject3d.h"
#include "Object3d.h"
#include "Vector3.h"
#include "SphereCollider.h"
#include "Box2DCollider.h"
#include "GameObjCommon.h"
#include "State.h"

enum REVERSE_Range {
	MIN = 800,
	MAX = 1000
};

class PlayerObject :
	public GameObjCommon
{
public:
	PlayerObject(XMFLOAT3 startPos);
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
	float GetSuction() { return suction; }
	float GetSpeed() { return velocity.Length(); }
	float GetScale() { return scalef; }

	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);

	int GetHp() { return size; }


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
	
	float size;	//質量

	//キーボード移動用
	float moveSpead;
	
	//スタート位置
	XMFLOAT3 startPos;
	//スケールに対する吸引比率
	const float suctionRatio = 600.0f;
	//吸引範囲
	float suction;
	//サイズ
	float scalef;//大きさ

	//反射フラグ
	bool isBounce;

	//自爆力
	const int destructPow = 10;

	//コライダー
	SphereCollider *broadSphereCollider;	//ブロード
	SphereCollider * pushBackCollider;	//押し返し用
	SphereCollider *attackCollider;	//攻撃用

	Box2DCollider *toMapChipCollider;


	GameObjCommon *coreUp;
};

