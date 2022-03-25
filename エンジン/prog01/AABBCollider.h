#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"
class AABBCollider : public BaseCollider, public AABB
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMVECTOR = DirectX::XMVECTOR;

public:
	AABBCollider(XMVECTOR offset = { 0,0,0,0 }, XMFLOAT3 scale = {1.0f,1.0f,1.0f}) :
		offset(offset),
		length(scale)
	{
		//AABB�`����Z�b�g
		shapeType = SHAPE_AABB;
	}

	// �X�V
	void Update() override;

	inline const XMVECTOR &GetOffset() { return offset; }

	inline void SetOffset(const XMVECTOR &offset) { this->offset = offset; }

	inline XMFLOAT3 GetRadius() { return length; }

	inline void SetRadius(XMFLOAT3 scale) { this->length = scale; }

private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMVECTOR offset;
	// ���a
	XMFLOAT3 length;
};

