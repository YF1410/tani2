#include "Enemy.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include <stdlib.h>
#include <time.h>
#include "Easing.h"
#include "Debris.h"
#include "SlimeMath.h"
#include "MapChip.h"

using namespace DirectX;

Enemy::Enemy(XMFLOAT3 startPos,PlayerObject *player) :
	GameObjCommon(
		ModelManager::ENEMY,	//エネミーモデルをセット
		GameObjCommon::ENEMY,	//エネミーとして扱う
		false,					//重力の影響を受ける
		startPos				//初期位置をセット
	)
{
	isAlive = true;
	scale = 20.0f;
	minTargetLength = holmingLength;
	this->player = player;
	state = HOMING;
	maxHP = 1;
	
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
		velocity = velocity.Normal() * maxMoveSpeed;
	}
	else {
		VelocityReset(0.9f);
	}

	//State別処理
	switch (state)
	{
	case Enemy::NOUPDATE:
		//画面外にいるときは更新しない
		return;
		break;
	case Enemy::STAY:		//待機
		////待機時間を経過させる
		//stayTime++;
		//if (isPlayerContact) {		//待機時間に関係なくプレイヤーを察知していればHOMINGへ以降
		//	state = HOMING;
		//}
		//else if (stayTime >= maxStayTime) {
		//	//角度をランダムで決定
		//	moveRad = XMConvertToRadians(static_cast<float>(rand() % 360));
		//	stayTime = 0;
		//	state = WANDERING;
		//}
		break;

	case Enemy::WANDERING:	//うろうろする
		////移動時間を経過させる
		//moveTime++;
		////ランダムな方向へと移動する
		//velocity.x += cos(moveRad) * moveSpeed;
		//velocity.z += sin(moveRad) * moveSpeed;
		////一定時間移動したらSATYに移行
		//if (moveTime >= maxMoveTime) {
		//	moveTime = 0;
		//	state = STAY;
		//}
		break;
	case Enemy::HOMING:
		//ターゲットが消滅していた場合や追跡範囲を外れたときはSTAYに移行

		/*if (targetLength >= holmingLength) {
			minTargetLength = holmingLength;
			state = STAY;

		}*/
		//else {//if(ここにrayがオブジェクトに届けばの条件式を書く){
			targetVec = Vector3(player->GetPos()- pos);
			targetVec.y = 0;
			velocity += targetVec.Normal() * moveSpeed;
		//}

		/*if (targetLength <= attackLength) {
			state = ATTACK;
		}*/

		break;
	case Enemy::DEAD:
		isAlive = false;

		break;
	default:
		break;
	}

	//共通処理
	//無敵時間タイマーを管理
	if (isInvincible) {
		InvincibleTimer++;
		if (InvincibleTimer <= 20) {
			scale = Ease(In, Back, (float)(InvincibleTimer / 20.0f), 1.0f * 20.0f, 0.7f * 20.0f);
		}
		if (20 < InvincibleTimer && InvincibleTimer <= 40) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 20.0f) / 20.0f), 0.7f * 20.0f, 1.2f * 20.0f);
		}
		if (40 < InvincibleTimer && InvincibleTimer <= 60) {
			scale = Ease(Out, Bounce, (float)((InvincibleTimer - 40.0f) / 20.0f), 1.2f * 20.0f, 1.0f * 2.0f);
		}

		//タイマーが60になったら無敵を解除
		if (InvincibleTimer >= 60) {
			isInvincible = false;
			//HPが0以下になったら死亡状態へ以降
			if (HP <= 0) {
				state = DEAD;
				//一定の確率でアイテムドロップ
				if (rand() % 101 <= 30) {
					Debris::debris.push_back(new Debris(pos, { 0,0,0 }, 5, false));
				}
			}
		}
	}

	//攻撃インターバル処理
	attack.Intervel();

	Move();

}

void Enemy::FinalUpdate()
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
	Debris *debri;
	PlayerObject *player;
	Enemy *enemy;

	Vector3 penalty = {0,0,0};
	switch (info.object->Tag)
	{
	case PLAYER:
		player = dynamic_cast<PlayerObject *>(info.object);
		//位置修正
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.2f;
		if (player->attack.is) {
			Damage(1.0f);
		}
		break;
	case DEBRIS:
 		debri = dynamic_cast<Debris *>(info.object);
		if (debri->isAttack) {
			Damage(1.0f);
		}
		break;

	case ENEMY:
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length()*0.2f;
		break;
	default:
		break;
	}

	penalty.y = 0;
	pos += penalty;
	GameObjCommon::Update();

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

int Enemy::Attack()
{
	attack.Start();
	return attackPow;
}

//void Enemy::HomingObjectCheck(Vector3 targetPos)
//{
//	this->targetPos = targetPos;
//	state = HOMING;
//}

void Enemy::HitWall(const XMVECTOR &hitPos, const Vector3 &normal)
{

	Vector3 HitPos = hitPos;
	//pos = HitPos + normal * (rect2d.Bottom - rect2d.Top);
	velocity = CalcWallScratchVector(velocity, normal);

}
