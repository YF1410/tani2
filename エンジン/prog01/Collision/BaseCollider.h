#pragma once

#include "CollisionInfo.h"
#include "GameObjCommon.h"


//�Փ˔���p�֘A�̌^��`
enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // ���ݒ�

	SHAPE_POINT		= 0x001,	//�_
	SHAPE_RAY		= 0x002,	//���i�I�[�Ȃ��j
	SHAPE_LINE		= 0x004,	//���i�I�[����j
	SHAPE_TRIANGLE	= 0x008,	//�O�p�`
	SHAPE_SPHERE	= 0x010,	//��
	SHAPE_CYLINDER	= 0x020,	//��
	SHAPE_AABB		= 0x040,	//AABB
	SHAPE_OBB		= 0x080,	//OBB
	SHAPE_CAPSULE	= 0x100,	//�J�v�Z��
	SHAPE_MESH		= 0x200,	//���b�V��
	// = 0x000	//�R���W�����^�C�v
};

class BaseCollider
{
public:
	friend class CollisionManager;
	BaseCollider() = default;
	virtual ~BaseCollider() = default;		//���z�f�X�g���N�^

	//�I�u�W�F�N�g���Z�b�g
	inline void SetObject(GameObjCommon *object) { this->object = object; }
	//�Z�b�g����Ă���I�u�W�F�N�g�����^�[��
	inline GameObjCommon *GetObject3d() { return object; }

	//�X�V
	virtual void Update() = 0;
	//�`��^�C�v�擾
	inline CollisionShapeType GetShapeType() { return shapeType; }

	//�Փˎ��R�[���o�b�N�֐�
	inline void OnCollision(const CollisionInfo& info)
	{
		object->OnCollision(info);
	}

	// �����蔻�葮�����Z�b�g
	inline void SetAttribute(unsigned short attribute)
	{
		this->attribute = attribute;
	}

	// �����蔻�葮����ǉ�
	inline void AddAttribute(unsigned short attribute)
	{
		this->attribute |= attribute;
	}

	// �����蔻�葮�����폜
	inline void RemoveAttribute(unsigned short attribute)
	{
		this->attribute &= !attribute;
	}

protected:
	GameObjCommon *object = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	// �����蔻�葮��
	unsigned short attribute = 0b1111111111111111;
};