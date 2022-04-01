#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

class Box2DCollider : public BaseCollider, public Box2D
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
public:
	Box2DCollider(std::string tag, XMVECTOR offset = { 0,0,0,0 }, float x = 1.0f, float y = 1.0f) :
		offset(offset),
		x(x),
		y(y)
	{
		// ボックス形状をセット
		shapeType = SHAPE_BOX2D;
		this->collisionName = tag;

	}
	// 更新
	void Update() override;

	inline const XMVECTOR &GetOffset() { return offset; }

	inline void SetOffset(const XMVECTOR &offset) { this->offset = offset; }

	inline float GetRadiusX() { return x; }
	inline float GetRadiusY() { return y; }

	inline void SetRadius(float x,float y) { 
		this->x = x;
		this->y = y;
	}

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset;
	// 半径
	float x;
	float y;
};

