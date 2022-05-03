#pragma once
#include "FbxObject3d.h"
#include "Object3d.h"
#include "Vector3.h"
#include "SphereCollider.h"
#include "Box2DCollider.h"
#include "GameObjCommon.h"
#include "State.h"
#include "ParticleEmitter.h"


class PlayerObject :
	public GameObjCommon
{
public:
	PlayerObject(XMFLOAT3 startPos);
	~PlayerObject();
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
	float GetSpeed() { return velocity.Length(); }
	float GetEnergy() { return energy; }

	//�ǂƂ̏Փ�
	void HitWall(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal);



	//����t���O
	STATE recovery;
	bool dontRecovery;

	//�U����
	int attackCount;
	//�U���t���O
	STATE attack;
	//�U����
	float attackPow;

	//
	STATE attackGage;

	//�f�u������
	STATE debrisCooldown;

	//�R���C�_�[
	SphereCollider *broadSphereCollider;	//�u���[�h
	SphereCollider *pushBackCollider;	//�����Ԃ��p
	SphereCollider *attackCollider;	//�U���p


private:


	//��_���[�W
	void Damage(float damage);
	//���G
	bool isInvincible;
	//����
	int invincibleCounter;

private: // �����o�ϐ�
	

	//�L�[�{�[�h�ړ��p
	float moveSpead;
	
	//�X�^�[�g�ʒu
	XMFLOAT3 startPos;
	//�X�P�[���ɑ΂���z���䗦
	const float suctionRatio = 600.0f;

	//���˃t���O
	bool isBounce;
	
	//�G�l���M�[
	float energy;	//����
	//�����Ŕ�΂���
	const int DESTRUCT_POW = 5;
	//�����Ɏg�����v�G�l���M�[
	const float SHOT_ENERGY = 100;



	
	Box2DCollider *toMapChipCollider;

	//�p�[�e�B�N��
	ParticleEmitter* healParticle1;
	ParticleEmitter* healParticle2;
	ParticleEmitter* boomParticle;
	ParticleEmitter* refParticle;
	ParticleEmitter* atkParticle;
	ParticleEmitter* atkStockParticle;
	int atkStockParticleTimer = 0;
	int atkStockParticleMaxTimer = 40;
	ParticleEmitter* recoveryParticle;

	int nowHealFrame = 0;
	int nowBoomFrame = 0;
	bool isHealFrameIncrease = false;
	bool isBoomFrameIncrease = false;
	int maxHealFrame = 3;
	int maxBoomFrame = 2;

	GameObjCommon *coreUp;

	int timer=0;
	int maxTimer = 30;
	int shakeCount = 201;
	int attenuation = 0;
	XMFLOAT2 shake;
};

