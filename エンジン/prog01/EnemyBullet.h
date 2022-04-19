#pragma once
#include "EnemyHelper.h"

class EnemyBullet:
	public EnemyHelper
{
public:
	EnemyBullet(Vector3 startPos, Vector3 startVec, Vector3 *targetPos, ModelManager::ModelName modelName);
	void Update() override;
};

