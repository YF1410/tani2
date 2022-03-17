#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "Collision.h"

class Debris :
	public FbxObject3d
{
public:
	Debris(XMFLOAT3 startPos,Vector3 startVec,float size);
	void Update();
	void Draw();

	static void StaticUpdate();
	static void StaticDraw();

	//プレイヤーに吸収されたとき
	float Absorbed();


	//当たり判定
	struct COLLIDER {
		Sphere sphere;
	}collider;

	//生存フラグ
	bool isAlive;
	//攻撃状態
	bool isAttack;

	//コンテナ
	static std::vector<Debris *> debris;
private:
	Vector3 pos;
	Vector3 moveVec;
	//数値的な大きさ
	float size;

};

