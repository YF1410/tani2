#include "Slime.h"

Slime* Slime::Create(FbxModel* model)
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
	if (!FbxObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void Slime::Update()
{
	Input* input = Input::GetInstance();

	// 行列の更新など
	FbxObject3d::Update();
}