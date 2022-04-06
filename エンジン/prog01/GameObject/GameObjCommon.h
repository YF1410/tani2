#pragma once
#include "Vector3.h"
#include "ModelManager.h"
#include "FbxObject3d.h"
#include "CollisionInfo.h"
#include <vector>
#include <map>

class GameObjCommon
{
public://�T�u�N���X
	//�Փˌ��m�p�^�O
	enum TAG {
		Notag,
		MapChip,
		PLAYER,
		DEBRIS,
		ENEMY,

		//Block�n
		DEFAULT_BLOACK,
		PASSING_BLOCK,

		MAX
	};
public:
	GameObjCommon(
		ModelManager::ModelName modelName,	//���f���̖��O
		TAG tag = Notag,					//�����蔻��p�^�O
		bool isGravity = false,				//�d�͏����̗L��
		Vector3 pos = { 0,0,0 },			//�����ʒu
		Vector3 scale = { 1,1,1 },			//�����T�C�Y
		Vector3 rotate = { 0,0,0 }			//������]
	) :
		Tag(tag),
		isGravity(isGravity),
		pos(pos),
		scale(scale),
		rotate(rotate),
		velocity(0, 0, 0),
		isActive(true),
		isInvisible(false),
		isDelete(false),
		gravityPow(0.4f)
	{
		objectData = nullptr;
		objectData = FbxObject3d::Create(ModelManager::GetIns()->GetModel(modelName));
		objectData->SetModel(ModelManager::GetIns()->GetModel(modelName));
		objectData->SetPosition(pos);
		objectData->SetRotation(rotate);
		objectData->SetScale(scale);
		Initialize();
	}

	~GameObjCommon();

	virtual void Initialize();		//������
	virtual void Update();			//�e�I�u�W�F�N�g�̍X�V
	virtual void Move();			//�ړ��ʌv�Z
	virtual void VelocityReset();								//�ړ��ʏ�����
	virtual void VelocityReset(float gain = 0.9f,float dead = 0.01f);	//�ړ��ʏ�����
	//�ύX��K��
	void Adaptation();
	//�A�j���[�V�����Đ�
	void PlayAnimation(int playNumber, bool isLoop = false) { objectData->PlayAnimation(isLoop); };
	//�`��
	virtual void Draw() const;

	////���ʃx�N�g���擾
	//Vector3 GetFrontVector() { return Vector3(object->matWorldf._31, object->matWorldf._32, object->matWorldf._33).Normalize(); }
	////�E�x�N�g���擾
	//Vector3 GetRightVector() { return Vector3(object->matWorldf._11, object->matWorldf._12, object->matWorldf._13).Normalize(); }
	////��x�N�g���擾
	//Vector3 GetUpVector() { return Vector3(object->matWorldf._21, object->matWorldf._22, object->matWorldf._23).Normalize(); }

	//�I�u�W�F�N�g�擾
	std::unique_ptr<FbxObject3d> &GetObjectDate() { return objectData; }

public:


	Vector3 pos;		//���W
	Vector3 oldPos = { 0,0,0 };		//���W
	Vector3 rotate;		//����
	float gravityPow;	//�d�͂̋���
	Vector3 velocity;	//�ړ���
	Vector3 scale;		//�g�嗦

	bool isGravity;		//�d�͂̉e��	true�ŏd�͂̉e�����󂯂�
	bool isActive;		//�X�V�t���O	false����Update()��ColliderUpdate()�̏��������s��Ȃ�
	bool isInvisible;	//��\���t���O	true�ŕ`����s��Ȃ�
	bool isDelete;		//�����t���O	true�ɂȂ�Ƃ��̃t���[���ŏ��ł���

	TAG Tag;			//�����蔻��ȂǂŎg���I�u�W�F�N�g�^�O

	//�����蔻�菜�O���X�g
	std::vector<TAG> exclusionList;

protected:
	std::unique_ptr<FbxObject3d> objectData;	//�I�u�W�F�N�g�f�[�^

public:
	//�u���[�h�t�F�C�Y�R���C�_�[�̃Z�b�g
	void SetBroadCollider(BaseCollider *collider);
	//�i���[�t�F�C�Y�R���C�_�[�̃Z�b�g
	void SetNarrowCollider(BaseCollider *collider);
	std::map<std::string, BaseCollider *> GetNarrowCollider() { return narrowColliders; }

	// �Փˎ��R�[���o�b�N�֐�
	virtual void OnCollision(const CollisionInfo &info) {}
protected:
	//�u���[�h�t�F�C�Y�R���C�_�[
	std::map<std::string, BaseCollider *> broadColliders;
	//�����ȃR���C�_�[
	std::map<std::string, BaseCollider *> narrowColliders;


};

