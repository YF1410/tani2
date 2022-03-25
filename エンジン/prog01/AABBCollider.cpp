
#include "AABBCollider.h"

void AABBCollider::Update()
{
	// ワールド行列から座標を抽出
	const XMMATRIX &matWorld = object->GetObjectDate()->GetMatWorld();

	// 球のメンバ変数を更新
	AABB::center = matWorld.r[3] + offset;
	AABB::length = length;
}
