#pragma once
#include "Object3d.h"

class Weapon : public Object3d
{
public:
	// 3D�I�u�W�F�N�g����
	static Weapon* Create(Model* model = nullptr);

public:
	// ������
	bool Initialize() override;
	// ���t���[������
	void Update() override;
};