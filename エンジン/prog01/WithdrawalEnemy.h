#pragma once
#include "Enemy.h"
class WithdrawalEnemy :
	public Enemy {
public:
	WithdrawalEnemy(XMFLOAT3 startPos, PlayerObject* targetPos);
	void Move() override;
private:
	Vector3 withdrawalPos;
};

