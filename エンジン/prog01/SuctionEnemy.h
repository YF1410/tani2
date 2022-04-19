#pragma once
#include "Enemy.h"
class SuctionEnemy :
    public Enemy {
public:
	SuctionEnemy(XMFLOAT3 startPos, PlayerObject* targetPos);
	void Move() override;
private:
	float suctioningRange;
	float attackRange;
};

