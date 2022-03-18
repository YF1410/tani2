#include "Enemy.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>

using namespace DirectX;

Enemy::Enemy() {
	//�G�l�~�[�̃R�A
	coreModel = Model::CreateFromObject("chr_sword");
	coreObj = Object3d::Create(coreModel.get());
	coreObj->SetPosition({ 700,0,200 });
	//�����蔻�����
	sphereModel = Model::CreateFromObject("sphere", true);
	sphereModel->SetAlpha(0.5f);
	sphereObj = Object3d::Create(sphereModel.get());
	sphereObj->SetPosition({ 700,0,200 });

	coreObj->SetScale({ 50.0f ,50.0f,50.0f });
	sphereObj->SetScale({ 300.0f ,300.0f,300.0f });

	enemyPos = coreObj->GetPosition();
	//����������
	srand(time(NULL));
}

Enemy::~Enemy() {
}

void Enemy::Update() {
	Input* input = Input::GetInstance();

	//����time�Ǘ�
	//if (input->TriggerKey(DIK_SPACE)) {
	//	//�v���C���[�Ƃ̋��������G�͈͂��Z����΂��낤�낷��ׂ�angle�ɗ������
	//	if (playerContact == false) {
	//	angle = static_cast<float>(rand() % 360);
	//	moveTime = 0;
	//	playerContact = false;
	//	}
	//	//���G�͈͂��v���C���[�Ƃ̋������Z����΂�����Ɉړ�����悤��angle�ɒl����
	//	else if (playerContact == true) {
	//		angle = atan2(playerPosy - coreObj->GetPosition().y, playerPosx - coreObj->GetPosition().x);
	//		playerContact = true;
	//	}
	//}

	//���낤�낷�鎞�Ԃ����Z���ăJ�E���g
	if (isPlayerContact == false) {
		if (wanderingCount == 0) {
			isWandering = true;
			wanderingCount += 1;
			angle = static_cast<float>(rand() % 360);
		}
	}

	if (isWandering == true) {
		moveTime += 1;
	}

	//�v���C���[�������Ă��Ȃ���Έ�莞�Ԃ��낤��
	if (moveTime <= maxMoveTime && isWandering == true) {
		enemyMove();
	}
	//�����Ă���Ύ��Ԋ֌W�����ɒǂ�����������
	if (isPlayerContact == true) {
		isWandering = false;
		wanderingCount = 0;
		moveTime = maxMoveTime + 1;
		enemyMove();
	}

	if (moveTime >= 300) {
		moveTime = 0;
		wanderingCount = 0;
	}

	coreObj->Update();
	sphereObj->Update();
}

void Enemy::Draw() {
	coreObj->Draw();
	sphereObj->Draw();
}

void Enemy::enemyMove() {
	float rad = XMConvertToRadians(angle);

	if (isPlayerContact == false) {
		enemyPos.x += cos(rad) * speed;
		enemyPos.z += sin(rad) * speed;
	}
	else if (isPlayerContact == true) {
		enemyPos.x += cos(angle) * speed;
		enemyPos.z += sin(angle) * speed;
	}

	coreObj->SetPosition(enemyPos);
	sphereObj->SetPosition({ enemyPos.x, enemyPos.y + 30, enemyPos.z });
}