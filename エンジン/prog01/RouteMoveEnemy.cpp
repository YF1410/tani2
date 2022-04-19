#include "RouteMoveEnemy.h"

RouteMoveEnemy::RouteMoveEnemy(XMFLOAT3 startPos, PlayerObject* targetPos) :
	Enemy(startPos, targetPos) {
	homing = false;
	homingRange = 500;
	moveDistance = 50;
	defaultPos = pos;
}

void RouteMoveEnemy::Move() {
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
		if (defaultPos.z-200 >= pos.z || defaultPos.z +200 <= pos.z) {
			targetVec = Vector3(defaultPos - pos);
			targetVec.y = 0;
			velocity += targetVec.Normal() * moveSpeed * 0.5f;
		}
		else {
			pos.x += moveDistance;
			if (pos.x <= 4000) {
				moveDistance *= -1;
			}
			else if (pos.x >= 8000) {
				moveDistance *= -1;
			}
		}
	}
}