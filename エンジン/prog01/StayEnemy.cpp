#include "StayEnemy.h"

StayEnemy::StayEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos,targetPos)
{
}

void StayEnemy::Move()
{
	if (Vector3(pos - player->pos).Length() <= attackRange) {
		Enemy::Move();
	}
}
