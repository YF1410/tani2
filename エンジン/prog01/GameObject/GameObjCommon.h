#pragma once
#include "Vector3.h"
#include "ModelManager.h"
#include "FbxObject3d.h"


class GameObjCommon
{
public://�T�u�N���X
	enum Tag {
		Notag,
		MapChip,
		Player,
		Enemy,

		MAX
	};
public:
	GameObjCommon(
		ModelManager::ModelName modelName,	//���f���̖��O
		Tag tag = Notag,					//�����蔻��p�^�O
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
		object = nullptr;
		object = FbxObject3d::Create(ModelManager::GetIns()->GetModel(modelName));
		object->SetModel(ModelManager::GetIns()->GetModel(modelName));
		object->SetPosition(pos);
		object->SetRotation(rotate);
		object->SetScale(scale);
		Initialize();
	}

	~GameObjCommon()
	{
	}

	virtual void Initialize();
	virtual void Update();
	//�d�͌v�Z�ƈړ��K��
	void MovePos();
	//�ύX��K��
	void Reflection();
	//�A�j���[�V�����Đ�
	void PlayAnimation(bool isLoop = false) { object->PlayAnimation(isLoop); };
	virtual void Draw() const;
	
	////���ʃx�N�g���擾
	//Vector3 GetFrontVector() { return Vector3(object->matWorldf._31, object->matWorldf._32, object->matWorldf._33).Normalize(); }
	////�E�x�N�g���擾
	//Vector3 GetRightVector() { return Vector3(object->matWorldf._11, object->matWorldf._12, object->matWorldf._13).Normalize(); }
	////��x�N�g���擾
	//Vector3 GetUpVector() { return Vector3(object->matWorldf._21, object->matWorldf._22, object->matWorldf._23).Normalize(); }

	//�I�u�W�F�N�g�擾
	//std::unique_ptr<FbxObject3d> GetObjectDate() { return object; }
 
public:


	Vector3 pos;	//���W
	Vector3 rotate;//����
	float gravityPow;
	Vector3 velocity;	//�ړ���
	Vector3 scale;	//�T�C�Y

	bool isGravity;	//�d�͂̉e�����󂯂邩�ǂ���
	bool isActive;	//�X�V�t���O
	bool isInvisible;//�������t���O
	bool isDelete;	//�����t���O

	Tag Tag;		//�I�u�W�F�N�g�^�O


protected:
	std::unique_ptr<FbxObject3d> object;

};

