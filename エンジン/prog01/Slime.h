#pragma once
#include "FbxObject3d.h"
#include "Input.h"

class Slime : public FbxObject3d
{
public:
	// 3D�I�u�W�F�N�g����
	static Slime* Create(FbxModel* model = nullptr);

public:
	// ������
	bool Initialize();
	// ���t���[������
	void Update();
};