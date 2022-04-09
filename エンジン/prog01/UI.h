#pragma once
#include "Sprite.h"

#include "PlayerObject.h"

class UI
{
public:
	void Upadate();
private:
	Sprite *shotCount[4];

	//プレイヤーデータ
	PlayerObject *player;
};

