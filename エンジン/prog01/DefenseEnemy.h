#pragma once
#include "Enemy.h"
class DefenseEnemy :
    public Enemy {
public:
    DefenseEnemy(XMFLOAT3 startPos, PlayerObject* targetPos);
    void Move() override;
private:
	bool homing;
	float homingRange;
    Vector3 defensePos;
};

