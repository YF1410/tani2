#include "Debris.h"
#include "ModelManager.h"
#include "DirectXCommon.h"

std::vector<Debris *> Debris::debris;

Debris::Debris(XMFLOAT3 startPos, Vector3 startVec, float size):
	pos(startPos),
	moveVec(startVec),
	size(size),
	isAttack(true)
{
	// 初期化
	FbxObject3d::Initialize();
	//モデルのセット
	SetModel(ModelManager::GetIns()->GetModel(SLIME));
	SetScale(size * 0.2f);
}

void Debris::Update()
{
	pos += moveVec;
	//停止処理
	if (moveVec.Length() <= 0.5f) {
		isAttack = false;
		moveVec = { 0,0,0 };
	}
	//原則処理
	else {
		moveVec = moveVec * 0.9f;
	}
	
	
	SetPosition(pos);

	FbxObject3d::Update();
}

void Debris::Draw()
{
	FbxObject3d::Draw(DirectXCommon::GetInstance()->GetCommandList());
}

void Debris::StaticUpdate()
{
	//更新
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->Update();
	}
}

void Debris::StaticDraw()
{
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->Draw();
	}
}
