#include "CushionEnemy.h"
#include "Debris.h"


CushionEnemy::CushionEnemy(XMFLOAT3 startPos, PlayerObject *targetPos) :
	Enemy(startPos, targetPos) {
	moveSpeed = 2.0f;
	//objectData.get()->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SLIME_BREAK));
}

void CushionEnemy::OnCollision(const CollisionInfo &info)
{
	//ダイナミックキャスト用
	Debris *debri;
	PlayerObject *player;
	Enemy *enemy;

	//衝突したコライダーで分岐
	switch (info.object->Tag)
	{
	case PLAYER:
		player = dynamic_cast<PlayerObject *>(info.object);
		player->velocity *= 0.1f;
		//位置修正
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
		penalty.y = 0;

		//プレイヤーが攻撃状態なら
		if (player->attack.is) {
			//ダメージを受ける
			Damage(1.0f);

			//衝突軸ベクトル
			Vector3 normal = pos - player->pos;
			normal.Normalize();
			//内積
			float dot = Vector3(player->pos - pos).VDot(normal);
			//定数ベクトル
			Vector3 constVec = 2 * dot / 2 * normal;

			velocity = 2 * constVec + velocity;

		}
		break;
	case DEBRIS:
		//デブリが攻撃状態ならダメージを受ける
		debri = dynamic_cast<Debris *>(info.object);
		if (debri->isAttack) {
			Damage(1.0f);
		}
		break;

	case ENEMY:
		//エネミー同士なら押し返しあう
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.2f;
		break;
	default:
		break;
	}

	penalty.y = 0;
	pos += penalty;
	GameObjCommon::Update();
}

void CushionEnemy::Move()
{
}
