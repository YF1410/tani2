#include "Enemy.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>

using namespace DirectX;

Enemy::Enemy() {
	//�G�l�~�[�̃R�A
	coreModel = Model::CreateFromObject("chr_sword");
	coreObj = Object3d::Create(coreModel.get());
	//�����蔻�����
	sphereModel = Model::CreateFromObject("sphere", true);
	sphereModel->SetAlpha(0.5f);
	sphereObj = Object3d::Create(sphereModel.get());
	sphereObj->SetPosition({ 0, 0.6f, 0 });
	//����������
	srand(time(NULL));
}

Enemy::~Enemy() {
}

void Enemy::Update() {
	Input* input = Input::GetInstance();

	//����time�Ǘ�
	if (input->TriggerKey(DIK_SPACE)) {
		//�v���C���[�Ƃ̋��������G�͈͂��Z����΂��낤�낷��ׂ�angle�ɗ������
		//if (searchPlayerLen <= ) {
		srand(time(NULL));
			angle = static_cast<float>(rand() % 360);
			moveTime = 0;
			playerContact = false;
		//}
		//���G�͈͂��v���C���[�Ƃ̋������Z����΂�����Ɉړ�����悤��angle�ɒl����
		/*else if (searchPlayerLen >= ) {
			angle = atan2(playerPosy - coreObj->GetPosition().y, playerPosx - coreObj->GetPosition().x);
			playerContact = true;
		}*/
	}

	//���낤�낷�鎞�Ԃ����Z���ăJ�E���g
	moveTime += 1;

	//�v���C���[�������Ă��Ȃ���Έ�莞�Ԃ��낤��
	if (moveTime <= maxMoveTime && playerContact == false) {
		enemyMove();
	}
	//�����Ă���Ύ��Ԋ֌W�����ɒǂ�����������
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