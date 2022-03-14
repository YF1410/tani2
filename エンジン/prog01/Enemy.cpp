#include "Enemy.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>

using namespace DirectX;

Enemy::Enemy() {
	//エネミーのコア
	coreModel = Model::CreateFromObject("chr_sword");
	coreObj = Object3d::Create(coreModel.get());
	//当たり判定可視化
	sphereModel = Model::CreateFromObject("sphere", true);
	sphereModel->SetAlpha(0.5f);
	sphereObj = Object3d::Create(sphereModel.get());
	sphereObj->SetPosition({ 0, 0.6f, 0 });
	//乱数初期化
	srand(time(NULL));
}

Enemy::~Enemy() {
}

void Enemy::Update() {
	Input* input = Input::GetInstance();

	//今後time管理
	if (input->TriggerKey(DIK_SPACE)) {
		//プレイヤーとの距離より索敵範囲が短ければうろうろする為にangleに乱数代入
		//if (searchPlayerLen <= ) {
		srand(time(NULL));
			angle = static_cast<float>(rand() % 360);
			moveTime = 0;
			playerContact = false;
		//}
		//索敵範囲よりプレイヤーとの距離が短ければそちらに移動するようにangleに値を代入
		/*else if (searchPlayerLen >= ) {
			angle = atan2(playerPosy - coreObj->GetPosition().y, playerPosx - coreObj->GetPosition().x);
			playerContact = true;
		}*/
	}

	//うろうろする時間を加算してカウント
	moveTime += 1;

	//プレイヤーを見つけていなければ一定時間うろうろ
	if (moveTime <= maxMoveTime && playerContact == false) {
		enemyMove();
	}
	//見つけていれば時間関係無しに追いかけ続ける
	if (playerContact == true) {
		enemyMove();
	}

	coreObj->Update();
	sphereObj->Update();
}

void Enemy::Draw() {
	coreObj->Draw();
	sphereObj->Draw();
}

void Enemy::enemyMove() {
	float rad = XMConvertToRadians(angle);//(angle * 3.14159265359f / 180.0f) * -1;
	XMFLOAT3 enemyPos = coreObj->GetPosition();

	enemyPos.x += cos(rad) * speed;
	enemyPos.y += sin(rad) * speed;

	coreObj->SetPosition(enemyPos);
	sphereObj->SetPosition({ enemyPos.x, enemyPos.y + 0.6f, enemyPos.z });
}