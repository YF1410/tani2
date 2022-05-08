#include "EnemyHp.h"

EnemyHp::EnemyHp(float &nowHp, float &maxHp, Vector3 &pos):
	GameObjCommon(ModelManager::ENEMY_HP_FRAME, Notag, false, { 0,0,0 }, { 1,1,1 }, { 0,0,0 }, false, true)
{
	pos_p = &pos;
	this->nowHp = &nowHp;
	this->maxHp = &maxHp;
	gage = std::make_unique<GameObjCommon>(ModelManager::ENEMY_HP_GAGE, Notag, false, Vector3{0,0,0}, Vector3{ 1,1,1 }, Vector3{ 0,0,0 }, false, true);
	HpDraw = {
		true,
		false,
		120,
		0
	};
}

void EnemyHp::Update()
{
	if (HpDraw.is) {
		//ポインターに合わせて移動
		pos = *pos_p;
		pos.y = 100;
		pos.z -= 200;
		gage.get()->pos = pos;
		//HP変動
		float HpScale = *nowHp / *maxHp;
		gage.get()->scale.x = HpScale;
		gage.get()->pos.x -= (1.0f - HpScale) * 250.0f;

		//インターバル
		HpDraw.Intervel();
		if (HpDraw.can) {
			HpDraw.is = false;
		}

		/*if (HpDraw.timer <= 60) {
			objectData.get()->SetAlpha((float)(HpDraw.timer / 60.0f));
			gage.get()->GetObjectDate()->SetAlpha((float)(HpDraw.timer / 60.0f));
		}*/

		GameObjCommon::Update();
		GameObjCommon::Adaptation();
		gage.get()->Update();
		gage.get()->Adaptation();
	}
}

void EnemyHp::Draw()
{
	if (HpDraw.is) {
		GameObjCommon::Draw();
		gage.get()->Draw();
	}
}
