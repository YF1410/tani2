#include "Debris.h"
#include "ModelManager.h"
#include "DirectXCommon.h"

std::vector<Debris *> Debris::debris;

Debris::Debris(XMFLOAT3 startPos, Vector3 startVec, float size):
	pos(startPos),
	moveVec(startVec),
	size(size)
{
	// ������
	FbxObject3d::Initialize();
	SetScale({ 10,10,10 });
	//���f���̃Z�b�g
	SetModel(ModelManager::GetIns()->GetModel(SLIME_BREAK));

}

void Debris::Update()
{
	pos += moveVec;
	moveVec = moveVec * 0.9f;
	
	
	SetPosition(pos);

	FbxObject3d::Update();
}

void Debris::Draw()
{
	FbxObject3d::Draw(DirectXCommon::GetInstance()->GetCommandList());
}

void Debris::StaticUpdate()
{
	//�X�V
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
