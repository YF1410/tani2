#pragma once
#include "Enemy.h"
class StayEnemy :
	public Enemy
{
public:
	StayEnemy(XMFLOAT3 startPos, PlayerObject *targetPos);
	void Move() override;
private:
	const float attackRange = 1000.0f;
};

