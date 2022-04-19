#pragma once
#include "Enemy.h"
class MimicEnemy :
	public Enemy
{
public:
	MimicEnemy(XMFLOAT3 startPos, PlayerObject *targetPos);
	void Move() override;
};

