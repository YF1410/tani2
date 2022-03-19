#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "Collision.h"

class Enemy {
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	//コンストラクタ
	Enemy(XMFLOAT3 startPos);
	//デストラクタ
	~Enemy();
	//内部更新
	virtual void Update();
	//最終更新
	void Adaptation();
	// 描画
	void Draw();
	//コンテナ更新
	static void StaticUpdate();
	static void StaticAdaptation();
	static void StaticDraw();

public:
	enum STATE {
		NOUPDATE,		//画面外等アップデートをしない状態
		STAY,			//何もしない
		WANDERING,		//さまよい状態
		HOMING,			//プレイヤーを見つけたとき
		ATTACK,			//攻撃状態
		DEAD,			//死亡状態
						
	}state;


public:		//当たり判定関係

	//当たり判定
	struct COLLIDER {
		//索敵範囲
		Sphere searchArea;
		//当たり判定
		Sphere hitSphere;
		//攻撃用判定
		Sphere attackSphere;
		//見た目に近い判定
		Sphere realSphere;

	}collider;
	//コライダーの更新
	void UpdateCollider();

	//ダメージを受ける
	void Damage(float damage);
	
	//追跡対象を決定
	void HomingObjectCheck(Vector3 targetPos);


	//当たり判定に使用
	Vector3 GetPos() { return pos; }
	void SetPos(Vector3 pos) { this->pos = pos; }

	//コンテナ
	static std::vector<Enemy*> enemys;

	//State
	bool isInvincible;		//無敵状態か


	//通常の索敵範囲
	const float sarchLength = 500;
	//追跡範囲
	const float holmingLength = 700;

private: // メンバ変数
	std::unique_ptr<FbxObject3d> enemyObj;

	int deadTimer;

	float searchPlayerLen = 5.0f;
	bool isPlayerContact = false;	//playerを確認したか
	bool isWandering = false;		//さまよい状態か
	int wanderingCount = 0;
	float moveSpeed = 3.0f;
	bool isAlive;

	//体力
	float HP;
	//無敵時間
	int InvincibleTimer;	//残り無敵時間

	//計算用座標
	Vector3 pos;
	//総移動量
	Vector3 moveVec;
	//移動位置予想
	Vector3 afterPos;

	//待機関係
	int stayTime = 0;
	const int maxStayTime = 120;

	//うろうろ関係
	//ランダム移動用角度
	float moveRad = 0.0f;
	int moveTime = 0;				//移動中タイマー
	const int maxMoveTime = 180;	//移動終了用

	//ホーミング状態
	//ターゲット座標ポインタ
	Vector3 targetPos;
	//ターゲットへのベクトル
	Vector3 targetVec;
	//対象までの距離
	float targetLength;
	//現在の最近対象への距離
	float minTargetLength;


	//描画用大きさ
	float scale;
};