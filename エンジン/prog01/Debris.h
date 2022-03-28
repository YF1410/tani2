#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "GameObjCommon.h"
#include "SphereCollider.h"

class Debris :
	public GameObjCommon
{
public:
	//コンストラクタ
	Debris(Vector3 startPos,Vector3 startVec,float size, Vector3 *playerPos);
	//内部更新
	void Update();
	//移動量リセットをオーバーライド
	void VelocityReset() override;
	//コンテナ更新
	static void StaticUpdate();
	static void StaticAdaptation();
	static void StaticDraw();

public:
	enum STATE {
		NOUPDATE,		//画面外等アップデートをしない状態
		STAY,			//何もしない
		ATTACK,			//攻撃状態
		RETURN,			//回収
	}state;
public:		//衝突時関係
	//衝突時コールバック
	void OnCollision(const CollisionInfo &info) override;

	//跳ね返るとき
	void Bounse(
		const Vector3 &hitPos,		//衝突位置
		const Vector3 &normal	//衝突した物との向きベクトル
	);
	void ReturnStart();

	//プレイヤーに吸い寄せられたとき
	void SuckedPlayer(const Vector3 &pos, const float &suckedRadius);


	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);

	Vector3 GetPos() { return pos; }
	BOX2D GetBox() { return rect2d; }

	//サイズを取得
	float GetSize() { return size; }

	//生存フラグ
	bool isAlive;
	//攻撃状態
	bool isAttack;
	//発射直後除外用
	bool isFirstAttack;
	//回収用のポインター
	Vector3 *playerPos = nullptr;
	
	//コンテナ
	static std::vector<Debris *> debris;

	bool isAttackFlame;
private:
	//空気抵抗
	Vector3 airResistance;

	//数値的な大きさ
	float size;

	//いつまで戻る力を持つか
	int returnTimer;

	//当たり判定
	SphereCollider *hitCollider;
	SphereCollider *attackCollider;


	//衝突用
	//ダメージを受ける
	void Damage(float damage);


	BOX2D rect2d;					//マップチップ計算用
};

