#include "EscapeEnemy.h"
#include "EnemyBullet.h"
#include "EnemyHelperManager.h"

EscapeEnemy::EscapeEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos,targetPos){
	homingRange = 1500;
	attack.interval = 180;
}

void EscapeEnemy::Move()
{
	if (Vector3(pos - player->pos).Length() <= homingRange) {
		homing = true;
	}
	else {
		homing = false;
	}

	if (!homing) {
		targetVec = Vector3(player->GetPos() - pos);
		targetVec.y = 0;
		velocity -= targetVec.Normal() * moveSpeed * 0.5f;
	}
	else {
		Attack();
	}
}

void EscapeEnemy::Attack()
{
	if (attack.can) {
		attack.Start();
		EnemyHelperManager::enemyHelpers.push_back(new EnemyBullet(pos, Vector3(player->GetPos() - pos).Normal() * 15, ModelManager::ModelName::SLIME));
	}
}
