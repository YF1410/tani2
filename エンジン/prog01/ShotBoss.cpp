#include "ShotBoss.h"
#include "Easing.h"
#include "Collision.h"
#include "EnemyHelperManager.h"
#include "EnemyBullet.h"

ShotBoss::ShotBoss(XMFLOAT3 startPos, PlayerObject *targetPos,Camera *cam) :
	Enemy(Vector3(Vector3(0,2000,0)+ Vector3(startPos)), targetPos) {
	objectData.get()->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SLIME));
	HP = 1000;
	maxHP = HP;
	hpBer->offset = { 0,100,-1000 };
	defScale = 5.0f;
	scale = defScale;
	isBounce = false;


	this->startPos =startPos;
	this->startPos.y = 120;
	isSpawn = true;
	this->cam = cam;

	//当たり判定変更
	float radius = 800;
	broadSphereCollider->SetOffset({ 0,radius,0 });
	broadSphereCollider->SetRadius(radius);
	toMapChipCollider->SetRadius(radius, radius);


	core = std::make_unique<GameObjCommon>(ModelManager::SLIME_CORE, ENEMY, false, Vector3{ 0,0,0 }, Vector3{ 2,2,2 });
	core.get()->scale = 5.0f;
	offset = { 0,50,0 };
	coreBasePos = pos + offset;
	spinVec = { 1,0,0 };


	coreCollider = new SphereCollider("BroadSphere", XMVECTOR{ 0,100,0 }, 100);
	core.get()->SetNarrowCollider(coreCollider);
	core.get()->Adaptation();

	//攻撃頻度
	attack.interval = 20;
}

void ShotBoss::Update()
{

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

	//スポーン直後は演出をする
	if (isSpawn) {
		if (pos.y < startPos.y) {
			pos.y = startPos.y;
			if (startTimer < 60) {
				spinVec.AddRotationY(Ease(In, Quart, float(startTimer / 30.0f), 0, 2.0f));
			}
			else {
				spinVec.AddRotationY(Ease(Out, Quad, float((startTimer - 30) / 30.0f), 2.0f, 0.05f));
			}
			core.get()->pos = coreBasePos + spinVec * spinR * scale.x;
			startTimer++;
			cam->SetTarget(pos);

			if (startTimer >= 120) {
				isSpawn = false;
			}
		}
		else {
			velocity.y -= 0.9f;
			cam->SetEye(Vector3(startPos+ Vector3(0, 100, -2500)));
			cam->SetTarget(pos);
		}
	}
	else{
		//通常時処理（条件式があればフラグで管理する）
		Move();
	}
	//共通処理
	//無敵時間タイマーを管理
	if (isInvincible) {
		InvincibleTimer++;
		//ここは後でアニメーションに変更する
		if (InvincibleTimer <= 10) {
			scale = Ease(In, Back, (float)(InvincibleTimer / 10.0f), 1.0f, 1.5f) * defScale;
		}
		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP > 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), 1.5f, 1.0f) * defScale;
		}

		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP <= 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), 1.5f, 0.0f) * defScale;
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
				scale = defScale;
			}
		}
	}

	//攻撃インターバル処理
	attack.Intervel();
	//移動をいったん適応
	PosAddVelocity();
	coreBasePos = pos + offset;

}

void ShotBoss::Draw() const
{
	core.get()->Draw();
	Enemy::Draw();
}

void ShotBoss::Move()
{
	spinVec.AddRotationY(0.05f);
	core.get()->pos = coreBasePos + spinVec * spinR * scale.x;
	core.get()->Update();
	core.get()->Adaptation();

	//攻撃可能なら
	if (attack.can) {
		attack.Start();
		//射撃
		Vector3 shot = Vector3(core.get()->pos - pos).Normal() * 30.0f;
		shot.y = 0;
		EnemyHelperManager::enemyHelpers.push_back(
			new EnemyBullet(
				Vector3(core.get()->pos.x,0, core.get()->pos.z),
				shot,
				ModelManager::SHOTBOSS)
		);
	}
	attack.Intervel();


}

void ShotBoss::HitPlayer(const CollisionInfo &info)
{
	PlayerObject *player;
	player = dynamic_cast<PlayerObject *>(info.object);

	player->velocity += Vector3(info.reject).Normal() * player->velocity.Length() * -0.5f;
	player->velocity.y = 0.0f;
	//プレイヤーがコアと接触したらダメージ処理
	if(player->attack.is){
		if (Collision::CheckSphere2Sphere(
			*dynamic_cast<Sphere *>(player->broadSphereCollider),
			*dynamic_cast<Sphere *>(coreCollider))
			) {
			Damage(player->attackPow);
		}
	}
}

void ShotBoss::HitDebri(const CollisionInfo &info)
{
	Debris *debri;
	debri = dynamic_cast<Debris *>(info.object);

	debri->velocity += Vector3(info.reject).Normal() * debri->velocity.Length() * -0.5f;
	debri->velocity.y = 0.0f;
	//プレイヤーがコアと接触したらダメージ処理
	if (debri->state == Debris::ATTACK||
		debri->state == Debris::RETURN) {
		if (Collision::CheckSphere2Sphere(
			*dynamic_cast<Sphere *>(debri->attackCollider),
			*dynamic_cast<Sphere *>(coreCollider))
			) {
			//ダメージ
			Damage(debri->velocity.Length());
		}
	}
}
