#include "Enemy.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include <stdlib.h>
#include <time.h>

using namespace DirectX;
std::vector<Enemy*> Enemy::enemys;

Enemy::Enemy() {

	// 初期化
	enemyObj = FbxObject3d::Create(ModelManager::GetIns()->GetModel(ENEMY));
	enemyObj->SetPosition({ 700,100,200 });
	////エネミーのコア
	//coreModel = Model::CreateFromObject("chr_sword");
	//coreObj = Object3d::Create(coreModel.get());
	//coreObj->SetPosition({ 700,100,200 });
	//当たり判定可視化
	sphereModel = Model::CreateFromObject("sphere", true);
	sphereModel->SetAlpha(0.2f);
	sphereObj = Object3d::Create(sphereModel.get());
	sphereObj->SetPosition({ 700,100,200 });

	//coreObj->SetScale({ 50.0f ,50.0f,50.0f });
	sphereObj->SetScale({ 360.0f ,360.0f,360.0f });

	pos = enemyObj->GetPos();
	//乱数初期化
	srand(time(NULL));
}

Enemy::~Enemy() {
}

void Enemy::Update() {

	//うろうろする時間を加算してカウント
	if (isPlayerContact == false) {
		if (wanderingCount == 0) {
			sphereObj->SetScale({ 360.0f ,360.0f,360.0f });
			isWandering = true;
			wanderingCount += 1;
			angle = static_cast<float>(rand() % 360);
		}
	}

	if (isWandering == true) {
		moveTime += 1;
	}

	//プレイヤーを見つけていなければ一定時間うろうろ
	if (moveTime <= maxMoveTime && isWandering == true) {
		enemyMove();
	}
	//見つけていれば時間関係無しに追いかけ続ける
	if (isPlayerContact == true) {
		sphereObj->SetScale({ 580.0f ,580.0f,580.0f });
		isWandering = false;
		wanderingCount = 0;
		moveTime = maxMoveTime + 1;
		enemyMove();
	}

	if (moveTime >= 300) {
		moveTime = 0;
		wanderingCount = 0;
	}

	enemyObj->Update();
	sphereObj->Update();
}

void Enemy::Reflection() {
	//描画位置決定
	pos = afterPos;
	enemyObj->SetPosition(pos);
	enemyObj->SetScale({ 50.0f ,50.0f,50.0f });
	//全て適応
	enemyObj->Update();
}

void Enemy::Draw() {
	enemyObj->Draw((DirectXCommon::GetInstance()->GetCommandList()));
	//sphereObj->Draw();
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

void Enemy::StaticReflection() {
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Reflection();
	}
}

void Enemy::StaticDraw() {
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw();
	}
}

void Enemy::enemyMove() {
	float rad = XMConvertToRadians(angle);

	if (isPlayerContact == false) {
		pos.x += cos(rad) * speed;
		pos.z += sin(rad) * speed;
	}
	else if (isPlayerContact == true) {
		pos.x += cos(angle) * speed;
		pos.z += sin(angle) * speed;
	}

	enemyObj->SetPosition(pos);
	sphereObj->SetPosition({ pos.x, pos.y, pos.z });
}