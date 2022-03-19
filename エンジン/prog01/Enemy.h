#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "Collision.h"

class Enemy {
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	//�R���X�g���N�^
	Enemy(XMFLOAT3 startPos);
	//�f�X�g���N�^
	~Enemy();
	//�����X�V
	virtual void Update();
	//�ŏI�X�V
	void Adaptation();
	// �`��
	void Draw();
	//�R���e�i�X�V
	static void StaticUpdate();
	static void StaticAdaptation();
	static void StaticDraw();

public:
	enum STATE {
		NOUPDATE,		//��ʊO���A�b�v�f�[�g�����Ȃ����
		STAY,			//�������Ȃ�
		WANDERING,		//���܂悢���
		HOMING,			//�v���C���[���������Ƃ�
		ATTACK,			//�U�����
		DEAD,			//���S���
						
	}state;


public:		//�����蔻��֌W

	//�����蔻��
	struct COLLIDER {
		//���G�͈�
		Sphere searchArea;
		//�����蔻��
		Sphere hitSphere;
		//�U���p����
		Sphere attackSphere;
		//�����ڂɋ߂�����
		Sphere realSphere;

	}collider;
	//�R���C�_�[�̍X�V
	void UpdateCollider();

	//�_���[�W���󂯂�
	void Damage(float damage);
	
	//�ǐՑΏۂ�����
	void HomingObjectCheck(Vector3 targetPos);


	//�����蔻��Ɏg�p
	Vector3 GetPos() { return pos; }
	void SetPos(Vector3 pos) { this->pos = pos; }

	//�R���e�i
	static std::vector<Enemy*> enemys;

	//State
	bool isInvincible;		//���G��Ԃ�


	//�ʏ�̍��G�͈�
	const float sarchLength = 500;
	//�ǐՔ͈�
	const float holmingLength = 700;

private: // �����o�ϐ�
	std::unique_ptr<FbxObject3d> enemyObj;

	int deadTimer;

	float searchPlayerLen = 5.0f;
	bool isPlayerContact = false;	//player���m�F������
	bool isWandering = false;		//���܂悢��Ԃ�
	int wanderingCount = 0;
	float moveSpeed = 3.0f;
	bool isAlive;

	//�̗�
	float HP;
	//���G����
	int InvincibleTimer;	//�c�薳�G����

	//�v�Z�p���W
	Vector3 pos;
	//���ړ���
	Vector3 moveVec;
	//�ړ��ʒu�\�z
	Vector3 afterPos;

	//�ҋ@�֌W
	int stayTime = 0;
	const int maxStayTime = 120;

	//���낤��֌W
	//�����_���ړ��p�p�x
	float moveRad = 0.0f;
	int moveTime = 0;				//�ړ����^�C�}�[
	const int maxMoveTime = 180;	//�ړ��I���p

	//�z�[�~���O���
	//�^�[�Q�b�g���W�|�C���^
	Vector3 targetPos;
	//�^�[�Q�b�g�ւ̃x�N�g��
	Vector3 targetVec;
	//�Ώۂ܂ł̋���
	float targetLength;
	//���݂̍ŋߑΏۂւ̋���
	float minTargetLength;


	//�`��p�傫��
	float scale;
};