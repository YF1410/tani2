#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "Collision.h"

class Debris
{
public:
	//�R���X�g���N�^
	Debris(XMFLOAT3 startPos,Vector3 startVec,float size);
	//�����X�V
	void Update();
	//�ŏI�X�V
	void Adaptation();
	//�`��
	void Draw();
	//�R���e�i�X�V
	static void StaticUpdate();
	static void StaticReflection();
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

	Vector3 pos;
	Vector3 moveVec;
	

	bool isAttackFlame;
private:
	//�j�ЃI�u�W�F�N�g
	std::unique_ptr<FbxObject3d> debriObj;

	//�v�Z�p���W
	//���ړ���
	//�ړ��\�z�ʒu
	Vector3 afterPos;

	//��C��R
	Vector3 airResistance;

	//���l�I�ȑ傫��
	float size;
	//�`��p�傫��
	float scale;

};

