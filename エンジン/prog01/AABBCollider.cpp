
#include "AABBCollider.h"

void AABBCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const XMMATRIX &matWorld = object->GetObjectDate()->GetMatWorld();

	// ���̃����o�ϐ����X�V
	AABB::center = matWorld.r[3] + offset;
	AABB::length = length;
}
