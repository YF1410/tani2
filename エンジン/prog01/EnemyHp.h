#pragma once
#include "GameObjCommon.h"
class EnemyHp :
	public GameObjCommon
{
public :
	EnemyHp(float &nowHp, float &maxHp, Vector3 &pos);
	void Update();
	void Draw();

private:
	float *nowHp;
	float *maxHp;
	Vector3 *pos_p;
};

