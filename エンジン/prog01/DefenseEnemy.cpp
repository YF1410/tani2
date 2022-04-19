#include "DefenseEnemy.h"

DefenseEnemy::DefenseEnemy(XMFLOAT3 startPos, PlayerObject* targetPos) :
	Enemy(startPos, targetPos) {
	homing = false;
	homingRange = 1500;
	defensePos = {6200,0,-10000};
}

void DefenseEnemy::Move() {
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
		targetVec = Vector3(defensePos - pos);
		targetVec.y = 0;
		velocity += targetVec.Normal() * moveSpeed;
	}
}