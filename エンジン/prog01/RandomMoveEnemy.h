#pragma once
#include "Enemy.h"
class RandomMoveEnemy :
	public Enemy
{
public:
	RandomMoveEnemy(XMFLOAT3 startPos, PlayerObject *targetPos);
	void Move() override;
private:
	STATE randomMove;
	bool homing;
	float homingRange;
	float moveAngle;
};

