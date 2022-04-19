#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(Vector3 startPos, Vector3 startVec, Vector3 *targetPos, ModelManager::ModelName modelName):
	EnemyHelper(startPos, startVec,targetPos,modelName) {
}

void EnemyBullet::Update()
{
	velocity.y = 0;
	pos += velocity;
	GameObjCommon::Update();
	GameObjCommon::Adaptation();
}
