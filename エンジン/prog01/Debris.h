#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "GameObjCommon.h"
#include "SphereCollider.h"

class Debris :
	public GameObjCommon
{
public:
	//�R���X�g���N�^
	Debris(Vector3 startPos,Vector3 startVec,float size, Vector3 *playerPos);
	//�����X�V
	void Update();
	//�ړ��ʃ��Z�b�g���I�[�o�[���C�h
	void VelocityReset() override;
	//�R���e�i�X�V
	static void StaticUpdate();
	static void StaticAdaptation();
	static void StaticDraw();

public:
	enum STATE {
		NOUPDATE,		//��ʊO���A�b�v�f�[�g�����Ȃ����
		STAY,			//�������Ȃ�
		ATTACK,			//�U�����
		RETURN,			//���
	}state;
public:		//�Փˎ��֌W
	//�Փˎ��R�[���o�b�N
	void OnCollision(const CollisionInfo &info) override;

	//���˕Ԃ�Ƃ�
	void Bounse(
		const Vector3 &hitPos,		//�Փˈʒu
		const Vector3 &normal	//�Փ˂������Ƃ̌����x�N�g��
	);
	void ReturnStart();

	//�v���C���[�ɋz���񂹂�ꂽ�Ƃ�
	void SuckedPlayer(const Vector3 &pos, const float &suckedRadius);


	//�ǂƂ̏Փ�
	void HitWall(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal);

	Vector3 GetPos() { return pos; }
	BOX2D GetBox() { return rect2d; }

	//�T�C�Y���擾
	float GetSize() { return size; }

	//�����t���O
	bool isAlive;
	//�U�����
	bool isAttack;
	//���˒��㏜�O�p
	bool isFirstAttack;
	//����p�̃|�C���^�[
	Vector3 *playerPos = nullptr;
	
	//�R���e�i
	static std::vector<Debris *> debris;

	bool isAttackFlame;
private:
	//��C��R
	Vector3 airResistance;

	//���l�I�ȑ傫��
	float size;

	//���܂Ŗ߂�͂�����
	int returnTimer;

	//�����蔻��
	SphereCollider *hitCollider;
	SphereCollider *attackCollider;


	//�Փ˗p
	//�_���[�W���󂯂�
	void Damage(float damage);


	BOX2D rect2d;					//�}�b�v�`�b�v�v�Z�p
};

