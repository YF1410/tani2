#pragma once
#include "EnemyHelper.h"
#include "Box2DCollider.h"
#include "SphereCollider.h"

class EnemyBullet:
	public EnemyHelper
{
public:
	EnemyBullet(Vector3 startPos, Vector3 startVec, ModelManager::ModelName modelName);
	void Update() override;
	void LustUpdate() override;
	void OnCollision(const CollisionInfo &info) override;


	SphereCollider *broadSphereCollider;	//予測用コライダー
	Box2DCollider *toMapChipCollider;		//マップチップ用コライダー
};

