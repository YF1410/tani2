#include "GameObjCommon.h"
#include "DirectXCommon.h"

void GameObjCommon::Initialize()
{
}

void GameObjCommon::Update()
{
}

void GameObjCommon::MovePos()
{
	//ˆÚ“®“K‰ž
	pos += velocity;
}

void GameObjCommon::Reflection()
{

	object->SetScale(scale);
	object->SetPosition(pos);
	object->SetRotation(rotate);

	object->Update();
}

void GameObjCommon::Draw() const
{
	if (isInvisible || !isActive) return;
	object->Draw(DirectXCommon::GetInstance()->GetCommandList());
}

