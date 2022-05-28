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
	enum ANIMATION_TYPE {
		RETRIEVE,
		BOOST,
		DEATH,
		NEUTRAL,
		MOVE,
		MAX

	}animationType;
	bool animationChangeFrag;

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

	void SetEndFlag(bool cFlag, bool gFlag);
	

	//�Q�b�^�[
	Vector3 GetPos() { return pos; }
	Vector3 *GetPosPointer() { return &pos; }
	float GetSpeed() { return velocity.Length(); }
	float GetEnergy() { return hp; }

	//�ǂƂ̏Փ�
	void HitWall(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal);

	float volume = 0.0f;

	//����t���O
	STATE recovery;
	int recoveryEndTimer;
	bool dontRecovery;
	bool dontBoost;


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


	//hp
	float hp;	//�̗�
	float maxHp;	//�ő�

	bool healChack = false;

	void StopState();
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
	ParticleEmitter* refParticle2;
	ParticleEmitter* atkParticle;
	bool refParticleFlag = false;

	int nowBoomFrame = 0;
	bool isHealFrameIncrease = false;
	bool isBoomFrameIncrease = false;
	int maxBoomFrame = 2;


	int maxTimer = 30;
	int shakeCount = 100;
	XMFLOAT2 shake;
	//�����Ȃ�����t���O
	bool endFlag = false;
	//�u�[�X�g���̉����Ȃ��Ă邩
	bool boostFlag = false;
	//angle��ۑ�
	float saveAngle = 0.0f;
	bool saveAngleFlag = false;

	//�V�F�C�N���E�p
	Vector3 savePos;
	int shakeTime;
	//���Z�b�g�t���O
	bool resetFlag = false;
	bool isHitStop = false;
	int hitStopTimer = 0;
	int hitStopCount = 0;
	bool isHitStopCoolDown = 0;
	int hitStopCoolDown = 12;
	bool isFirstHitStop = false;
	Vector3 saveVelocity = {0,0,0};
};