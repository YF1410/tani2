#include "Enemy.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include <stdlib.h>
#include <time.h>
#include "Easing.h"

using namespace DirectX;
std::vector<Enemy*> Enemy::enemys;

Enemy::Enemy(XMFLOAT3 startPos):
	pos(startPos),
	state(STAY),
	isAlive(true)
{
	//初期化
	enemyObj = FbxObject3d::Create(ModelManager::GetIns()->GetModel(ENEMY));
	//coreObj->SetScale({ 50.0f ,50.0f,50.0f });
	//sphereObj->SetScale({ 360.0f ,360.0f,360.0f });

	scale = 1.0f;
	HP = 3;
	minTargetLength = holmingLength;
	//乱数初期化
	srand(time(NULL));
	//当たり判定セット
	UpdateCollider();
}

Enemy::~Enemy() {
}

void Enemy::Update() {
	//移動量初期化
	moveVec = { 0,0,0 };

	//State別処理
	switch (state)
	{
	case Enemy::NOUPDATE:
		//画面外にいるときは更新しない
		return;
		break;
	case Enemy::STAY:		//待機
		//待機時間を経過させる
		stayTime++;
		
		if (isPlayerContact) {		//待機時間に関係なくプレイヤーを察知していればHOMINGへ以降
			state = HOMING;
		}
		else if(stayTime >= maxStayTime) {
			//角度をランダムで決定
			moveRad = XMConvertToRadians(static_cast<float>(rand() % 360));
			stayTime = 0;
			state = WANDERING;
		}

		break;

	case Enemy::WANDERING:	//うろうろする
		//移動時間を経過させる
		moveTime++;
		
		//ランダムな方向へと移動する
		moveVec.x += cos(moveRad) * moveSpeed;
		moveVec.z += sin(moveRad) * moveSpeed;

		//一定時間移動したらSATYに移行
		if (moveTime >= maxMoveTime) {
			moveTime = 0;
			state = STAY;
		}
		break;
	case Enemy::HOMING:
		//ターゲットが消滅していた場合や追跡範囲を外れたときはSTAYに移行
		if (targetLength >= holmingLength) {
			minTargetLength = holmingLength;
			state = STAY;

		}
		else {//if(ここにrayがオブジェクトに届けばの条件式を書く){
			targetVec = Vector3(targetPos - pos);
			targetVec.y = 0;
			moveVec += targetVec.Normalize() * moveSpeed;
		}

		if (targetLength <= attackLength) {
			state = ATTACK;
		}

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
			scale = Ease(In,Back, (float)(InvincibleTimer / 20.0f), 1.0f, 0.7f);
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

	enemyObj->Update();
	UpdateCollider();
}

void Enemy::Adaptation()
{
	//描画位置決定
	pos = afterPos;
	
	enemyObj->SetScale(scale);
	enemyObj->SetPosition(pos);
	enemyObj->Update();
}

void Enemy::Draw() {
	enemyObj->Draw((DirectXCommon::GetInstance()->GetCommandList()));
}

void Enemy::StaticUpdate() {

	//削除
	for (int i = enemys.size() - 1; i >= 0; i--) {
		if (!enemys[i]->isAlive) {
			delete enemys[i];
			enemys.erase(enemys.begin() + i);
		}
	}
	//更新
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Update();
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

void Enemy::UpdateCollider()
{
	//移動後の位置予測
	afterPos = pos + moveVec;

	//索敵範囲用判定
	collider.searchArea.center = afterPos;
	collider.searchArea.radius = sarchLength;
	//当たり判定
	collider.hitSphere.center = afterPos + Vector3{0,20,0};
	collider.hitSphere.radius = 20.0f;
	//攻撃判定
	collider.attackSphere.center = afterPos;
	collider.attackSphere.radius = 20.0f;
	//見た目に近い判定
	collider.realSphere.center = afterPos;
	collider.realSphere.radius = 20.0f;
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

void Enemy::HomingObjectCheck(Vector3 targetPos)
{
	this->targetPos = targetPos;
	state = HOMING;
}
