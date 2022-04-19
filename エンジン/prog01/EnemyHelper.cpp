#include "EnemyHelper.h"

EnemyHelper::EnemyHelper(Vector3 startPos, Vector3 startVec, Vector3 *targetPos, ModelManager::ModelName modelName):
	GameObjCommon(
		modelName,
		GameObjCommon::ENEMY,
		false,
		startPos,
		{ 1,1,1 },
		{ 0,0,0 },
		true
	){
	this->targetPos = targetPos;
	velocity = startVec;
	isAlive = true;

}
