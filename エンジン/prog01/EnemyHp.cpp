#include "EnemyHp.h"

EnemyHp::EnemyHp(float &nowHp, float &maxHp, Vector3 &pos) :
	GameObjCommon(ModelManager::ModelName::SLIME,Notag,false) {
	pos_p = &pos;
	this->nowHp = &nowHp;
	this->maxHp = &maxHp;
}

void EnemyHp::Update()
{
	if (isInvisible) return;
	//ポインターに合わせて移動
	pos = *pos_p;
	pos.z -= 50.0f;
}


void EnemyHp::Draw()
{
	if (isInvisible) return;
	GameObjCommon::Draw();
}
