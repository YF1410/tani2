#pragma once
#include "Enemy.h"
class RouteMoveEnemy :
	public Enemy
{
public:
	RouteMoveEnemy(XMFLOAT3 startPos, PlayerObject* targetPos);
	void Move() override;
private:
	bool homing;
	float homingRange;
	float moveDistance;
	Vector3 defaultPos;
};

