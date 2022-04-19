#pragma once
#include "GameObjCommon.h"
#include "Vector3.h"
#include <vector>
class EnemyHelper :
	public GameObjCommon
{
public:
	EnemyHelper(Vector3 startPos, Vector3 startVec, Vector3 *targetPos,ModelManager::ModelName modelName);
	virtual void Update() override = 0;
	virtual void LustUpdate() override {};
	virtual void OnCollision(const CollisionInfo &info) override {};

	bool isAlive;
protected:
	Vector3 *targetPos;

};

