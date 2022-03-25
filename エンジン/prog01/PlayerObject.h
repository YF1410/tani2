#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include "SphereCollider.h"
#include "GameObjCommon.h"

enum DESTRUCT_TYPE {
	DIRECTIVITY	=1,	//�w����
	CIRCLE		=2,	//�S����
};

enum DESTRUCT_POW {
	WEAK	= 8,	//�ア
	STRONG	= 16	//����
};

class PlayerObject :
	public GameObjCommon
{
public:
	PlayerObject();
	//������
	void Initialize() override;
	//���t���[������
	void Update() override;
	
	//�Փˎ��R�[���o�b�N
	void OnCollision(const CollisionInfo &info) override;
	
	
	float size;	//����

	//�Q�b�^�[
	Vector3 GetPos() { return pos; }
	Vector3 *GetPosPointer() { return &pos; }
	float GetSuction() { return suction; }
	float GetSpeed() { return velocity.Length(); }
	float GetScale() { return scale; }

private:	//�Փˎ��̏����֐�
	//�ǂƂ̏Փ�
	void HitWall(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal);
	//�c�[���z��������
	void Absorb(float size);

private: // �����o�ϐ�
	//�L�[�{�[�h�ړ��p
	float moveSpead;
	
	//�X�P�[���ɑ΂���z���䗦
	const float suctionRatio = 300.0f;
	//�z���͈�
	float suction;
	//�T�C�Y
	float scale;//�傫��



	//�����^�C�v
	DESTRUCT_TYPE destructType;
	//������
	DESTRUCT_POW destructPow;
	//�����t���O
	bool destructFlag = false;

	//�R���C�_�[
	SphereCollider *hitSphere;	//�Փ˔���p
};

