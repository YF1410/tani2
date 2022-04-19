#pragma once
#include "Enemy.h"
class GetawayEnemy :
    public Enemy {
public:
	GetawayEnemy(XMFLOAT3 startPos, PlayerObject* targetPos);
	void Move() override;
private:
	bool getingaway;
	float getingawayRange;
};

