#pragma once
#include "Enemy.h"

class CushionEnemy:
	public Enemy
{
public:
	CushionEnemy(XMFLOAT3 startPos, PlayerObject *targetPos);
	void OnCollision(const CollisionInfo &info) override;
	void Move() override;
};

