#pragma once
#include "Enemy.h"
class SuctionEnemy :
    public Enemy {
public:
	SuctionEnemy(XMFLOAT3 startPos, PlayerObject* targetPos);
	void Suction();
private:
	bool suctioning;
	float suctioningRange;
};

