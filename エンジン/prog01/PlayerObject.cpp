#include "PlayerObject.h"

PlayerObject::PlayerObject(Model* model, Model* model2)
{
	slime = Slime::Create(model);
	weapon = Weapon::Create(model2);
}

PlayerObject::~PlayerObject()
{
	delete slime;
	delete weapon;
}

void PlayerObject::Update()
{

	slime->Update();
	weapon->Update();
}

void PlayerObject::Draw()
{
	slime->Draw();
	weapon->Draw();
}