#include "BoundEnemy.h"
#include "yMath.h"

BoundEnemy::BoundEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos,targetPos){
	objectData.get()->SetModel(ModelManager::GetIns()->GetModel(ModelManager::ENEMY_ROBO_3));

}

void BoundEnemy::HitPlayer(const CollisionInfo &info)
{
	PlayerObject *player;
	player = dynamic_cast<PlayerObject *>(info.object);
	//位置修正
	penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
	penalty.y = 0;

	//プレイヤーが攻撃状態なら
	if (player->attack.is) {
		//ダメージを受ける
		Damage(player->attackPow);

		Vector3 nextPos = info.inter + Vector3(pos - info.object->pos).Normal() * (broadSphereCollider->GetRadius() /*+ player->broadSphereCollider->GetRadius()*/);
		nextPos.y = 0;
		velocity = CalcReflectVector(velocity, Vector3(pos - player->pos).Normal())* 100.0f;
		velocity.y = 0;
		pos = nextPos;
	}
	else {
		//位置修正
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
		penalty.y = 0;
	}
}
