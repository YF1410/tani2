#include "BoundEnemy.h"
#include "yMath.h"

BoundEnemy::BoundEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos,targetPos){
}

void BoundEnemy::OnCollision(const CollisionInfo &info)
{

	//ダイナミックキャスト用
	PlayerObject *player;
	Enemy *enemy;
	Debris *debri;

	//衝突したコライダーで分岐
	switch (info.object->Tag)
	{
	case PLAYER:
		player = dynamic_cast<PlayerObject *>(info.object);
		//位置修正
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
		penalty.y = 0;

		//プレイヤーが攻撃状態なら
		if (player->attack.is) {
			//ダメージを受ける
			Damage(1.0f);
			
			////衝突軸ベクトル
			//Vector3 normal = pos - player->pos;
			//normal.Normalize();
			////内積
			//float dot = Vector3(player->pos - pos).VDot(normal);
			////定数ベクトル
			//Vector3 constVec = 2 * dot / 2 * normal;

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

void BoundEnemy::Damage(float damage)
{
	//無敵時間中は処理を中断
	if (isInvincible) { return; }
	//ダメージを受ける
	HP -= damage;
	//無敵時間をセットする
	isInvincible = true;
	InvincibleTimer = 0;
	velocity.z *= -10;
}
