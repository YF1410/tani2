#include "GameObjCommon.h"
#include "DirectXCommon.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

GameObjCommon::~GameObjCommon()
{
	//�u���[�h�t�F�C�Y�R���C�_�[�폜
	if (broadColliders.size() != 0)
	{
		auto iter = broadColliders.begin();
		while (iter != broadColliders.end()) {
			CollisionManager::GetInstance()->RemoveBroadCollider(iter->second);
			++iter;
		}
	}
	
}

void GameObjCommon::Initialize()
{
}

void GameObjCommon::Update()
{

	//�u���[�h�t�F�C�Y�R���C�_�[����X�V
	if (broadColliders.size() != 0)
	{
		auto iter = broadColliders.begin();
		while (iter != broadColliders.end()) {
			iter->second->Update();
			++iter;
		}
	}
	//�i���[�t�F�C�Y�R���C�_�[����X�V
	if (narrowColliders.size() != 0)
	{
		auto iter = narrowColliders.begin();
		while (iter != narrowColliders.end()) {
			iter->second->Update();
			++iter;
		}
	}
}

void GameObjCommon::FixedUpdate()
{
}

void GameObjCommon::LustUpdate()
{
}

void GameObjCommon::PosAddVelocity()
{
	oldPos = pos;
	if (isGravity) {
		velocity.y -= gravityPow;
	}
	pos += velocity;
	savePos += velocity;

	GameObjCommon::Update();

}

void GameObjCommon::VelocityReset()
{
	velocity = 0;
}

void GameObjCommon::VelocityReset(float gain, float dead)
{
	velocity *= gain;
	if (velocity.x < dead && -dead < velocity.x) {
		velocity.x = 0;
	}
	if (velocity.y < dead && -dead < velocity.y) {
		velocity.y = 0;
	}
	if (velocity.z < dead && -dead < velocity.z) {
		velocity.z = 0;
	}
}


void GameObjCommon::Adaptation()
{

	objectData->SetScale(scale);
	objectData->SetPosition(pos);
	objectData->SetRotation(rotate);

	objectData->Update();

	//�u���[�h�t�F�C�Y�R���C�_�[����X�V
	if (broadColliders.size() != 0)
	{
		auto iter = broadColliders.begin();
		while (iter != broadColliders.end()) {
			iter->second->Update();
			++iter;
		}
	}
	//�i���[�t�F�C�Y�R���C�_�[����X�V
	if (narrowColliders.size() != 0)
	{
		auto iter = narrowColliders.begin();
		while (iter != narrowColliders.end()) {
			iter->second->Update();
			++iter;
		}
	}
}

void GameObjCommon::Draw() const
{
	if (isInvisible) return;	//�����ȂƂ��ƍ폜�^�C�~���O�ł͕`�悵�Ȃ�
	Vector3 offsetPos = pos + Vector3{2500,0,0};
	objectData->SetPosition(offsetPos);
	objectData->Update();

	objectData->Draw(DirectXCommon::GetInstance()->GetCommandList());
}

void GameObjCommon::SetBroadCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	broadColliders[collider->GetCollisionName()] = collider;
	// �R���W�����}�l�[�W���ɒǉ�
	CollisionManager::GetInstance()->AddCollider(collider);
	//�s��̍X�V
	//objectData->UpdateWorldMatrix();
	//�R���C�_�[���X�V���Ă���
	collider->Update();
}

void GameObjCommon::SetNarrowCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	narrowColliders[collider->GetCollisionName()] = collider;
	collider->Update();
}