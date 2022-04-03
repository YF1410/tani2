#pragma once
#include "FbxObject3d.h"
#include "Object3d.h"
#include "Vector3.h"
#include "SphereCollider.h"
#include "Box2DCollider.h"
#include "GameObjCommon.h"

enum DESTRUCT_TYPE {
	DIRECTIVITY	=1,	//指向性
	CIRCLE		=2,	//全方向
};

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
	//衝突時コールバック
	void OnCollision(const CollisionInfo &info) override;
	
	
	float size;	//質量

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

	bool isCheckPoint;

private:
	struct REVERS_AREA {
		std::unique_ptr<Object3d> sphere;
		int timer;
		float alpha;
		REVERS_AREA(Vector3 pos,Model *areaModel) {
			sphere = Object3d::Create(areaModel);
			sphere.get()->SetPosition(pos);
			sphere.get()->SetScale({800,800,800});
			alpha = 1.0f;
			timer = 60;
		}
	};

	std::unique_ptr<Model> areaModel;
	std::vector<REVERS_AREA *> reversAreas;

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

	//回収フラグ
	bool canReturn;
	//再回収可能までのカウンター
	int returnCounter;

	//自爆タイプ
	DESTRUCT_TYPE destructType;
	//自爆力
	const int  destructPow = 10;

	//コライダー
	SphereCollider *broadSphereCollider;	//衝突判定用
	Box2DCollider *toMapChipCollider;

	REVERSE_Range reverseRange;
};

