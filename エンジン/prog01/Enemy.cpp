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
std::vector<Enemy*> Enemy::enemys;

Enemy::Enemy(XMFLOAT3 startPos,PlayerObject *player) :
	GameObjCommon(
		ModelManager::ENEMY,	//エネミーモデルをセット
		GameObjCommon::ENEMY,	//エネミーとして扱う
		false,					//重力の影響を受ける
		startPos				//初期位置をセット
	)
{
	isAlive = true;
	scale = 1.0f;
	HP = 3;
	minTargetLength = holmingLength;
	state = HOMING;
	this->player = player;
	//乱数初期化
	srand(time(NULL));
	//当たり判定初期化
	float radius = 100;
	SetBroadCollider(new SphereCollider("hitCollider",XMVECTOR{ 0,radius,0 }, radius));


	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);

}


void Enemy::Initialize()
{
}

void Enemy::Update() {
	//移動量初期化
	velocity = { 0,0,0 };

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
		//if (targetLength >= holmingLength) {
		//	minTargetLength = holmingLength;
		//	state = STAY;

		//}
		//else {
		targetVec = Vector3(player->GetPos() - pos);
		targetVec.y = 0;
		velocity += targetVec.Normal() * moveSpeed;
		//}
		break;
	case Enemy::ATTACK:
		if (true) {
			state = STAY;
		}
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
			scale = Ease(In, Back, (float)(InvincibleTimer / 20.0f), 1.0f, 0.7f);
		}
		if (20 < InvincibleTimer && InvincibleTimer <= 40) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 20.0f) / 20.0f), 0.7f, 1.2f);
		}
		if (40 < InvincibleTimer && InvincibleTimer <= 60) {
			scale = Ease(Out, Bounce, (float)((InvincibleTimer - 40.0f) / 20.0f), 1.2f, 1.0f);
		}

		//タイマーが30になったら無敵を解除
		if (InvincibleTimer >= 60) {
			isInvincible = false;
		}
	}
	Move();

	//マップチップとの当たり判定
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	if (MapChip::GetInstance()->CheckHitMapChip(toMapChipCollider, &velocity, &hitPos)) {
		Vector3 normal = { 0,0,0 };

		if (hitPos.x != 0) {
			int vec = 1;	//向き
			if (0 < velocity.x) {
				vec = -1;
			}
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * vec;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//向き
			if (velocity.z < 0) {
				vec = -1;
			}
			pos.z = hitPos.z - toMapChipCollider->GetRadiusY() * vec;
			normal.z = vec;
		}
		normal.Normalize();
		HitWall(hitPos, normal);
	}

}

void Enemy::OnCollision(const CollisionInfo &info)
{
	Debris *debri;
	switch (info.object->Tag)
	{
	case PLAYER:
		if (true) {

		}
		break;
	case DEBRIS:
 		debri = dynamic_cast<Debris *>(info.object);
		if (debri->isAttack) {
			Damage(1.0f);
		}
		break;
	default:
		break;
	}
}

void Enemy::StaticAdaptation() {
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Adaptation();
	}
}

void Enemy::StaticDraw() {
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw();
	}
}

void Enemy::Damage(float damage)
{
	//無敵時間中は処理を中断
	if (isInvincible) { return; }
	//ダメージを受ける
	HP -= damage;
	//HPが0以下になったら死亡状態へ以降
	if (HP < 0) {
		state = DEAD;
	}
	else{
		//無敵時間をセットする
		isInvincible = true;
		InvincibleTimer = 0;
	}
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
