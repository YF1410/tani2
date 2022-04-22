#pragma once
#include "FbxObject3d.h"
#include "Object3d.h"
#include "Vector3.h"
#include "SphereCollider.h"
#include "Box2DCollider.h"
#include "GameObjCommon.h"
#include "State.h"

enum REVERSE_Range {
	MIN = 800,
	MAX = 1000
};

class PlayerObject :
	public GameObjCommon
{
public:
	PlayerObject(XMFLOAT3 startPos);
	//������
	void Initialize() override;
	//���t���[������
	void Update() override;
	void Draw() const override;
	void LustUpdate() override;
	//�Փˎ��R�[���o�b�N
	void OnCollision(const CollisionInfo &info) override;
	

	//�Q�b�^�[
	Vector3 GetPos() { return pos; }
	Vector3 *GetPosPointer() { return &pos; }
	float GetSuction() { return suction; }
	float GetSpeed() { return velocity.Length(); }
	float GetScale() { return scalef; }

	//�ǂƂ̏Փ�
	void HitWall(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal);

	int GetHp() { return size; }


	//����t���O
	STATE collect;
	//�U���t���O
	STATE attack;
	//�U����
	float attackPow;

private:


	//��_���[�W
	void Damage(float damage);
	//���G
	bool isInvincible;
	//����
	int invincibleCounter;

private: // �����o�ϐ�
	
	float size;	//����

	//�L�[�{�[�h�ړ��p
	float moveSpead;
	
	//�X�^�[�g�ʒu
	XMFLOAT3 startPos;
	//�X�P�[���ɑ΂���z���䗦
	const float suctionRatio = 600.0f;
	//�z���͈�
	float suction;
	//�T�C�Y
	float scalef;//�傫��

	//���˃t���O
	bool isBounce;

	//������
	const int destructPow = 10;

	//�R���C�_�[
	SphereCollider *broadSphereCollider;	//�u���[�h
	SphereCollider * pushBackCollider;	//�����Ԃ��p
	SphereCollider *attackCollider;	//�U���p

	Box2DCollider *toMapChipCollider;


	GameObjCommon *coreUp;
};

