#include "Weapon.h"

Weapon* Weapon::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Weapon* instance = new Weapon();
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
	//挙動↓↓↓


	// 行列の更新など
	Object3d::Update();
}