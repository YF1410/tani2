#include "Weapon.h"

Weapon* Weapon::Create(Model* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Weapon* instance = new Weapon();
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

bool Weapon::Initialize()
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	return true;
}

void Weapon::Update()
{
	//����������


	// �s��̍X�V�Ȃ�
	Object3d::Update();
}