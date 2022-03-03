#pragma once
#include "Object3d.h"
#include "Slime.h"
#include "Weapon.h"

class PlayerObject
{
public:
	PlayerObject(Model* model = nullptr, Model* model2 = nullptr);
	~PlayerObject();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();

private: // メンバ変数
	Slime* slime = nullptr;
	Weapon* weapon = nullptr;
};

