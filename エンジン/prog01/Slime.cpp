#include "Slime.h"

Slime* Slime::Create(Model* model)
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
	if (!Object3d::Initialize())
	{
		return false;
	}

	return true;
}

void Slime::Update()
{
	Input* input = Input::GetInstance();
	//����������
	if (input->PushKey(DIK_A))
	{
		position.x -= 0.2f;
	}
	else if (input->PushKey(DIK_D))
	{
		position.x += 0.2f;
	}

	if (input->PushKey(DIK_W))
	{
		position.y -= 0.2f;
	}
	else if (input->PushKey(DIK_S))
	{
		position.y += 0.2f;
	}

	// �s��̍X�V�Ȃ�
	Object3d::Update();
}