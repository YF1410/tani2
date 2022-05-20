#pragma once
#include "Enemy.h"
class ShotBoss :
	public Enemy
{
public:
	ShotBoss(XMFLOAT3 startPos, PlayerObject *targetPos);
	void Update() override;
	void Draw() const override;
	void Move() override;
protected:
	void HitPlayer(const CollisionInfo &info) override;
	void HitDebri(const CollisionInfo &info) override;

private:
	std::unique_ptr<GameObjCommon> core;
	//��]��
	Vector3 coreBasePos;
	Vector3 offset;
	//��]����
	Vector3 spinVec;
	const float spinR = 80;

	SphereCollider *coreCollider;	//�\���p�R���C�_�[

};

