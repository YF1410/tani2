#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include "Collision.h"

enum DESTRUCT_TYPE {
	DIRECTIVITY	=1,	//指向性
	CIRCLE		=2,	//全方向
};

enum DESTRUCT_POW {
	WEAK	= 8,	//弱い
	STRONG	= 16	//強い
};

class PlayerObject
{
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
	PlayerObject(FbxModel *coreModel = nullptr);
	~PlayerObject();
	//初期化
	void Init();
	//毎フレーム処理
	void Update();
	//最終更新
	void Adaptation();
	//描画
	void Draw();

	

	float size;	//質量

	//ゲッター
	Vector3 GetPos() { return pos; }
	float GetSuction() { return suction; }
	float GetSpeed() { return moveVec.Length(); }
	float GetScale() { return scale; }

public:		//衝突時関係

	//当たり判定
	struct COLLIDER {
		Sphere realSphere;		//見た目が大事な当たり判定
		Sphere suctionSphere;	//破片の吸い寄せ用当たり判定
		Sphere absorbSphere;	//吸収用
	}collider;
	//当たり判定一括更新用
	void UpdateCollider();


	//コールバック
	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);
	//残骸を吸収した時
	void Absorb(float size);

private: // メンバ変数
	//スライムオブジェクト
	std::unique_ptr<FbxObject3d> slimeObj;
	//キーボード移動用
	float moveSpead;
	
	//プレイヤーの基準座標
	Vector3 pos;
	//総移動量
	Vector3 moveVec;
	//移動予想位置
	Vector3 afterPos;
	
	//スケールに対する吸引比率
	const float suctionRatio = 300.0f;
	//吸引範囲
	float suction;
	//サイズ
	float scale;//大きさ



	//自爆タイプ
	DESTRUCT_TYPE destructType;
	//自爆力
	DESTRUCT_POW destructPow;
	//自爆フラグ
	bool destructFlag = false;


};

