#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "MeshCollider.h"

using namespace DirectX;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::CheckBroadCollisions()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	//�u���[�h�t�F�[�Y
	itA = broadColliders.begin();
	for (; itA != broadColliders.end(); ++itA)
	{
		itB = itA;
		++itB;
		for (; itB != broadColliders.end(); ++itB)
		{
			BaseCollider *colA = *itA;
			BaseCollider *colB = *itB;

			//�������m�̏Փ˂�A�R���C�_�[���ݒ�̎҂͖�������
			if (itB == itA ||
				colA->GetShapeType() == SHAPE_UNKNOWN ||
				colB->GetShapeType() == SHAPE_UNKNOWN) {
				continue;
			}
			//���O���X�g�ɍڂ��Ă�����̂��p�X����
			if (colA->object->exclusionList.size() != 0) {
				auto result = std::find(
					colA->object->exclusionList.begin(),
					colA->object->exclusionList.end(),
					colB->object->Tag);
				if (result != colA->object->exclusionList.end()) {
					continue;
				}
			}

			//����`�F�b�N
			CheckHitCollision(colA, colB);
		}
	}
}

void CollisionManager::CheckHitCollision(BaseCollider *colA, BaseCollider *colB)
{
	int collisions = colA->GetShapeType() | colB->GetShapeType();

	//�u���[�h�Ɏg�p������̂���`�F�b�N
	//���Ƌ�
	if (collisions == SHAPE_SPHERE)
	{
		Sphere *SphereA = dynamic_cast<Sphere *>(colA);
		Sphere *SphereB = dynamic_cast<Sphere *>(colB);
		DirectX::XMVECTOR inter;
		//����
		if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter))
		{
			colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, colA->GetCollisionName()));
			colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, colB->GetCollisionName()));
		}
	}
	//AABB��AABB
	else if (collisions == SHAPE_AABB) {
	
	}

	//�قȂ�g�ݍ��킹
	//����AABB
	else if (collisions == (SHAPE_SPHERE | SHAPE_AABB)) {
		Sphere *sohereCollider;
		AABB *boxCollider;
		//�����l�ɕϊ����킹��
		if (colA->GetShapeType() == SHAPE_SPHERE) {
			sohereCollider = dynamic_cast<Sphere *>(colA);
			boxCollider = dynamic_cast<AABB *>(colB);
		}
		else {
			sohereCollider = dynamic_cast<Sphere *>(colB);
			boxCollider = dynamic_cast<AABB *>(colA);
		}
		DirectX::XMVECTOR inter;
		//����
		if (Collision::CheckSphere2AABB(*sohereCollider, *boxCollider)) {
			colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, colA->GetCollisionName()));
			colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, colB->GetCollisionName()));
		}
	}
	//���ƃ��b�V��
	else if (collisions == (SHAPE_SPHERE | SHAPE_MESH))
	{
		MeshCollider *meshCollider;
		Sphere *sphere;
		//�����l�ɕϊ����킹��
		if (colA->GetShapeType() == SHAPE_MESH) {
			meshCollider = dynamic_cast<MeshCollider *>(colA);
			sphere = dynamic_cast<Sphere *>(colB);
		}
		else {
			meshCollider = dynamic_cast<MeshCollider *>(colB);
			sphere = dynamic_cast<Sphere *>(colA);
		}
		DirectX::XMVECTOR inter;
		if (meshCollider->CheckCollisionSphere(*sphere, &inter))
		{
			colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, colA->GetCollisionName()));
			colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, colB->GetCollisionName()));
		}
	}


	//�J�v�Z���n
	//�J�v�Z����AABB
	else if (collisions == (SHAPE_CAPSULE | SHAPE_AABB)) {
		Capsule *capsuleCollider;
		Box *boxCollider;
		//�����l�ɕϊ����킹��
		if (colA->GetShapeType() == SHAPE_CAPSULE) {
			capsuleCollider = dynamic_cast<Capsule *>(colA);
			boxCollider = dynamic_cast<Box *>(colB);
		}
		else {
			capsuleCollider = dynamic_cast<Capsule *>(colB);
			boxCollider = dynamic_cast<Box *>(colA);
		}
		DirectX::XMVECTOR inter;
		//����
		if (Collision::CheckCapsule2Box(*capsuleCollider, *boxCollider)) {
			colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, colA->GetCollisionName()));
			colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, colB->GetCollisionName()));
		}
	}
}

//bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
//{
//	//�S�����L���ɂ��đ����ł����s
//	return Raycast(ray, 0xffff, hitInfo, maxDistance);
//}
//
//bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
//{
//	bool result = false;
//	//�����p�̃C�e���[�^
//	std::forward_list<BaseCollider*>::iterator it;
//	//���܂łōł��߂��R���C�_�[���L�^����ׂ̃C�e���[�^
//	std::forward_list<BaseCollider*>::iterator it_hit;
//	//���܂łōł��߂��R���C�_�[�̋������L�^����ϐ�
//	float distance = maxDistance;
//	//���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^����ϐ�
//	XMVECTOR inter;
//
//	// �S�ẴR���C�_�[�Ƒ�����`�F�b�N
//	it = colliders.begin();
//	for (; it != colliders.end(); ++it)
//	{
//		BaseCollider* colA = *it;
//
//
//		//���̏ꍇ
//		if (colA->GetShapeType() == SHAPE_SPHERE)
//		{
//			Sphere* sphere = dynamic_cast<Sphere*>(colA);
//
//			float tempDistance;
//			XMVECTOR tempInter;
//
//			//������Ȃ���Ώ��O
//			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
//			//�������ŏ��łȂ���Ώ��O
//			if (tempDistance >= distance) continue;
//			//���܂łōł��߂��̂ŋL�^�����
//			result = true;
//			distance = tempDistance;
//			inter = tempInter;
//			it_hit = it;
//		}
//		else if (colA->GetShapeType() == SHAPE_MESH)
//		{
//			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
//		
//			float tempDistance;
//			DirectX::XMVECTOR tempInter;
//			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
//			if (tempDistance >= distance) continue;
//		
//			result = true;
//			distance = tempDistance;
//			inter = tempInter;
//			it_hit = it;
//		}
//	}
//
//	//�ŏI�I�ɂȂɂ��ɓ������Ă����猋�ʂ���������
//	if (result && hitInfo)
//	{
//		hitInfo->distance = distance;
//		hitInfo->inter = inter;
//		hitInfo->collider = *it_hit;
//		hitInfo->object = hitInfo->collider->GetObject3d();
//	}
//
//	return result;
//}

void CollisionManager::QuerySphere(const Sphere& sphere, QueryCallback* callback)
{
	assert(callback);

	std::forward_list<BaseCollider*>::iterator it;

	// �S�ẴR���C�_�[�Ƒ�����`�F�b�N
	it = broadColliders.begin();
	for (; it != broadColliders.end(); ++it)
	{
		BaseCollider* col = *it;

		// ��
		if (col->GetShapeType() == SHAPE_SPHERE)
		{
			Sphere* sphereB = dynamic_cast<Sphere*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;
			if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

			// ���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3d();
			info.inter = tempInter;
			info.reject = tempReject;

			// �N�G���[�R�[���o�b�N�Ăяo��
			if (!callback->OnQueryHit(info))
			{
				// �߂�l��false�̏ꍇ�A�p�������I��
				return;
			}
		}
		// ���b�V��
		else if (col->GetShapeType() == SHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;
			if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

			// ���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3d();
			info.inter = tempInter;
			info.reject = tempReject;

			// �N�G���[�R�[���o�b�N�Ăяo��
			if (!callback->OnQueryHit(info))
			{
				// �߂�l��false�̏ꍇ�A�p�������I��
				return;
			}
		}
	}
}