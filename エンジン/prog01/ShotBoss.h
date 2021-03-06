#pragma once
#include "Enemy.h"
class ShotBoss :
	public Enemy
{
public:
	ShotBoss(XMFLOAT3 startPos, PlayerObject *targetPos, Camera *cam);
	void Update() override;
	void Draw() const override;
	void Move() override;
protected:
	void HitPlayer(const CollisionInfo &info) override;
	void HitDebri(const CollisionInfo &info) override;

private:
	std::unique_ptr<GameObjCommon> core;
	//回転軸
	Vector3 coreBasePos;
	Vector3 offset;
	//回転向き
	Vector3 spinVec;
	const float spinR = 80;

	//スポーン直後
	bool isSpawn;
	Vector3 startPos;
	Camera *cam;
	int startTimer = 0;

	SphereCollider *coreCollider;	//予測用コライダー

};

