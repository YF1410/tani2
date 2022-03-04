#include "PlayerObject.h"
#include "Input.h"

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
	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_SPACE))
	{
		if (!flag)
		{
			flag = true;
		}
		else
		{
			flag = false;
		}
		angle += 180;
	}

	if (input->PushKey(DIK_A))
	{
		angle -= 2.0f;
	}
	//”½ŽžŒv‰ñ‚è‚É’nã‚ðˆÚ“®
	else if (input->PushKey(DIK_D))
	{
		angle += 2.0f;
	}

	//‰ñ“]‚Ì•
	if (input->PushKey(DIK_S) && len >= 2.0f)
	{
		len -= 0.2f;
	}
	//”½ŽžŒv‰ñ‚è‚É’nã‚ðˆÚ“®
	else if (input->PushKey(DIK_W) && len <= 8.0f)
	{
		len += 0.2f;
	}

	XMFLOAT3 pPos = slime->GetPosition();
	sphereOBJ->SetPosition({ pPos.x, pPos.y + 0.6f, pPos.z });

	//‰ñ“]
	float rad = (angle * 3.14159265359f / 180.0f) * -1;
	float aroundX = cos(rad) * len / 1.0f;
	float aroundY = sin(rad) * len / 1.0f;
	if (!flag)
	{
		XMFLOAT3 wPos = weapon->GetPosition();
		wPos.x = aroundX + pPos.x;
		wPos.y = aroundY + pPos.y + 0.5f;
		weapon->SetPosition(wPos);
	}
	else if(flag)
	{
		XMFLOAT3 wPos = weapon->GetPosition();
		pPos.x = aroundX + wPos.x;
		pPos.y = aroundY + wPos.y - 0.5f;
		slime->SetPosition(pPos);
	}

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