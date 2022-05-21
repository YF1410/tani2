#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "GameObjCommon.h"
#include "SphereCollider.h"
#include "Box2DCollider.h"
#include "PlayerObject.h"

class Debris :
	public GameObjCommon
{
public:
	//コンストラクタ
	Debris(Vector3 startPos,Vector3 startVec,float size);
	~Debris();
	//内部更新
	void Update();
	void LustUpdate();
	//移動量リセットをオーバーライド
	void VelocityReset() override;
	//コンテナ更新
	static void StaticInitialize(PlayerObject *player);
	static void StaticUpdate();
	static void StaticLustUpdate();
	static void StaticAdaptation();
	static void StaticDraw();
	static void Finalize();

public:
	enum STATE {
		NOUPDATE,		//画面外等アップデートをしない状態
		STAY,			//何もしない
		ATTACK,			//攻撃状態
		RETURN,			//回収
		SUCTION,
	}state;
public:		//衝突時関係
	//衝突時コールバック
	void OnCollision(const CollisionInfo &info) override;

	void ReturnStart();

	//プレイヤーに吸い寄せられたとき
	void SuckedPlayer(const Vector3 &pos, const float &suckedRadius);


	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);

	Vector3 GetPos() { return pos; }

	//サイズを取得
	float GetSize() { return size; }

	void SetVelocity(Vector3 velocity) { this->velocity = velocity; }
	STATE GetState() { return state; }
	void SetState(STATE state) { this->state = state; }

	//生存フラグ
	bool isAlive;
	//攻撃状態
	bool isAttack;
	//発射直後除外用
	bool isFirstAttack;

	bool isHitStop = false;
	int hitStopTimer = 0;

	//回収用のポインター
	static PlayerObject *playerData;
	
	//コンテナ
	static std::vector<Debris *> debris;

	bool isAttackFlame;

	//当たり判定
	SphereCollider *hitCollider;
	//攻撃判定
	SphereCollider *attackCollider;

	//衝突用
	//ダメージを受ける
	void Damage(float damage);
private:
	//空気抵抗
	Vector3 airResistance;

	//数値的な大きさ
	float size;

	//いつまで戻る力を持つか
	int returnTimer;


	bool reversFlag;

	Box2DCollider *toMapChipCollider;			//マップチップ計算用
	bool isBoost;
};

