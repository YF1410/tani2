#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"
class AABBCollider : public BaseCollider, public AABB
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;

public:
	AABBCollider(XMVECTOR offset = { 0,0,0,0 }, XMFLOAT3 scale = {1.0f,1.0f,1.0f}) :
		offset(offset),
		length(scale)
	{
		//AABB形状をセット
		shapeType = SHAPE_AABB;
	}

	// 更新
	void Update() override;

	inline const XMVECTOR &GetOffset() { return offset; }

	inline void SetOffset(const XMVECTOR &offset) { this->offset = offset; }

	inline XMFLOAT3 GetRadius() { return length; }

	inline void SetRadius(XMFLOAT3 scale) { this->length = scale; }

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset;
	// 半径
	XMFLOAT3 length;
};

