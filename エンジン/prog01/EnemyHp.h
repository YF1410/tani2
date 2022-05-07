#pragma once
#include "GameObjCommon.h"
#include "State.h"

class EnemyHp:
	public GameObjCommon
{
public :
	EnemyHp(float &nowHp, float &maxHp, Vector3 &pos);
	void Update();
	void Draw();
	STATE HpDraw;
private:
	float *nowHp;
	float *maxHp;
	Vector3 *pos_p;
	std::unique_ptr<GameObjCommon> gage;

};

