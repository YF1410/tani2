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

	//ブロードフェーズ
	itA = broadColliders.begin();
	for (; itA != broadColliders.end(); ++itA)
	{
		itB = itA;
		++itB;
		for (; itB != broadColliders.end(); ++itB)
		{
			BaseCollider *colA = *itA;
			BaseCollider *colB = *itB;

			//自分同士の衝突や、コライダー未設定の者は無視する
			if (itB == itA ||
				colA->GetShapeType() == SHAPE_UNKNOWN ||
				colB->GetShapeType() == SHAPE_UNKNOWN) {
				continue;
			}
			//除外リストに載っているものをパスする
			if (colA->object->exclusionList.size() != 0) {
				auto result = std::find(
					colA->object->exclusionList.begin(),
					colA->object->exclusionList.end(),
					colB->object->Tag);
				if (result != colA->object->exclusionList.end()) {
					continue;
				}
			}

			//判定チェック
			CheckHitCollision(colA, colB);
		}
	}
}

void CollisionManager::CheckHitCollision(BaseCollider *colA, BaseCollider *colB)
{
	int collisions = colA->GetShapeType() | colB->GetShapeType();

	//ブロードに使用するものからチェック
	//球と球
	if (collisions == SHAPE_SPHERE)
	{
		Sphere *SphereA = dynamic_cast<Sphere *>(colA);
		Sphere *SphereB = dynamic_cast<Sphere *>(colB);
		DirectX::XMVECTOR inter;
		//判定
		if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter))
		{
			colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, colA->GetCollisionName()));
			colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, colB->GetCollisionName()));
		}
	}
	//AABBとAABB
	else if (collisions == SHAPE_AABB) {
	
	}

	//異なる組み合わせ
	//球とAABB
	else if (collisions == (SHAPE_SPHERE | SHAPE_AABB)) {
		Sphere *sohereCollider;
		AABB *boxCollider;
		//引数様に変換合わせる
		if (colA->GetShapeType() == SHAPE_SPHERE) {
			sohereCollider = dynamic_cast<Sphere *>(colA);
			boxCollider = dynamic_cast<AABB *>(colB);
		}
		else {
			sohereCollider = dynamic_cast<Sphere *>(colB);
			boxCollider = dynamic_cast<AABB *>(colA);
		}
		DirectX::XMVECTOR inter;
		//判定
		if (Collision::CheckSphere2AABB(*sohereCollider, *boxCollider)) {
			colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, colA->GetCollisionName()));
			colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, colB->GetCollisionName()));
		}
	}
	//球とメッシュ
	else if (collisions == (SHAPE_SPHERE | SHAPE_MESH))
	{
		MeshCollider *meshCollider;
		Sphere *sphere;
		//引数様に変換合わせる
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


	//カプセル系
	//カプセルとAABB
	else if (collisions == (SHAPE_CAPSULE | SHAPE_AABB)) {
		Capsule *capsuleCollider;
		Box *boxCollider;
		//引数様に変換合わせる
		if (colA->GetShapeType() == SHAPE_CAPSULE) {
			capsuleCollider = dynamic_cast<Capsule *>(colA);
			boxCollider = dynamic_cast<Box *>(colB);
		}
		else {
			capsuleCollider = dynamic_cast<Capsule *>(colB);
			boxCollider = dynamic_cast<Box *>(colA);
		}
		DirectX::XMVECTOR inter;
		//判定
		if (Collision::CheckCapsule2Box(*capsuleCollider, *boxCollider)) {
			colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, colA->GetCollisionName()));
			colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, colB->GetCollisionName()));
		}
	}
}

//bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
//{
//	//全属性有効にして属性版を実行
//	return Raycast(ray, 0xffff, hitInfo, maxDistance);
//}
//
//bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
//{
//	bool result = false;
//	//走査用のイテレータ
//	std::forward_list<BaseCollider*>::iterator it;
//	//今までで最も近いコライダーを記録する為のイテレータ
//	std::forward_list<BaseCollider*>::iterator it_hit;
//	//今までで最も近いコライダーの距離を記録する変数
//	float distance = maxDistance;
//	//今までで最も近いコライダーとの交点を記録する変数
//	XMVECTOR inter;
//
//	// 全てのコライダーと総当りチェック
//	it = colliders.begin();
//	for (; it != colliders.end(); ++it)
//	{
//		BaseCollider* colA = *it;
//
//
//		//球の場合
//		if (colA->GetShapeType() == SHAPE_SPHERE)
//		{
//			Sphere* sphere = dynamic_cast<Sphere*>(colA);
//
//			float tempDistance;
//			XMVECTOR tempInter;
//
//			//当たらなければ除外
//			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
//			//距離が最小でなければ除外
//			if (tempDistance >= distance) continue;
//			//今までで最も近いので記録を取る
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
//	//最終的になにかに当たっていたら結果を書き込む
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

	// 全てのコライダーと総当りチェック
	it = broadColliders.begin();
	for (; it != broadColliders.end(); ++it)
	{
		BaseCollider* col = *it;

		// 球
		if (col->GetShapeType() == SHAPE_SPHERE)
		{
			Sphere* sphereB = dynamic_cast<Sphere*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;
			if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

			// 交差情報をセット
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3d();
			info.inter = tempInter;
			info.reject = tempReject;

			// クエリーコールバック呼び出し
			if (!callback->OnQueryHit(info))
			{
				// 戻り値がfalseの場合、継続せず終了
				return;
			}
		}
		// メッシュ
		else if (col->GetShapeType() == SHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;
			if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

			// 交差情報をセット
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3d();
			info.inter = tempInter;
			info.reject = tempReject;

			// クエリーコールバック呼び出し
			if (!callback->OnQueryHit(info))
			{
				// 戻り値がfalseの場合、継続せず終了
				return;
			}
		}
	}
}