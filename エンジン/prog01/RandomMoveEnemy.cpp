#include "RandomMoveEnemy.h"

RandomMoveEnemy::RandomMoveEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos, targetPos) {
	randomMove = {
		true,
		false,
		60,
		0
	};
	velocity.x = 1;
	homing = false;
	homingRange = 2500;
	objectData.get()->SetModel(ModelManager::GetIns()->GetModel(ModelManager::ENEMY_ROBO_7));
}

void RandomMoveEnemy::Move()
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
		if (randomMove.can) {
			randomMove.Start();
			moveAngle = rand() % 361;
			float Rad = XMConvertToRadians(moveAngle);
			velocity.AddRotationY(Rad);
		}
		if (randomMove.is) {
			velocity += velocity.Normal() * 10;
		}
		randomMove.Intervel();
	}
}