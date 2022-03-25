#pragma once

#include <DirectXMath.h>

class GameObjCommon;
class BaseCollider;

struct CollisionInfo // 衝突情報
{
public:
	CollisionInfo(GameObjCommon* object, BaseCollider* collider, const DirectX::XMVECTOR& inter)
	{
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

	//衝突相手のオブジェクト
	GameObjCommon *object = nullptr;
	//衝突相手のコライダー
	BaseCollider* collider = nullptr;
	//衝突点
	DirectX::XMVECTOR inter;
};