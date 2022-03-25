#include "GameObjCommon.h"
#include "DirectXCommon.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

GameObjCommon::~GameObjCommon()
{
	CollisionManager::GetInstance()->RemoveCollider(collider);
}

void GameObjCommon::Initialize()
{
}

void GameObjCommon::Update()
{

	if (collider) {
		collider->Update();
	}
}

void GameObjCommon::Move()
{
	if (isGravity) {
		velocity.y -= gravityPow;
	}
	pos += velocity;
}

void GameObjCommon::VelocityReset()
{
	velocity = 0;
}

void GameObjCommon::VelocityReset(bool isStap, float gain)
{
	if (isStap) {
		velocity = 0;
	}
	else {
		velocity *= gain;
	}
}


void GameObjCommon::Adaptation()
{

	objectData->SetScale(scale);
	objectData->SetPosition(pos);
	objectData->SetRotation(rotate);

	objectData->Update();

	// �����蔻��X�V
	if (collider)
	{
		collider->Update();
	}
}

void GameObjCommon::Draw() const
{
	if (isInvisible) return;	//�����ȂƂ��ƍ폜�^�C�~���O�ł͕`�悵�Ȃ�
	objectData->Draw(DirectXCommon::GetInstance()->GetCommandList());
}

void GameObjCommon::SetCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	this->collider = collider;
	// �R���W�����}�l�[�W���ɒǉ�
	CollisionManager::GetInstance()->AddCollider(collider);
	//�s��̍X�V
	//objectData->UpdateWorldMatrix();
	//�R���C�_�[���X�V���Ă���
	collider->Update();
}

