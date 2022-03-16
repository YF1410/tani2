#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>

class Debris :
	public FbxObject3d
{
public:
	Debris(XMFLOAT3 startPos,Vector3 startVec,float size);
	void Update();
	void Draw();

	static void StaticUpdate();
	static void StaticDraw();

	static std::vector<Debris *> debris;
private:
	Vector3 pos;
	Vector3 moveVec;
	float size;
};

