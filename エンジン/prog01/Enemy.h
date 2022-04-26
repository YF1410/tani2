#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "GameObjCommon.h"
#include "SphereCollider.h"
#include "PlayerObject.h"
#include "Box2DCollider.h"
#include "State.h"
#include "Debris.h"


class Enemy :
	public GameObjCommon
{
public:
	//�R���X�g���N�^
	Enemy(XMFLOAT3 startPos,PlayerObject *targetPos);
	~Enemy();
	//������
	void Initialize() override;
	//�����X�V
	virtual void Update() override;
	//�ŏI�X�V
	virtual void LustUpdate() override;
	//�Փˎ��R�[���o�b�N
	virtual void OnCollision(const CollisionInfo &info) override;

	bool GetIsAlive() { return isAlive; }
public:		//�����蔻��֌W
	//�_���[�W��^����
	int CauseDamage();
	//�U���t���O
	STATE attack;
	//�^������_���[�W
	int attackPow = 1;

	//State
	bool isInvincible;		//���G��Ԃ�

	bool isAlive;

protected: // �����o�ϐ�
	//�ړ�����
	virtual void Move();
	virtual void Attack();
	
	//�_���[�W���󂯂�
	virtual void Damage(float damage);

	//�ړ��n
	float moveSpeed = 5.0f;			//�ړ���
	float maxMoveSpeed = 15.0f;		//�ő�ړ���

	//�̗�
	float maxHP;		//�ő�HP
	float HP;			//����HP


	//���G����
	int InvincibleTimer;	//�c�薳�G����

	//�v���C���[�̃|�C���^
	PlayerObject *player;
	//�^�[�Q�b�g�ւ̃x�N�g��
	Vector3 targetVec;
	//�Ώۂ܂ł̋���
	float targetLength;

	//�ǂƂ̏Փ�
	void HitWall(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal);

	//�R���C�_�[
	SphereCollider *broadSphereCollider;	//�\���p�R���C�_�[
	SphereCollider *pushBackCollider;		//�����ȃR���C�_�[�i���󖢎g�p�j
	Box2DCollider *toMapChipCollider;		//�}�b�v�`�b�v�p�R���C�_�[
};