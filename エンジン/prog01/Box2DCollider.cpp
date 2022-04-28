#include "Box2DCollider.h"

void Box2DCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const Vector3 &matWorld = object->pos;

	// ���̃����o�ϐ����X�V
	Box2D::center = matWorld + offset;
	Box2D::Top = -y;
	Box2D::Bottom = y;
	Box2D::Right = x;
	Box2D::Left = -x;
}
