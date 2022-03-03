#pragma once
#include "Object3d.h"
#include "Slime.h"
#include "Weapon.h"

class PlayerObject
{
public:
	PlayerObject(Model* model = nullptr, Model* model2 = nullptr);
	~PlayerObject();
	// –ˆƒtƒŒ[ƒ€ˆ—
	void Update();
	// •`‰æ
	void Draw();

private: // ƒƒ“ƒo•Ï”
	Slime* slime = nullptr;
	Weapon* weapon = nullptr;
};

