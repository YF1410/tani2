#include "Enemy.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include <stdlib.h>
#include <time.h>
#include "Easing.h"
#include "SlimeMath.h"
#include "MapChip.h"

using namespace DirectX;

Enemy::Enemy(XMFLOAT3 startPos,PlayerObject *player) :
	GameObjCommon(
		ModelManager::ENEMY_MACARON,	//�G�l�~�[���f�����Z�b�g
		GameObjCommon::ENEMY,	//�G�l�~�[�Ƃ��Ĉ���
		false,					//�d�͂̉e�����󂯂�
		startPos				//�����ʒu���Z�b�g
	)
{
	isAlive = true;
	scale = 1.0f;
	this->player = player;
	maxHP = 100.0f;
	
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

Enemy::~Enemy()
{
}


void Enemy::Initialize()
{
	//�U���֌W
	attack = {
		true,
		false,
		40,
		0
	};
	HP = maxHP;

}

void Enemy::Update() {
	//�ړ��ʏ�����
	if (velocity.Length() > maxMoveSpeed) {
		//�ō����x�𒴂��Ă����琧������
		velocity = velocity.Normal() * maxMoveSpeed;
	}
	else {
		//���t���[�������Ă���ړ��ʂ�0.9�{�ɂȂ�
		VelocityReset(0.9f);
	}
	//�����Ԃ�������
	penalty = { 0,0,0 };

	//�ʏ펞�����i������������΃t���O�ŊǗ�����j
	Move();

	//���ʏ���
	//���G���ԃ^�C�}�[���Ǘ�
	if (isInvincible) {
		InvincibleTimer++;
		//�����͌�ŃA�j���[�V�����ɕύX����
		if (InvincibleTimer <= 10) {
			scale = Ease(In, Back, (float)(InvincibleTimer / 10.0f), 1.0f, 3.0f);
		}
		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP > 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 30.0f), 3.0f  , 1.0f  );
		}

		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP <= 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 30.0f), 3.0f, 0.0f);
		}
		//�^�C�}�[��30�ɂȂ����疳�G������
		if (InvincibleTimer >= 30) {
			isInvincible = false;
			//HP��0�ȉ��ɂȂ����玀�S��Ԃֈȍ~
			if (HP <= 0) {
				isAlive = false;
				//���̊m���ŃA�C�e���h���b�v
				if (rand() % 101 <= 30) {
					Debris::debris.push_back(new Debris(pos, { 0,0,0 }, 5));
				}
			}
			else {
				scale = 1.0f;
			}
		}
	}

	//�U���C���^�[�o������
	attack.Intervel();
	//�ړ�����������K��
	PosAddVelocity();

}

void Enemy::LustUpdate()
{

	//�}�b�v�`�b�v�Ƃ̓����蔻��
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	Vector3 normal = { 0,0,0 };
	//�㉺���E
	if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos,&normal)) {
		if (hitPos.x != 0) {
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * normal.x;
		}
		if (hitPos.z != 0) {
			pos.z = hitPos.z + toMapChipCollider->GetRadiusY() * normal.z;
		}
		normal.Normalize();
		HitWall(hitPos, normal.Normal());
	}
	//�p
	else if (MapChip::GetInstance()->CheckMapChipToSphere2d(broadSphereCollider, &velocity, &hitPos)) {
		//Vector3 normal = { 0,0,0 };
		//if (hitPos.x != 0) {
		//	int vec = 1;	//����
		//	if (0 < velocity.x) {
		//		vec = -1;
		//	}
		//	pos.x = hitPos.x;
		//	normal.x = vec;
		//}
		//if (hitPos.z != 0) {
		//	int vec = 1;	//����
		//	if (velocity.z < 0) {
		//		vec = -1;
		//	}
		//	pos.z = hitPos.z;
		//	normal.z = vec;
		//}
		//normal.Normalize();
		//velocity = CalcWallScratchVector(velocity, normal);
	}

}

void Enemy::OnCollision(const CollisionInfo &info)
{
	//�_�C�i�~�b�N�L���X�g�p
	Debris *debri;
	PlayerObject *player;
	Enemy *enemy;

	//�Փ˂����R���C�_�[�ŕ���
	switch (info.object->Tag)
	{
	case PLAYER:
		player = dynamic_cast<PlayerObject *>(info.object);
		//�ʒu�C��
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
		penalty.y = 0;

		//�v���C���[���U����ԂȂ�
		if (player->attack.is) {
			//�_���[�W���󂯂�
			Damage(player->attackPow);

			//�Փˎ��x�N�g��
			Vector3 normal = pos - player->pos;
			normal.Normalize();
			//����
			float dot = Vector3(player->pos - pos).VDot(normal);
			//�萔�x�N�g��
			Vector3 constVec = 2 * dot / 2 * normal;

			velocity = 2 * constVec + velocity;

		}
		break;
	case DEBRIS:
		//�f�u�����U����ԂȂ�_���[�W���󂯂�
 		debri = dynamic_cast<Debris *>(info.object);
		if (debri->isAttack) {
			Damage(debri->velocity.Length());
		}
		break;

	case ENEMY:
		//�G�l�~�[���m�Ȃ牟���Ԃ�����
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length()*0.2f;
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
	targetVec = Vector3(player->GetPos() - pos);
	targetVec.y = 0;
	velocity += targetVec.Normal() * moveSpeed;
}

void Enemy::Attack()
{
	//�U���\�Ȃ�
	if (attack.can) {
		attack.Start();
	}
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

int Enemy::CauseDamage()
{
	Attack();
	return attackPow;
}

void Enemy::HitWall(const XMVECTOR &hitPos, const Vector3 &normal)
{
	//����
	velocity = CalcWallScratchVector(velocity, normal);

}
