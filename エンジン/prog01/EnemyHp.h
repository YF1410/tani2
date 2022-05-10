#pragma once
#include "GameObjCommon.h"
#include "State.h"

class EnemyHp :
	public GameObjCommon
{
public:
	EnemyHp(float &nowHp, float &maxHp, Vector3 &pos, Vector3 offset = {0,100,-200});
	void Update();
	void Draw();
	STATE HpDraw;
	Vector3 offset;
private:
	float* nowHp;
	float* maxHp;
	Vector3* pos_p;
	std::unique_ptr<GameObjCommon> gage;
};

