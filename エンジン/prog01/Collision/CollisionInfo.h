#pragma once

#include <DirectXMath.h>
#include <string>
class GameObjCommon;
class BaseCollider;

struct CollisionInfo // 衝突情報
{
public:
	CollisionInfo(GameObjCommon *object, BaseCollider *collider, const DirectX::XMVECTOR &inter, std::string myName)
	{
		this->object = object;
		this->collider = collider;
		this->inter = inter;
		this->myName = myName;
	}

	//衝突相手のオブジェクト
	GameObjCommon *object = nullptr;
	//衝突相手のコライダー
	BaseCollider* collider = nullptr;
	//衝突した自分のコライダーの名前
	std::string myName;
	//衝突点
	DirectX::XMVECTOR inter;
};