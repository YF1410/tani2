#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "Collision.h"

class Debris :
	public FbxObject3d
{
public:
	Debris(XMFLOAT3 startPos,Vector3 startVec,float size);
	void Update();
	void Draw();

	static void StaticUpdate();
	static void StaticDraw();

	//�v���C���[�ɋz�����ꂽ�Ƃ�
	float Absorbed();


	//�����蔻��
	struct COLLIDER {
		Sphere sphere;
	}collider;

	//�����t���O
	bool isAlive;
	//�U�����
	bool isAttack;

	//�R���e�i
	static std::vector<Debris *> debris;
private:
	Vector3 pos;
	Vector3 moveVec;
	//���l�I�ȑ傫��
	float size;

};

