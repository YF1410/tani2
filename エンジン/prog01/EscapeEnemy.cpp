#include "EscapeEnemy.h"

EscapeEnemy::EscapeEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos,targetPos){
	homingRange = 1500;
}

void EscapeEnemy::Move()
{
	if (Vector3(pos - player->pos).Length() <= homingRange) {
		homing = true;
	}
	else {
		homing = false;
	}

	if (homing) {
		Enemy::Move();
	}
	else {

		targetVec = Vector3(player->GetPos() - pos);
		targetVec.y = 0;
		velocity -= targetVec.Normal() * moveSpeed * 0.5f;
	}
}
