#include "PlayerObject.h"

using namespace DirectX;

PlayerObject::PlayerObject(Model* model, Model* model2)
{
	slime = Slime::Create(model);
	weapon = Weapon::Create(model2);
	weapon->SetPosition({ 5,0,0 });
	sphereModel = Model::CreateFromObject("sphere", true);
	sphereModel->SetAlpha(0.5f);
	sphereOBJ = Object3d::Create(sphereModel.get());
	sphereOBJ->SetPosition({ 0, 0.6f, 0 });
}

PlayerObject::~PlayerObject()
{
	delete slime;
	delete weapon;
}

void PlayerObject::Update()
{

	XMFLOAT3 pos = slime->GetPosition();
	sphereOBJ->SetPosition({ pos.x, pos.y + 0.6f, pos.z });

	slime->Update();
	weapon->Update();
	sphereOBJ->Update();
}

void PlayerObject::Draw()
{
	slime->Draw();
	weapon->Draw();
	sphereOBJ->Draw();
}