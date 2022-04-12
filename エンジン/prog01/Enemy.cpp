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
		ModelManager::ENEMY,	//�G�l�~�[���f�����Z�b�g
		GameObjCommon::ENEMY,	//�G�l�~�[�Ƃ��Ĉ���
		false,					//�d�͂̉e�����󂯂�
		startPos				//�����ʒu���Z�b�g
	)
{
	isAlive = true;
	scale = 20.0f;
	minTargetLength = holmingLength;
	this->player = player;
	state = HOMING;
	maxHP = 1;
	
	//�����蔻�菉����
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
	//�U���֌W
	attack = {
		true,
		false,
		20,
		0
	};
	HP = maxHP;

}

void Enemy::Update() {
	//�ړ��ʏ�����
	if (velocity.Length() > maxMoveSpeed) {
		velocity = velocity.Normal() * maxMoveSpeed;
	}
	else {
		VelocityReset(0.9f);
	}

	//State�ʏ���
	switch (state)
	{
	case Enemy::NOUPDATE:
		//��ʊO�ɂ���Ƃ��͍X�V���Ȃ�
		return;
		break;
	case Enemy::STAY:		//�ҋ@
		////�ҋ@���Ԃ��o�߂�����
		//stayTime++;
		//if (isPlayerContact) {		//�ҋ@���ԂɊ֌W�Ȃ��v���C���[���@�m���Ă����HOMING�ֈȍ~
		//	state = HOMING;
		//}
		//else if (stayTime >= maxStayTime) {
		//	//�p�x�������_���Ō���
		//	moveRad = XMConvertToRadians(static_cast<float>(rand() % 360));
		//	stayTime = 0;
		//	state = WANDERING;
		//}
		break;

	case Enemy::WANDERING:	//���낤�낷��
		////�ړ����Ԃ��o�߂�����
		//moveTime++;
		////�����_���ȕ����ւƈړ�����
		//velocity.x += cos(moveRad) * moveSpeed;
		//velocity.z += sin(moveRad) * moveSpeed;
		////��莞�Ԉړ�������SATY�Ɉڍs
		//if (moveTime >= maxMoveTime) {
		//	moveTime = 0;
		//	state = STAY;
		//}
		break;
	case Enemy::HOMING:
		//�^�[�Q�b�g�����ł��Ă����ꍇ��ǐՔ͈͂��O�ꂽ�Ƃ���STAY�Ɉڍs

		/*if (targetLength >= holmingLength) {
			minTargetLength = holmingLength;
			state = STAY;

		}*/
		//else {//if(������ray���I�u�W�F�N�g�ɓ͂��΂̏�����������){
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

	//���ʏ���
	//���G���ԃ^�C�}�[���Ǘ�
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

		//�^�C�}�[��60�ɂȂ����疳�G������
		if (InvincibleTimer >= 60) {
			isInvincible = false;
			//HP��0�ȉ��ɂȂ����玀�S��Ԃֈȍ~
			if (HP <= 0) {
				state = DEAD;
				//���̊m���ŃA�C�e���h���b�v
				if (rand() % 101 <= 30) {
					Debris::debris.push_back(new Debris(pos, { 0,0,0 }, 5, false));
				}
			}
		}
	}

	//�U���C���^�[�o������
	attack.Intervel();

	Move();

}

void Enemy::FinalUpdate()
{

	//�}�b�v�`�b�v�Ƃ̓����蔻��
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	//�㉺���E
	if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos)) {
		Vector3 normal = { 0,0,0 };

		if (hitPos.x != 0) {
			int vec = 1;	//����
			if (0 < moveVec.x) {
				vec = -1;
			}
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * vec;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//����
			if (moveVec.z < 0) {
				vec = -1;
			}
			pos.z = hitPos.z - toMapChipCollider->GetRadiusY() * vec;
			normal.z = vec;
		}
		normal.Normalize();
		HitWall(hitPos, normal);
	}
	//�p
	else if (MapChip::GetInstance()->CheckMapChipToSphere2d(broadSphereCollider, &velocity, &hitPos)) {
		Vector3 normal = { 0,0,0 };
		if (hitPos.x != 0) {
			int vec = 1;	//����
			if (0 < velocity.x) {
				vec = -1;
			}
			pos.x = hitPos.x;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//����
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
		//�ʒu�C��
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
	//���G���Ԓ��͏����𒆒f
	if (isInvincible) { return; }
	//�_���[�W���󂯂�
	HP -= damage;
	//���G���Ԃ��Z�b�g����
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
