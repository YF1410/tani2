#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "GameObjCommon.h"
#include "SphereCollider.h"
#include "Box2DCollider.h"
#include "PlayerObject.h"

class Debris :
	public GameObjCommon
{
public:
	//�R���X�g���N�^
	Debris(Vector3 startPos,Vector3 startVec,float size);
	~Debris();
	//�����X�V
	void Update();
	void LustUpdate();
	//�ړ��ʃ��Z�b�g���I�[�o�[���C�h
	void VelocityReset() override;
	//�R���e�i�X�V
	static void StaticInitialize(PlayerObject *player);
	static void StaticUpdate();
	static void StaticLustUpdate();
	static void StaticAdaptation();
	static void StaticDraw();
	static void Finalize();

public:
	enum STATE {
		NOUPDATE,		//��ʊO���A�b�v�f�[�g�����Ȃ����
		STAY,			//�������Ȃ�
		ATTACK,			//�U�����
		RETURN,			//���
		SUCTION,
	}state;
public:		//�Փˎ��֌W
	//�Փˎ��R�[���o�b�N
	void OnCollision(const CollisionInfo &info) override;

	void ReturnStart();

	//�v���C���[�ɋz���񂹂�ꂽ�Ƃ�
	void SuckedPlayer(const Vector3 &pos, const float &suckedRadius);


	//�ǂƂ̏Փ�
	void HitWall(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal);

	Vector3 GetPos() { return pos; }

	//�T�C�Y���擾
	float GetSize() { return size; }

	void SetVelocity(Vector3 velocity) { this->velocity = velocity; }
	STATE GetState() { return state; }
	void SetState(STATE state) { this->state = state; }

	//�����t���O
	bool isAlive;
	//�U�����
	bool isAttack;
	//���˒��㏜�O�p
	bool isFirstAttack;

	bool isHitStop = false;
	int hitStopTimer = 0;

	//����p�̃|�C���^�[
	static PlayerObject *playerData;
	
	//�R���e�i
	static std::vector<Debris *> debris;

	bool isAttackFlame;

	//�����蔻��
	SphereCollider *hitCollider;
	//�U������
	SphereCollider *attackCollider;

	//�Փ˗p
	//�_���[�W���󂯂�
	void Damage(float damage);
private:
	//��C��R
	Vector3 airResistance;

	//���l�I�ȑ傫��
	float size;

	//���܂Ŗ߂�͂�����
	int returnTimer;


	bool reversFlag;

	Box2DCollider *toMapChipCollider;			//�}�b�v�`�b�v�v�Z�p
	bool isBoost;
};

