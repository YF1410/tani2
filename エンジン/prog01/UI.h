#pragma once
#include "Sprite.h"

#include "PlayerObject.h"

class UI
{
public:
	void Upadate();
private:
	Sprite *shotCount[4];

	//�v���C���[�f�[�^
	PlayerObject *player;
};

