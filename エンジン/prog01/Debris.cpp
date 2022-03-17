#include "Debris.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "SlimeMath.h"

std::vector<Debris *> Debris::debris;

Debris::Debris(XMFLOAT3 startPos, Vector3 startVec, float size):
	pos(startPos),
	moveVec(startVec),
	size(size),
	isAttack(true),
	isAlive(true)
{
	// ������
	FbxObject3d::Initialize();
	//���f���̃Z�b�g
	SetModel(ModelManager::GetIns()->GetModel(SLIME));
	//�T�C�Y����X�P�[���փR���o�[�g
	SetScale(SizeToScaleConvert(size));

	collider.sphere.radius = size + 10.0f;
}

void Debris::Update()
{
	//��~����
	if (moveVec.Length() <= 0.5f) {
		isAttack = false;
		moveVec = { 0,0,0 };
	}
	//��������
	else {
		pos += moveVec;
		moveVec = moveVec * 0.9f;
	}
	
	collider.sphere.center = pos;
	collider.sphere.radius = size + 10.0f;
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
	for (int i = debris.size() - 1; i >= 0; i--) {
		if (!debris[i]->isAlive) {
			delete debris[i];
			debris.erase(debris.begin() + i);
		}
	}
}

void Debris::StaticDraw()
{
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->Draw();
	}
}

float Debris::Absorbed()
{
	isAlive = false;
	return size;
}
