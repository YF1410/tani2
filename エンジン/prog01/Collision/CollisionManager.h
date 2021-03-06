#pragma once

#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include "QueryCallback.h"

#include <d3d12.h>
#include <forward_list>

class BaseCollider;

class CollisionManager
{
public:// 静的メンバ関数
	static CollisionManager* GetInstance();

public:// メンバ関数
	// コライダーの追加
	inline void AddCollider(BaseCollider* collider)
	{
		broadColliders.push_front(collider);
	}

	// コライダーの削除
	inline void RemoveBroadCollider(BaseCollider* collider)
	{
		broadColliders.remove(collider);
	}

	// 全ての衝突チェック
	void CheckBroadCollisions();

	//二つの衝突チェック
	bool CheckHitCollision(BaseCollider *colA, BaseCollider *colB, bool isBroad = false);

	// レイキャスト
	//bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	/*bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);*/

	// 球による衝突全検索
	void QuerySphere(const Sphere& sphere, QueryCallback* callback);

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;

	//ブロードフェイズ用のコライダーのリスト
	std::forward_list<BaseCollider*> broadColliders;
};