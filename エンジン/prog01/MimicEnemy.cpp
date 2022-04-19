#include "MimicEnemy.h"
#include "Easing.h"
#include "Debris.h"

MimicEnemy::MimicEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos, targetPos) {
	objectData.get()->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SLIME_BREAK));
}

void MimicEnemy::Move()
{
}

