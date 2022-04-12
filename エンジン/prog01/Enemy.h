#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "GameObjCommon.h"
#include "SphereCollider.h"
#include "PlayerObject.h"
#include "Box2DCollider.h"
#include "State.h"

class Enemy :
	public GameObjCommon
{
public:
	//�R���X�g���N�^
	Enemy(XMFLOAT3 startPos,PlayerObject *targetPos);
	//������
	void Initialize() override;
	//�����X�V
	virtual void Update() override;
	virtual void FinalUpdate() override;

	//�Փˎ��R�[���o�b�N
	void OnCollision(const CollisionInfo &info) override;

public:
	enum STATE_NAME {
		NOUPDATE,		//��ʊO���A�b�v�f�[�g�����Ȃ����
		STAY,			//�������Ȃ�
		WANDERING,		//���܂悢���
		HOMING,			//�v���C���[���������Ƃ�
		DEAD,			//���S���
						
	}state;


public:		//�����蔻��֌W

	//�_���[�W��^����
	int Attack();
	//�U���t���O
	STATE attack;


	//�^������_���[�W
	int attackPow = 1;
	
	//�ǐՑΏۂ�����
	//void HomingObjectCheck(Vector3 targetPos);


	//�����蔻��Ɏg�p
	Vector3 GetPos() { return pos; }
	void SetPos(Vector3 pos) { this->pos = pos; }


	//State
	bool isInvincible;		//���G��Ԃ�


	//�ʏ�̍��G�͈�
	const float sarchLength = 500;
	//�ǐՔ͈�
	const float holmingLength = 700;
	//�U���͈�
	const float attackLength = 100;

	bool isAlive;


private: // �����o�ϐ�

	//�_���[�W���󂯂�
	void Damage(float damage);


	int deadTimer;

	float searchPlayerLen = 5.0f;
	bool isPlayerContact = false;	//player���m�F������
	bool isWandering = false;		//���܂悢��Ԃ�
	int wanderingCount = 0;
	float moveSpeed = 5.0f;
	float maxMoveSpeed = 20.0f;

	//�̗�
	int maxHP;		//�ő�HP
	int HP;			//����HP


	//���G����
	int InvincibleTimer;	//�c�薳�G����

	//�ҋ@�֌W
	int stayTime = 0;
	const int maxStayTime = 120;

	//���낤��֌W
	//�����_���ړ��p�p�x
	float moveRad = 0.0f;
	int moveTime = 0;				//�ړ����^�C�}�[
	const int maxMoveTime = 180;	//�ړ��I���p

	//�z�[�~���O���
	PlayerObject *player;
	//�^�[�Q�b�g���W�|�C���^
	Vector3 *targetPos;
	//�^�[�Q�b�g�ւ̃x�N�g��
	Vector3 targetVec;
	//�Ώۂ܂ł̋���
	float targetLength;
	//���݂̍ŋߑΏۂւ̋���
	float minTargetLength;



	//�ǂƂ̏Փ�
	void HitWall(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal);

	SphereCollider *broadSphereCollider;
	SphereCollider *pushBackCollider;
	Box2DCollider *toMapChipCollider;
};