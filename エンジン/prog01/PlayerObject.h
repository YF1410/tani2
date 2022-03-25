#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include "SphereCollider.h"
#include "GameObjCommon.h"

enum DESTRUCT_TYPE {
	DIRECTIVITY	=1,	//指向性
	CIRCLE		=2,	//全方向
};

enum DESTRUCT_POW {
	WEAK	= 8,	//弱い
	STRONG	= 16	//強い
};

class PlayerObject :
	public GameObjCommon
{
public:
	PlayerObject();
	//初期化
	void Initialize() override;
	//毎フレーム処理
	void Update() override;
	
	//衝突時コールバック
	void OnCollision(const CollisionInfo &info) override;
	
	
	float size;	//質量

	//ゲッター
	Vector3 GetPos() { return pos; }
	Vector3 *GetPosPointer() { return &pos; }
	float GetSuction() { return suction; }
	float GetSpeed() { return velocity.Length(); }
	float GetScale() { return scale; }

private:	//衝突時の処理関数
	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);
	//残骸を吸収した時
	void Absorb(float size);

private: // メンバ変数
	//キーボード移動用
	float moveSpead;
	
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

	//コライダー
	SphereCollider *hitSphere;	//衝突判定用
};

