#pragma once
#include "Enemy.h"
class KiteEnemy :
	public Enemy {
public:
	KiteEnemy(XMFLOAT3 startPos, PlayerObject* targetPos);
	void Move() override;
private:
	bool homing;
	float homingRange;
};

