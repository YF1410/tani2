#include "Slime.h"

Slime* Slime::Create(FbxModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Slime* instance = new Slime();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	if (model)
	{
		instance->SetModel(model);
	}

	return instance;
}

bool Slime::Initialize()
{
	if (!FbxObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void Slime::Update()
{
	Input* input = Input::GetInstance();

	// �s��̍X�V�Ȃ�
	FbxObject3d::Update();
}