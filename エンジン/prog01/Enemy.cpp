#include "Enemy.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include <stdlib.h>
#include <time.h>
#include "Easing.h"
#include "SlimeMath.h"
#include "MapChip.h"

using namespace DirectX;

Enemy::Enemy(XMFLOAT3 startPos, PlayerObject* player) :
	GameObjCommon(
		ModelManager::ENEMY_ROBO_1,	//�G�l�~�[���f�����Z�b�g
		GameObjCommon::ENEMY,	//�G�l�~�[�Ƃ��Ĉ���
		false,					//�d�͂̉e�����󂯂�
		startPos				//�����ʒu���Z�b�g
	) {
	isAlive = true;
	this->player = player;
	maxHP = 100.0f;
	HP = maxHP;
	isBounce = true;
	//HPbar����
	hpBer = new EnemyHp(HP, maxHP,pos);


	defScale = 2.5f;
	scale = defScale;
	//�����蔻�菉����
	float radius = 100;
	broadSphereCollider = new SphereCollider("BroadSphere", XMVECTOR{ 0,radius,0 }, radius);
	SetBroadCollider(broadSphereCollider);

	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, radius, radius);
	SetNarrowCollider(toMapChipCollider);

	Initialize();
}

Enemy::~Enemy() {
}


void Enemy::Initialize() {
	//�U���֌W
	attack = {
		true,
		false,
		40,
		0
	};
}

void Enemy::Update() {
	//�ړ��ʏ�����
	VelocityReset(0.95f);
	if (!isInvincible && velocity.Length() > maxMoveSpeed) {
		//�ō����x�𒴂��Ă����琧������
		velocity = velocity.Normal() * maxMoveSpeed;
	}
	if (isInvincible && velocity.Length() > maxMoveSpeed * 10.0f){
		velocity = velocity.Normal() * maxMoveSpeed * 4.0f;

	}
	penalty = { 0,0,0 };

	//�ʏ펞�����i������������΃t���O�ŊǗ�����j
	Move();

	//���ʏ���
	//���G���ԃ^�C�}�[���Ǘ�
	if (isInvincible) {
		InvincibleTimer++;
		//�����͌�ŃA�j���[�V�����ɕύX����
		if (InvincibleTimer <= 10) {
			scale = Ease(In, Back, (float)(InvincibleTimer / 10.0f), 1.0f, 3.0f)*defScale;
		}
		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP > 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), 3.0f  , 1.0f  ) * defScale;
		}

		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP <= 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), 3.0f, 0.0f) * defScale;
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
				scale = defScale;
			}
		}
	}

	//�U���C���^�[�o������
	attack.Intervel();
	//�ړ�����������K��
	PosAddVelocity();
}

void Enemy::LustUpdate() {

	//�}�b�v�`�b�v�Ƃ̓����蔻��
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	Vector3 normal = { 0,0,0 };
	//�㉺
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
	else if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal)) {

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

void Enemy::OnCollision(const CollisionInfo &info)
{
	//�_�C�i�~�b�N�L���X�g�p
	Enemy *enemy;

	//�Փ˂����R���C�_�[�ŕ���
	switch (info.object->Tag)
	{
	case PLAYER:
		HitPlayer(info);
		break;
	case DEBRIS:
		HitDebri(info);
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
	if (!isInvincible) {
		targetVec = Vector3(player->GetPos() - pos);
		targetVec.y = 0;
		velocity += targetVec.Normal() * moveSpeed;
	}
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
	hpBer->HpDraw.Start();
	//���G���Ԓ��͏����𒆒f
	if (isInvincible) { return; }
	hpBer->HpDraw.timer = hpBer->HpDraw.interval;
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

void Enemy::HitPlayer(const CollisionInfo &info)
{
	PlayerObject *player;

	player = dynamic_cast<PlayerObject *>(info.object);

	//�v���C���[���U����ԂȂ�
	if (player->attack.is) {
		//�_���[�W���󂯂�
		Damage(player->attackPow);
	}
	else {
		//�ʒu�C��
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
		penalty.y = 0;
	}
}

void Enemy::HitDebri(const CollisionInfo &info)
{
	Debris *debri;
	//�f�u�����U����ԂȂ�_���[�W���󂯂�
	debri = dynamic_cast<Debris *>(info.object);
	if (debri->isAttack) {
		Damage(debri->velocity.Length());
	}
}
