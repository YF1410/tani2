#pragma once
#include "Enemy.h"
#include "SlimeMath.h"
class BoundEnemy :
	public Enemy
{
public:
	BoundEnemy(XMFLOAT3 startPos, PlayerObject *targetPos);
protected:
	void HitPlayer(const CollisionInfo &info) override;
};

