#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "Collision.h"
#include "GameObjCommon.h"

class Debris :
	public GameObjCommon
{
public:
	//�R���X�g���N�^
	Debris(Vector3 startPos,Vector3 startVec,float size);
	//�����X�V
	void Update();
	//�ړ��ʃ��Z�b�g���I�[�o�[���C�h
	void VelocityReset() override;
	//�R���e�i�X�V
	static void StaticUpdate();
	static void StaticAdaptation();
	static void StaticDraw();

public:		//�Փˎ��֌W

	//�����蔻��
	struct COLLIDER {
		//�����ڂɋ߂�����
		Sphere realSphere;
		//�U������
		Sphere attackSphere;
		//�����蔻��
		Sphere hitSphere;
	}collider;
	//�����蔻��ꊇ�X�V�p
	void UpdateCollider();
	//���˕Ԃ�Ƃ�
	void Bounse(
		const Vector3 &hitPos,		//�Փˈʒu
		const Vector3 &normal	//�Փ˂������Ƃ̌����x�N�g��
	);
	//�v���C���[�ɋz���񂹂�ꂽ�Ƃ�
	void SuckedPlayer(const Vector3 &pos, const float &suckedRadius);
	//�v���C���[�ɋz�����ꂽ�Ƃ�
	float AbsorbedToPlayer();

	//�_���[�W���󂯂�
	void Damage(float damage);

	Vector3 GetPos() { return pos; }


	//�����t���O
	bool isAlive;
	//�U�����
	bool isAttack;
	
	//�R���e�i
	static std::vector<Debris *> debris;

	bool isAttackFlame;
private:
	//��C��R
	Vector3 airResistance;

	//���l�I�ȑ傫��
	float size;

};

