#include "BoundEnemy.h"
#include "Easing.h"
#include "yMath.h"

BoundEnemy::BoundEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos,targetPos){
	objectData.get()->SetModel(ModelManager::GetIns()->GetModel(ModelManager::ENEMY_ROBO_3));
	scale = { 1.5f,1.5f,1.5f };
	startScale = scale.x;
}

void BoundEnemy::Update() {
	//旧ポジション
	oldPos = pos;
	//移動量初期化
	VelocityReset(0.95f);
	if (!isInvincible && velocity.Length() > maxMoveSpeed) {
		//最高速度を超えていたら制限する
		velocity = velocity.Normal() * maxMoveSpeed;
	}
	if (isInvincible && velocity.Length() > maxMoveSpeed * 10.0f) {
		velocity = velocity.Normal() * maxMoveSpeed * 4.0f;

	}
	penalty = { 0,0,0 };

	//通常時処理（条件式があればフラグで管理する）
	if (isHitStop) {
		velocity = 0;
	}
	Move();


	//共通処理
	//無敵時間タイマーを管理
	if (isInvincible) {
		InvincibleTimer++;

		//ダメージ受けた時のもわっとでかくなるやつここから
		/*if (InvincibleTimer <= 10) {
			scale = Ease(In, Back, (float)(InvincibleTimer / 10.0f), 1.0f, 3.0f) * defScale;
		}
		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP > 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), 3.0f, 1.0f) * defScale;
		}

		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP <= 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), 3.0f, 0.0f) * defScale;
		}*/
		//ここまで
		
		//タイマーが30になったら無敵を解除
		if (InvincibleTimer >= 30) {
			isInvincible = false;
			//HPが0以下になったら死亡状態へ以降
			if (HP <= 0) {
				isAlive = false;
				//一定の確率でアイテムドロップ
				if (rand() % 101 <= 30) {
					Debris::debris.push_back(new Debris(pos, { 0,0,0 }, 5));
				}
			}
			else {
				scale = 1.5f;
			}
		}
	}

	//攻撃インターバル処理
	attack.Intervel();
	//移動をいったん適応
	PosAddVelocity();

	if (isHitStop) {
		hitStopTimer++;
		if (hitStopTimer >= 20) {
			hitStopTimer = 0;
			isHitStop = false;
		}
	}
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
