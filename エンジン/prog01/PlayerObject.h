#pragma once
#include "FbxObject3d.h"
#include "Object3d.h"
#include "Vector3.h"
#include "SphereCollider.h"
#include "Box2DCollider.h"
#include "GameObjCommon.h"
#include "State.h"
#include "ParticleEmitter.h"


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
	float GetEnergy() { return energy; }

	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);



	//回収フラグ
	STATE recovery;
	bool dontRecovery;

	//攻撃回数
	int attackCount;
	//攻撃フラグ
	STATE attack;
	//攻撃力
	float attackPow;

	//
	STATE attackGage;

	//デブリ生成
	STATE debrisCooldown;

	//コライダー
	SphereCollider *broadSphereCollider;	//ブロード
	SphereCollider *pushBackCollider;	//押し返し用
	SphereCollider *attackCollider;	//攻撃用


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



	
	Box2DCollider *toMapChipCollider;

	//パーティクル
	ParticleEmitter* healParticle1;
	ParticleEmitter* healParticle2;
	ParticleEmitter* boomParticle;
	ParticleEmitter* refParticle;
	ParticleEmitter* atkParticle;
	ParticleEmitter* atkStockParticle;
	int atkStockParticleTimer = 0;
	int atkStockParticleMaxTimer = 40;
	ParticleEmitter* recoveryParticle;

	int nowHealFrame = 0;
	int nowBoomFrame = 0;
	bool isHealFrameIncrease = false;
	bool isBoomFrameIncrease = false;
	int maxHealFrame = 3;
	int maxBoomFrame = 2;

	GameObjCommon *coreUp;

	int timer=0;
	int maxTimer = 30;
	int shakeCount = 201;
	int attenuation = 0;
	XMFLOAT2 shake;
};

