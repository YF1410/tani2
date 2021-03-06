#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:
	//球と平面の当たり判定
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
		DirectX::XMVECTOR* inter = nullptr);
	//点と三角形の最近接点を求める
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
		const Triangle& triangle, DirectX::XMVECTOR* closest);
	//球と法線付き三角形の当たり判定
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	//レイと平面の当たり判定
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	//レイと法線付き三角形の当たり判定
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	//レイと球の当たり判定
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	//球と球の当たり判定
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB,
		DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	//球と点の当たり判定
	static bool CheckSphere2Point(const Sphere &sphere,const Vector3 &point,
		DirectX::XMVECTOR *inter = nullptr, DirectX::XMVECTOR *reject = nullptr);
	//球とカプセルの当たり判定
	static bool CheckSphere2Capsule(const Sphere& sphere, const Capsule& capsule);
	//球とボックスの当たり判定
	static bool CheckSphere2Box(const Sphere& sphere, const Box& box);
	//球とAABBの当たり判定
	static bool CheckSphere2AABB(const Sphere &sphere, const AABB &aabb, DirectX::XMVECTOR *inter= nullptr);
	//カプセルとボックスの当たり判定
	static bool CheckCapsule2Box(const Capsule& capsule, const Box& box);

	static bool CheckLine2Box(const Line& line, const AABB& aabb);
};