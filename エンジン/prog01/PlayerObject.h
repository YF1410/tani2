#pragma once
#include "Object3d.h"
#include "Slime.h"
#include "Weapon.h"

class PlayerObject
{
public:
	PlayerObject(Model* model = nullptr, Model* model2 = nullptr);
	~PlayerObject();
	// ���t���[������
	void Update();
	// �`��
	void Draw();

private: // �����o�ϐ�
	Slime* slime = nullptr;
	Weapon* weapon = nullptr;
};

