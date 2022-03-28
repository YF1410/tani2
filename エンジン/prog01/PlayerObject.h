#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include "SphereCollider.h"
#include "GameObjCommon.h"

enum DESTRUCT_TYPE {
	DIRECTIVITY	=1,	//指向性
	CIRCLE		=2,	//全方向
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
	
	//衝突時コールバック
	void OnCollision(const CollisionInfo &info) override;
	
	
	float size;	//質量

	//ゲッター
	Vector3 GetPos() { return pos; }
	Vector3 *GetPosPointer() { return &pos; }
	float GetSuction() { return suction; }
	float GetSpeed() { return velocity.Length(); }
	float GetScale() { return scalef; }
	BOX2D GetBox() { return rect2d; }

	//壁との衝突
	void HitWall(
		const XMVECTOR &hitPos,		//衝突位置
		const Vector3 &normal);
private:	//衝突時の処理関数

private: // メンバ変数
	//キーボード移動用
	float moveSpead;
	
	//スタート位置
	XMFLOAT3 startPos;
	//スケールに対する吸引比率
	const float suctionRatio = 300.0f;
	//吸引範囲
	float suction;
	//サイズ
	float scalef;//大きさ



	//自爆タイプ
	DESTRUCT_TYPE destructType;
	//自爆力
	const int  destructPow = 10;

	//コライダー
	SphereCollider *hitCollider;	//衝突判定用
	SphereCollider *absorptionCollider;	//衝突判定用

	BOX2D rect2d;					//マップチップ計算用
	void AdjustToMapchipEdgePosition(EdgeType contact_edge, float contact_pos);

};

