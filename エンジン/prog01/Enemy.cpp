#include "Enemy.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include <stdlib.h>
#include <time.h>
#include "Easing.h"
#include "SlimeMath.h"
#include "MapChip.h"
#include "Audio.h"

using namespace DirectX;

Enemy::Enemy(XMFLOAT3 startPos, PlayerObject* player) :
	GameObjCommon(
		ModelManager::ENEMY_ROBO_5,	//エネミーモデルをセット
		GameObjCommon::ENEMY,	//エネミーとして扱う
		false,					//重力の影響を受ける
		Vector3(startPos.x, 2000, startPos.z)				//初期位置をセット
	) {
	isAlive = true;
	isSpawn = true;
	this->startPos = startPos;
	this->player = player;
	maxHP = 100.0f;
	HP = maxHP;
	isBounce = false;
	//HPbar生成
	hpBer = new EnemyHp(HP, maxHP, pos);


	defScale = 1.5f;
	scale = defScale;
	//当たり判定初期化
	float radius = 100;
	broadSphereCollider = new SphereCollider("BroadSphere", XMVECTOR{ 0,radius,0 }, radius);
	SetBroadCollider(broadSphereCollider);

	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, radius, radius);
	SetNarrowCollider(toMapChipCollider);

	spawnTimer = 0.0f;

	Initialize();
}

Enemy::~Enemy() {
	delete hpBer;
}


void Enemy::Initialize() {
	//攻撃関係
	attack = {
		true,
		false,
		40,
		0
	};
}

void Enemy::Update() {
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
	//ヒットストップ
	if (isHitStop) {
		velocity = 0;
	}
	Move();


	//共通処理


	if (isSpawn) {
		pos.y = Ease(In, Cubic, spawnTimer, 2000, startPos.y);
		spawnTimer += 0.02f;
		if (spawnTimer > 1.0f) {
			isSpawn = false;
			pos.y = startPos.y;
			spawnTimer = 1.0f;
		}
		objectData.get()->SetAlpha(spawnTimer);

	}

	//攻撃インターバル処理
	attack.Intervel();
	//移動をいったん適応
	PosAddVelocity();

	//ヒットストップ
	if (isHitStop) {
		hitStopTimer++;
		if (hitStopTimer >= 20) {
			hitStopTimer = 0;
			isHitStop = false;
		}
	}
	if (velocity.Length() <= 1.0f) {
		rotate.y = ConvertNormalToDeg(Vector3(player->GetPos() - pos).Normal(), Vector3{ 0,0,1 }).y + 180;
	}
	else {
		rotate.y = ConvertNormalToDeg(velocity.Normal(), Vector3{ 0,0,1 }).y + 180;
	}
}

void Enemy::LustUpdate() {
	//無敵時間タイマーを管理
	if (isInvincible) {
		InvincibleTimer++;

		//ダメージ受けた時のもわっとでかくなるやつここから
		//ここは後でアニメーションに変更する
		if (InvincibleTimer <= 10) {
			scale = Ease(In, Back, (float)(InvincibleTimer / 10.0f), defScale, defScale + 1.5f);
		}
		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP > 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), defScale + 1.5f, defScale);
		}

		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP <= 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), defScale + 1.5f, 0.0f);
		}
		//ここまで

		//タイマーが30になったら無敵を解除
		if (InvincibleTimer >= 30) {
			isInvincible = false;
			//HPが0以下になったら死亡状態へ以降
			if (HP < 1.0f) {
				isAlive = false;
				//一定の確率でアイテムドロップ
				if (rand() % 101 <= 30) {
					Debris::debris.push_back(new Debris(pos, { 0,0,0 }, 5));
				}
			}
			else {
				scale = defScale;
			}
		}
	}

	//マップチップとの当たり判定
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	Vector3 normal = { 0,0,0 };
	//上下
	if (MapChip::GetInstance()->CheckMapChipAreaToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal)) {
		if (hitPos.x != 0) {
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * normal.x;
		}
		if (hitPos.z != 0) {
			pos.z = hitPos.z + toMapChipCollider->GetRadiusY() * normal.z;
		}
		normal.Normalize();
		HitWall(hitPos, normal.Normal());

		//Reset
		hitPos = { 0,0,0 };
		moveVec = velocity + penalty;
		normal = { 0,0,0 };
	}
	else if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal, &oldPos)) {

		if (hitPos.x != 0) {
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * normal.x;
		}
		if (hitPos.z != 0) {
			pos.z = hitPos.z + toMapChipCollider->GetRadiusY() * normal.z;
		}
		normal.Normalize();
		HitWall(hitPos, normal.Normal());

	}


	hpBer->Update();
}

void Enemy::Draw() const
{
	GameObjCommon::Draw();
	hpBer->Draw();

}

void Enemy::OnCollision(const CollisionInfo& info)
{
	//ダイナミックキャスト用
	Enemy* enemy;

	//衝突したコライダーで分岐
	switch (info.object->Tag)
	{
	case PLAYER:
		HitPlayer(info);
		break;
	case DEBRIS:
		HitDebri(info);
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


void Enemy::Move()
{
	if (!isInvincible) {
		targetVec = Vector3(player->GetPos() - pos);
		targetVec.y = 0;
		velocity += targetVec.Normal() * moveSpeed;
	}
}

void Enemy::Attack()
{
	//攻撃可能なら
	if (attack.can) {
		attack.Start();
	}
}

void Enemy::Damage(float damage,bool isDebrisAttack)
{
	isHitStop = true;
	hpBer->HpDraw.Start();
	//無敵時間中は処理を中断
	if (isInvincible) { return; }
	hpBer->HpDraw.timer = hpBer->HpDraw.interval;

	if (isDebrisAttack) {
		Audio::GetInstance()->PlayWave(18, 0.5f);
	}

	//ダメージを受ける
	HP -= damage;
	if (HP <= 0) {
		HP = 0;
		if (isDebrisAttack) {
			isDead = true;
		}
	}
	//無敵時間をセットする
	isInvincible = true;
	InvincibleTimer = 0;
}

int Enemy::CauseDamage()
{
	Attack();
	return attackPow;
}

void Enemy::HitWall(const XMVECTOR& hitPos, const Vector3& normal)
{
	//反射
	velocity = CalcWallScratchVector(velocity, normal);

}

void Enemy::HitPlayer(const CollisionInfo& info)
{
	PlayerObject* player;

	player = dynamic_cast<PlayerObject*>(info.object);

	//プレイヤーが攻撃状態なら
	if (player->attack.is) {
		//ダメージを受ける
		Damage(player->attackPow);
	}
	else {
		//位置修正
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
		penalty.y = 0;
	}
}

void Enemy::HitDebri(const CollisionInfo& info)
{
	Debris* debri;
	//デブリが攻撃状態ならダメージを受ける
	debri = dynamic_cast<Debris*>(info.object);
	if (debri->isAttack) {
		//ヒットストップ
		debri->isHitStop = true;
		Damage(debri->velocity.Length(),true);
	}
}
