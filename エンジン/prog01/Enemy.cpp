#include "Enemy.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include <stdlib.h>
#include <time.h>
#include "Easing.h"
#include "Debris.h"

using namespace DirectX;
std::vector<Enemy*> Enemy::enemys;

Enemy::Enemy(XMFLOAT3 startPos) :
	GameObjCommon(
		ModelManager::ENEMY,	//�G�l�~�[���f�����Z�b�g
		GameObjCommon::ENEMY,	//�G�l�~�[�Ƃ��Ĉ���
		false,					//�d�͂̉e�����󂯂�
		startPos				//�����ʒu���Z�b�g
	)
{
	isAlive = true;
	scale = 1.0f;
	HP = 3;
	minTargetLength = holmingLength;
	state = STAY;
	//����������
	srand(time(NULL));
	//�����蔻�菉����
	float radius = 100;
	SetCollider(new SphereCollider("hitCollider",XMVECTOR{ 0,radius,0 }, radius));
}


void Enemy::Initialize()
{
}

void Enemy::Update() {
	//�ړ��ʏ�����
	velocity = { 0,0,0 };

	//State�ʏ���
	switch (state)
	{
	case Enemy::NOUPDATE:
		//��ʊO�ɂ���Ƃ��͍X�V���Ȃ�
		return;
		break;
	case Enemy::STAY:		//�ҋ@
		//�ҋ@���Ԃ��o�߂�����
		stayTime++;

		if (isPlayerContact) {		//�ҋ@���ԂɊ֌W�Ȃ��v���C���[���@�m���Ă����HOMING�ֈȍ~
			state = HOMING;
		}
		else if (stayTime >= maxStayTime) {
			//�p�x�������_���Ō���
			moveRad = XMConvertToRadians(static_cast<float>(rand() % 360));
			stayTime = 0;
			state = WANDERING;
		}

		break;

	case Enemy::WANDERING:	//���낤�낷��
		//�ړ����Ԃ��o�߂�����
		moveTime++;

		//�����_���ȕ����ւƈړ�����
		velocity.x += cos(moveRad) * moveSpeed;
		velocity.z += sin(moveRad) * moveSpeed;

		//��莞�Ԉړ�������SATY�Ɉڍs
		if (moveTime >= maxMoveTime) {
			moveTime = 0;
			state = STAY;
		}
		break;
	case Enemy::HOMING:
		//�^�[�Q�b�g�����ł��Ă����ꍇ��ǐՔ͈͂��O�ꂽ�Ƃ���STAY�Ɉڍs
		if (targetLength >= holmingLength) {
			minTargetLength = holmingLength;
			state = STAY;

		}
		else {//if(������ray���I�u�W�F�N�g�ɓ͂��΂̏�����������){
			targetVec = Vector3(targetPos - pos);
			targetVec.y = 0;
			velocity += targetVec.Normalize() * moveSpeed;
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

	//���ʏ���

	//���G���ԃ^�C�}�[���Ǘ�
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

		//�^�C�}�[��30�ɂȂ����疳�G������
		if (InvincibleTimer >= 60) {
			isInvincible = false;
		}
	}
	Move();

	//�}�b�v�`�b�v�p�̔�����ړ�
	rect2d.Top = -(int)((pos.z + scale.x * 100.0f));
	rect2d.Bottom = -(int)((pos.z - scale.x * 100.0f));
	rect2d.Right = (int)((pos.x + scale.x * 100.0f));
	rect2d.Left = (int)((pos.x - scale.x * 100.0f));

}

void Enemy::OnCollision(const CollisionInfo &info)
{
	Debris *debri;
	switch (info.object->Tag)
	{
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

void Enemy::StaticUpdate() {

	//�폜
	for (int i = enemys.size() - 1; i >= 0; i--) {
		if (!enemys[i]->isAlive) {
			delete enemys[i];
			enemys.erase(enemys.begin() + i);
		}
	}
	//�X�V
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

void Enemy::Damage(float damage)
{
	//���G���Ԓ��͏����𒆒f
	if (isInvincible) { return; }
	//�_���[�W���󂯂�
	HP -= damage;
	//HP��0�ȉ��ɂȂ����玀�S��Ԃֈȍ~
	if (HP < 0) {
		state = DEAD;
	}
	else{
		//���G���Ԃ��Z�b�g����
		isInvincible = true;
		InvincibleTimer = 0;
	}
}

void Enemy::HomingObjectCheck(Vector3 targetPos)
{
	this->targetPos = targetPos;
	state = HOMING;
}
