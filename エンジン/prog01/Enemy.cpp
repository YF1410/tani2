#include "Enemy.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include <stdlib.h>
#include <time.h>
#include "Easing.h"
#include "SlimeMath.h"
#include "MapChip.h"

using namespace DirectX;

Enemy::Enemy(XMFLOAT3 startPos,PlayerObject *player) :
	GameObjCommon(
		ModelManager::ENEMY_MACARON,	//エネミーモデルをセット
		GameObjCommon::ENEMY,	//エネミーとして扱う
		false,					//重力の影響を受ける
		startPos				//初期位置をセット
	)
{
	isAlive = true;
	scale = 1.0f;
	this->player = player;
	maxHP = 10.0f;
	
	//当たり判定初期化
	float radius = 100;
	broadSphereCollider = new SphereCollider("BroadSphere", XMVECTOR{ 0,radius,0 }, radius);
	SetBroadCollider(broadSphereCollider);

	pushBackCollider = new SphereCollider("pushBackCollider", XMVECTOR{ 0,radius,0 }, radius);
	SetNarrowCollider(pushBackCollider);

	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);

	Initialize();
}

Enemy::~Enemy()
{
}


void Enemy::Initialize()
{
	//攻撃関係
	attack = {
		true,
		false,
		20,
		0
	};
	HP = maxHP;

}

void Enemy::Update() {
	//移動量初期化
	if (velocity.Length() > maxMoveSpeed) {
		//最高速度を超えていたら制限する
		velocity = velocity.Normal() * maxMoveSpeed;
	}
	else {
		//毎フレーム持っている移動量が0.9倍になる
		VelocityReset(0.9f);
	}
	//押し返し初期化
	penalty = { 0,0,0 };

	//通常時処理（条件式があればフラグで管理する）
	Move();

	//共通処理
	//無敵時間タイマーを管理
	if (isInvincible) {
		InvincibleTimer++;
		//ここは後でアニメーションに変更する
		if (InvincibleTimer <= 10) {
			scale = Ease(In, Back, (float)(InvincibleTimer / 10.0f), 1.0f, 3.0f);
		}
		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP > 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 30.0f), 3.0f  , 1.0f  );
		}

		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP <= 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 30.0f), 3.0f, 0.0f);
		}
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
				scale = 1.0f;
			}
		}
	}

	//攻撃インターバル処理
	attack.Intervel();
	//移動をいったん適応
	PosAddVelocity();

}

void Enemy::LustUpdate()
{

	//マップチップとの当たり判定
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	//上下左右
	if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos)) {
		Vector3 normal = { 0,0,0 };

		if (hitPos.x != 0) {
			int vec = 1;	//向き
			if (0 < moveVec.x) {
				vec = -1;
			}
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * vec;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//向き
			if (moveVec.z < 0) {
				vec = -1;
			}
			pos.z = hitPos.z - toMapChipCollider->GetRadiusY() * vec;
			normal.z = vec;
		}
		normal.Normalize();
		HitWall(hitPos, normal);
	}
	//角
	else if (MapChip::GetInstance()->CheckMapChipToSphere2d(broadSphereCollider, &velocity, &hitPos)) {
		Vector3 normal = { 0,0,0 };
		if (hitPos.x != 0) {
			int vec = 1;	//向き
			if (0 < velocity.x) {
				vec = -1;
			}
			pos.x = hitPos.x;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//向き
			if (velocity.z < 0) {
				vec = -1;
			}
			pos.z = hitPos.z;
			normal.z = vec;
		}
		normal.Normalize();
		velocity = CalcWallScratchVector(velocity, normal);
	}

}

void Enemy::OnCollision(const CollisionInfo &info)
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
		//位置修正
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
		penalty.y = 0;

		//プレイヤーが攻撃状態なら
		if (player->attack.is) {
			//ダメージを受ける
			Damage(player->attackPow);

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
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length()*0.2f;
		break;
	default:
		break;
	}

	penalty.y = 0;
	pos += penalty;
	GameObjCommon::Update();

}


void Enemy::Move()
{
	targetVec = Vector3(player->GetPos() - pos);
	targetVec.y = 0;
	velocity += targetVec.Normal() * moveSpeed;
}

void Enemy::Attack()
{
	//攻撃可能なら
	if (attack.can) {
		attack.Start();
	}
}

void Enemy::Damage(float damage)
{
	//無敵時間中は処理を中断
	if (isInvincible) { return; }
	//ダメージを受ける
	HP -= damage;
	//無敵時間をセットする
	isInvincible = true;
	InvincibleTimer = 0;
}

int Enemy::CauseDamage()
{
	Attack();
	return attackPow;
}

void Enemy::HitWall(const XMVECTOR &hitPos, const Vector3 &normal)
{
	//反射
	velocity = CalcWallScratchVector(velocity, normal);

}
