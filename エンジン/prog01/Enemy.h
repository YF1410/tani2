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
#include "EnemyHp.h"

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
	//�`��
	virtual void Draw() const override;
	//�Փˎ��R�[���o�b�N
	virtual void OnCollision(const CollisionInfo &info) override;

public:		//�����蔻��֌W
	//�_���[�W��^����
	int CauseDamage();
	//�U���t���O
	STATE attack;
	//�^������_���[�W
	int attackPow = 10;

	//State
	bool isInvincible;		//���G��Ԃ�

	//�v���C���[�����˕Ԃ邩�ۂ�
	bool isBounce;

	bool isAlive;


	//�R���C�_�[
	SphereCollider *broadSphereCollider;	//�\���p�R���C�_�[
	Box2DCollider *toMapChipCollider;		//�}�b�v�`�b�v�p�R���C�_�[

protected: // �����o�ϐ�
	EnemyHp *hpBer;

	//�ړ�����
	virtual void Move();
	virtual void Attack();
	
	//�_���[�W���󂯂�
	virtual void Damage(float damage);

	//�ړ��n
	float moveSpeed = 5.0f;			//�ړ���
	float maxMoveSpeed = 15.0f;		//�ő�ړ���
	//�ʏ펞�̑傫��
	float defScale;

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

	virtual void HitPlayer(const CollisionInfo &info);
	virtual void HitDebri(const CollisionInfo &info);
};