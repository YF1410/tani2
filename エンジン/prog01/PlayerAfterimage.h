#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>
#include "GameObjCommon.h"
#include "PlayerObject.h"

class PlayerAfterimage {
public:
	PlayerAfterimage(FbxObject3d player);
	~PlayerAfterimage();
	void Update();
	void LustUpdate();

	static void StaticUpdate();
	static void StaticLustUpdate();
	static void StaticDraw();
	static void Finalize();

	//ƒRƒ“ƒeƒi
	static std::vector<PlayerAfterimage*> playerAfterimages;
	
	std::unique_ptr<FbxObject3d> playerObj;

	bool isAlive;
	float alpha;
};

