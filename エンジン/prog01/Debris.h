#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "Collision.h"

class Debris
{
public:
	Debris(XMFLOAT3 startPos,Vector3 startVec,float size);
	//�����X�V
	void Update();
	//�ŏI�X�V
	void Reflection();
	//�`��
	void Draw();

	static void StaticUpdate();
	static void StaticReflection();
	static void StaticDraw();
public:		//�Փˎ��֌W

	//�����蔻��
	struct COLLIDER {
		//���˕Ԃ�p����������
		Sphere realSphere;
		Sphere absorbedSphere;

	}collider;
	//�����蔻��ꊇ�X�V�p
	void UpdateCollider();
	//���˕Ԃ�Ƃ�
	void Bounse(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal	//�Փ˂������Ƃ̌����x�N�g��
	);
	//�v���C���[�ɋz���񂹂�ꂽ�Ƃ�
	void SuckedPlayer(const Vector3 &pos, const float &suckedRadius);
	//�v���C���[�ɋz�����ꂽ�Ƃ�
	float AbsorbedToPlayer();

	//�j�Г��m�ł��������Ƃ�
	float AbsorbedToDebri();

	Vector3 GetPos() { return pos; }


	//�����t���O
	bool isAlive;
	//�U�����
	bool isAttack;
	//��~
	bool isStop;

	//�R���e�i
	static std::vector<Debris *> debris;
private:
	//�j�ЃI�u�W�F�N�g
	std::unique_ptr<FbxObject3d> debriObj;

	//�v�Z�p���W
	Vector3 pos;
	//���ړ���
	Vector3 moveVec;
	//�ړ��\�z�ʒu
	Vector3 afterPos;

	//��C��R
	Vector3 airResistance;

	//���l�I�ȑ傫��
	float size;
	//�`��p�傫��
	float scale;

};

