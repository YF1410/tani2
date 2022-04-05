#include "Collision.h"
#include "Vector3.h"
using namespace DirectX;

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
	DirectX::XMVECTOR* inter)
{
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
	//距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}
	//擬似交点を計算
	if (inter)
	{
		//平面上の最近接点を擬似交点とする
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
	const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
	DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	XMVECTOR p;
	//球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	//点pと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.center;
	//距離の二乗を求める
	//(同じベクトル同士の内積は三平方の定理のルート内部の式と一致する)
	v = XMVector3Dot(v, v);
	//球と三角形の距離が半径以下なら当たっていない
	if (v.m128_f32[0] > sphere.radius * sphere.radius)
	{
		return false;
	}
	//擬似交点を計算
	if (inter)
	{
		//三角形上の最近接点pを擬似交点とする
		*inter = p;
	}
	// 押し出すベクトルを計算
	if (reject)
	{
		float ds = XMVector3Dot(sphere.center, triangle.normal).m128_f32[0];
		float dt = XMVector3Dot(triangle.p0, triangle.normal).m128_f32[0];
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}

	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane,
	float* distance, DirectX::XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f; //誤差吸収用の微小な値
	//面法線とレイの方向ベクトルの内積
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	//裏面には当たらない
	if (d1 > -epsilon)
	{
		return false;
	}
	//始点と原点の距離(平面の法線方向)
	//面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//始点と平面の距離(平面の法線方向)
	float dist = d2 - plane.distance;
	//始点と平面の距離(レイ方向)
	float t = dist / -d1;
	//交点が始点より後ろにあるので、当たらない
	if (t < 0)
	{
		return false;
	}
	//距離を書き込む
	if (distance)
	{
		*distance = t;
	}
	//交点を計算
	if (inter)
	{
		*inter = ray.start + t * ray.dir;
	}

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
	float* distance, DirectX::XMVECTOR* inter)
{
	//三角形が乗っている平面を算出
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	//レイと平面が当たっていなければ、当たっていない
	if (!CheckRay2Plane(ray, plane, distance, &interPlane))
	{
		return false;
	}
	//レイと平面が当たっていたので、距離と交点が書き込まれた
	//レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f; //誤差吸収用の微小な値
	XMVECTOR m;
	//辺p0_p1について
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}
	//辺p1_p2について
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}
	//辺p2_p0について
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}

	//内側なので、当たっている
	if (inter)
	{
		*inter = interPlane;
	}

	return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
	float* distance, DirectX::XMVECTOR* inter)
{
	XMVECTOR m = ray.start - sphere.center;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	//layの始点がsphereの外側にあり(c > 0)、layがsphereから離れていく方向を差している場合(b > 0)、当たらない
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b * b - c;
	//負の判別式はレイが球を外れていることに一致
	if (discr < 0.0f)
	{
		return false;
	}

	//レイは球と交差している
	//交差する最小の値tを計算
	float t = -b - sqrtf(discr);
	//tが負である場合、レイは球の内側から開始しているのでtをゼロにクランプ
	if (t < 0)
	{
		t = 0.0f;
	}

	if (distance)
	{
		*distance = t;
	}

	if (inter)
	{
		*inter = ray.start + t * ray.dir;
	}

	return true;
}

bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB,
	DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	//中心点の距離の２乗 <= 半径の和の２乗　なら交差
	float dist = XMVector3LengthSq(sphereA.center - sphereB.center).m128_f32[0];

	float radius2 = sphereA.radius + sphereB.radius;
	radius2 *= radius2;

	if (dist <= radius2)
	{
		if (inter)
		{
			//Aの半径が0の時座標はBの中心　Bの半径が0の時座標はAの中心　となるよう補完
			float t = sphereB.radius / (sphereA.radius + sphereB.radius);
			*inter = XMVectorLerp(sphereA.center, sphereB.center, t);
		}
		// 押し出すベクトルを計算
		if (reject)
		{
			float rejectLen = sphereA.radius + sphereB.radius - sqrtf(dist);
			*reject = XMVector3Normalize(sphereA.center - sphereB.center);
			*reject *= rejectLen;
		}
		return true;
	}

	return false;
}

bool Collision::CheckSphere2Capsule(const Sphere& sphere, const Capsule& capsule)
{
	Vector3 vStartToEnd = capsule.endPosition - capsule.startPosition;

	Vector3 n = vStartToEnd;
	n.Normal();

	Vector3 sPos = { sphere.center.m128_f32[0], sphere.center.m128_f32[1], sphere.center.m128_f32[2] };

	float t = n.VDot(Vector3(sPos - capsule.startPosition));

	Vector3 vPsPn = n * t;
	Vector3 posPn = capsule.startPosition + vPsPn;

	float lengthRate = t / vStartToEnd.Length();

	float distance;
	if (lengthRate < 0.0f)
	{
		distance = Vector3(sPos + capsule.startPosition).Length() - capsule.radius;
	}
	else if (lengthRate <= 1.0f)
	{
		distance = Vector3(posPn - sPos).Length() - capsule.radius;
	}
	else
	{
		distance = Vector3(sPos + capsule.endPosition).Length() - capsule.radius;
	}

	return distance < sphere.radius;
}

bool Collision::CheckSphere2Box(const Sphere& sphere, const Box& box)
{
	float sqDistance = 0.0f;
	float pos;

	pos = sphere.center.m128_f32[0];
	if (pos < box.center.m128_f32[0] - box.scale.x)
	{
		sqDistance += (pos - box.center.m128_f32[0] - box.scale.x) * (pos - box.center.m128_f32[0] - box.scale.x);
	}
	else if (pos > box.center.m128_f32[0] + box.scale.x)
	{
		sqDistance += (pos - box.center.m128_f32[0] + box.scale.x) * (pos - box.center.m128_f32[0] + box.scale.x);
	}

	pos = sphere.center.m128_f32[1];
	if (pos < box.center.m128_f32[1] - box.scale.y)
	{
		sqDistance += (pos - box.center.m128_f32[1] - box.scale.y) * (pos - box.center.m128_f32[1] - box.scale.y);
	}
	else if (pos > box.center.m128_f32[1] + box.scale.y)
	{
		sqDistance += (pos - box.center.m128_f32[1] + box.scale.y) * (pos - box.center.m128_f32[1] + box.scale.y);
	}

	pos = sphere.center.m128_f32[2];
	if (pos < box.center.m128_f32[2] - box.scale.z)
	{
		sqDistance += (pos - box.center.m128_f32[2] - box.scale.z) * (pos - box.center.m128_f32[2] - box.scale.z);
	}
	else if (pos > box.center.m128_f32[2] + box.scale.z)
	{
		sqDistance += (pos - box.center.m128_f32[2] + box.scale.z) * (pos - box.center.m128_f32[2] + box.scale.z);
	}

	return sqDistance < sphere.radius* sphere.radius;
}

bool Collision::CheckSphere2AABB(const Sphere& sphere, const AABB& aabb, DirectX::XMVECTOR* inter)
{
	//長さのべき乗
	float sqLen = 0;
	Vector3 spherePos = sphere.center;
	if (spherePos.x < aabb.center.x) {
		sqLen += (spherePos.x - aabb.center.x) * (spherePos.x - aabb.center.x);
	}
	else if (spherePos.x > aabb.center.x + aabb.length.x) {
		sqLen += (spherePos.x - (aabb.center.x + aabb.length.x)) * (spherePos.x - (aabb.center.x + aabb.length.x));
	}
	if (spherePos.y < aabb.center.y) {
		sqLen += (spherePos.y - aabb.center.y) * (spherePos.y - aabb.center.y);
	}
	else if (spherePos.y > aabb.center.y + aabb.length.y) {
		sqLen += (spherePos.y - (aabb.center.y + aabb.length.y)) * (spherePos.y - (aabb.center.y + aabb.length.y));
	}
	if (spherePos.z < aabb.center.z) {
		sqLen += (spherePos.z - aabb.center.z) * (spherePos.z - aabb.center.z);
	}
	else if (spherePos.z > aabb.center.z + aabb.length.z) {
		sqLen += (spherePos.z - (aabb.center.z + aabb.length.z)) * (spherePos.z - (aabb.center.z + aabb.length.z));
	}

	//衝突
	if (sqLen < sphere.radius * sphere.radius) {
		if (inter) {
			Vector3 checkPos = spherePos - aabb.center;
			//衝突位置
			if (checkPos.x < aabb.length.x) {
				inter->m128_f32[0] = aabb.length.x;
			}
			else {
				inter->m128_f32[0] = spherePos.x;
			}

			if (checkPos.y < aabb.length.y) {
				inter->m128_f32[1] = aabb.length.y;
			}
			else {
				inter->m128_f32[1] = spherePos.y;
			}

			if (checkPos.z < aabb.length.z) {
				inter->m128_f32[2] = aabb.length.z;
			}
			else {
				inter->m128_f32[2] = spherePos.z;
			}
		}
		return true;
	}
	return false;
}

bool Collision::CheckCapsule2Box(const Capsule& capsule, const Box& box)
{
	Vector3 vStartToEnd = capsule.endPosition - capsule.startPosition;

	Vector3 n = vStartToEnd;
	n.Normal();

	Vector3 boxMinPos = { box.center.m128_f32[0] - box.scale.x, box.center.m128_f32[1] - box.scale.y, box.center.m128_f32[2] - box.scale.z };
	Vector3 boxMaxPos = { box.center.m128_f32[0] + box.scale.x, box.center.m128_f32[1] + box.scale.y, box.center.m128_f32[2] + box.scale.z };

	float t = n.VDot(Vector3(boxMaxPos - capsule.startPosition));

	Vector3 vPsPn = n * t;
	Vector3 posPn = capsule.startPosition + vPsPn;

	float lengthRate = t / vStartToEnd.Length();

	float sqDistance = 0.0f;
	float pos;
	if (lengthRate < 0.0f)
	{
		pos = capsule.startPosition.x;
		if (pos < boxMinPos.x)
		{
			sqDistance += (pos - boxMinPos.x) * (pos - boxMinPos.x);
		}
		else if (pos > boxMaxPos.x)
		{
			sqDistance += (pos - boxMaxPos.x) * (pos - boxMaxPos.x);
		}

		pos = capsule.startPosition.y;
		if (pos < boxMinPos.y)
		{
			sqDistance += (pos - boxMinPos.y) * (pos - boxMinPos.y);
		}
		else if (pos > boxMaxPos.y)
		{
			sqDistance += (pos - boxMaxPos.y) * (pos - boxMaxPos.y);
		}

		pos = capsule.startPosition.z;
		if (pos < boxMinPos.z)
		{
			sqDistance += (pos - boxMinPos.z) * (pos - boxMinPos.z);
		}
		else if (pos > boxMaxPos.z)
		{
			sqDistance += (pos - boxMaxPos.z) * (pos - boxMaxPos.z);
		}
	}
	else if (lengthRate <= 1.0f)
	{
		pos = posPn.x;
		if (pos < boxMinPos.x)
		{
			sqDistance += (pos - boxMinPos.x) * (pos - boxMinPos.x);
		}
		else if (pos > boxMaxPos.x)
		{
			sqDistance += (pos - boxMaxPos.x) * (pos - boxMaxPos.x);
		}

		pos = posPn.y;
		if (pos < boxMinPos.y)
		{
			sqDistance += (pos - boxMinPos.y) * (pos - boxMinPos.y);
		}
		else if (pos > boxMaxPos.y)
		{
			sqDistance += (pos - boxMaxPos.y) * (pos - boxMaxPos.y);
		}

		pos = posPn.z;
		if (pos < boxMinPos.z)
		{
			sqDistance += (pos - boxMinPos.z) * (pos - boxMinPos.z);
		}
		else if (pos > boxMaxPos.z)
		{
			sqDistance += (pos - boxMaxPos.z) * (pos - boxMaxPos.z);
		}
	}
	else
	{
		pos = capsule.endPosition.x;
		if (pos < boxMinPos.x)
		{
			sqDistance += (pos - boxMinPos.x) * (pos - boxMinPos.x);
		}
		else if (pos > boxMaxPos.x)
		{
			sqDistance += (pos - boxMaxPos.x) * (pos - boxMaxPos.x);
		}

		pos = capsule.endPosition.y;
		if (pos < boxMinPos.y)
		{
			sqDistance += (pos - boxMinPos.y) * (pos - boxMinPos.y);
		}
		else if (pos > boxMaxPos.y)
		{
			sqDistance += (pos - boxMaxPos.y) * (pos - boxMaxPos.y);
		}

		pos = capsule.endPosition.z;
		if (pos < boxMinPos.z)
		{
			sqDistance += (pos - boxMinPos.z) * (pos - boxMinPos.z);
		}
		else if (pos > boxMaxPos.z)
		{
			sqDistance += (pos - boxMaxPos.z) * (pos - boxMaxPos.z);
		}
	}

	return sqDistance < capsule.radius* capsule.radius;
}

//bool Collision::CheckLine2Box(const Line& line, const AABB& aabb)
//{
//	//bool isCollision = ColRayAABB(lineStart, &(*lineEnd - *lineStart), &aabb[i], &box_WorldMat[i], col_t);
//
//	// 交差判定
//	float p[3], d[3], min[3], max[3];
//	p[0] = line.start.x;
//	p[1] = line.start.y;
//	p[2] = line.start.z;
//
//	d[0] = line.end.x - line.start.x;
//	d[1] = line.end.y - line.start.y;
//	d[2] = line.end.z - line.start.z;
//
//	min[0] = aabb.center.x - aabb.length.x;
//	min[1] = aabb.center.y - aabb.length.y;
//	min[2] = aabb.center.z - aabb.length.z;
//
//	max[0] = aabb.center.x + aabb.length.x;
//	max[1] = aabb.center.y + aabb.length.y;
//	max[2] = aabb.center.z + aabb.length.z;
//
//	float t = -FLT_MAX;
//	float t_max = FLT_MAX;
//
//	for (int i = 0; i < 3; ++i) {
//		if (abs(d[i]) < FLT_EPSILON) {
//			if (p[i] < min[i] || p[i] > max[i])
//				return false;	// 交差していない
//		}
//		else {
//			// スラブとの距離を算出
//			// t1が近スラブ、t2が遠スラブとの距離
//			float odd = 1.0f / d[i];
//			float t1 = (min[i] - p[i]) * odd;
//			float t2 = (max[i] - p[i]) * odd;
//			if (t1 > t2) {
//				float tmp = t1; t1 = t2; t2 = tmp;
//			}
//
//			if (t1 > t) t = t1;
//			if (t2 < t_max) t_max = t2;
//
//			// スラブ交差チェック
//			if (t >= t_max)
//				return false;
//		}
//	}
//	Vector3 colPos;
//	// 交差している
//	colPos = line.start + t * (line.end - line.start);
//
//	return true;
//}
