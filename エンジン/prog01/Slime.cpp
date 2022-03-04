#include "Slime.h"

Slime* Slime::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Slime* instance = new Slime();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// 初期化
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
	//挙動↓↓↓
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

	// 行列の更新など
	Object3d::Update();
}