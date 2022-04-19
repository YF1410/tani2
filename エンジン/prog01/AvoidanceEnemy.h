#pragma once
#include "Enemy.h"
class AvoidanceEnemy :
	public Enemy
{
public:
	AvoidanceEnemy(XMFLOAT3 startPos, PlayerObject *targetPos);
	void Move() override;
private:
	//回避用ステート
	STATE avoidance;
	//回避状開始時のプレイヤー位置
	Vector3 avoidancePos;
	//回避範囲
	float avoidanceRange;
};

