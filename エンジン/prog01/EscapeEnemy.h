#pragma once
#include "Enemy.h"
class EscapeEnemy :
	public Enemy
{
public:
	EscapeEnemy(XMFLOAT3 startPos, PlayerObject *targetPos);
	void Move() override;
private:
	bool homing;
	float homingRange;
};

