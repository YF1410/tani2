#include "Box2DCollider.h"

void Box2DCollider::Update()
{
	// ワールド行列から座標を抽出
	const Vector3 &matWorld = object->pos;

	// 球のメンバ変数を更新
	Box2D::center = matWorld + offset;
	Box2D::Top = -y;
	Box2D::Bottom = y;
	Box2D::Right = x;
	Box2D::Left = -x;
}
