#include "KiteEnemy.h"
KiteEnemy::KiteEnemy(XMFLOAT3 startPos, PlayerObject* targetPos) :
	Enemy(startPos, targetPos) {
	homing = false;
	homingRange = 1500;
}

void KiteEnemy::Move() {
	if (Vector3(pos - player->pos).Length() >= homingRange) {
		homing = true;
	}
	else {
		homing = false;
	}

	if (homing) {
		Enemy::Move();
	}
	else {
		if (!(Vector3(pos - player->pos).Length() < homingRange + 100 &&
			Vector3(pos - player->pos).Length() > homingRange - 100)) {
			targetVec = Vector3(player->GetPos() - pos);
			targetVec.y = 0;
			velocity -= targetVec.Normal() * moveSpeed;
		}
	}
}