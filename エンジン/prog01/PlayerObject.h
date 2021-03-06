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
	enum ANIMATION_TYPE {
		BOOST = 0,
		DEATH = 1,
		MOVE = 2,
		RETRIEVE = 3,
		NEUTRAL,
		MAX

	}animationType;
	bool animationChangeFrag;

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

	void SetEndFlag(bool cFlag, bool gFlag);
	

	//ゲッター
	Vector3 GetPos() { return pos; }
	Vector3 *GetPosPointer() { return &pos; }
	float GetSpeed() { return velocity.Length(); }
	float GetEnergy() { return hp; }

	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);

	float volume = 0.0f;

	//回収フラグ
	STATE recovery;
	int recoveryEndTimer;
	bool dontRecovery;
	bool dontBoost;


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

	//残像生成
	STATE afterImageCooldown;
	std::vector<std::unique_ptr<FbxObject3d>> afterImage;

	//コライダー
	SphereCollider *broadSphereCollider;	//ブロード
	SphereCollider *pushBackCollider;	//押し返し用
	SphereCollider *attackCollider;	//攻撃用


	//hp
	float hp;	//体力
	float maxHp;	//最大

	bool healChack = false;

	void StopState();
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
	ParticleEmitter* refParticle2;
	ParticleEmitter* atkParticle;
	bool refParticleFlag = false;

	int nowBoomFrame = 0;
	bool isHealFrameIncrease = false;
	bool isBoomFrameIncrease = false;
	int maxBoomFrame = 2;


	int maxTimer = 30;
	int shakeCount = 100;
	XMFLOAT2 shake;
	//動かなくするフラグ
	bool endFlag = false;
	//ブースト時の音がなってるか
	bool boostFlag = false;
	//angleを保存
	float saveAngle = 0.0f;
	bool saveAngleFlag = false;

	//シェイク相殺用
	Vector3 savePos;
	int shakeTime;
	//リセットフラグ
	bool resetFlag = false;
	bool isHitStop = false;
	int hitStopTimer = 0;
	int hitStopCount = 0;
	bool isHitStopCoolDown = 0;
	int hitStopCoolDown = 12;
	bool isFirstHitStop = false;
	Vector3 saveVelocity = {0,0,0};
};