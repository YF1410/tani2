#pragma once

#include <DirectXMath.h>

class GameObjCommon;
class BaseCollider;

struct CollisionInfo // �Փˏ��
{
public:
	CollisionInfo(GameObjCommon* object, BaseCollider* collider, const DirectX::XMVECTOR& inter)
	{
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

	//�Փˑ���̃I�u�W�F�N�g
	GameObjCommon *object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR inter;
};