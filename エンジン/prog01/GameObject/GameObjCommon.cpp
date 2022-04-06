#include "GameObjCommon.h"
#include "DirectXCommon.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

GameObjCommon::~GameObjCommon()
{
	//ブロードフェイズコライダー削除
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

	//ブロードフェイズコライダー判定更新
	if (broadColliders.size() != 0)
	{
		auto iter = broadColliders.begin();
		while (iter != broadColliders.end()) {
			iter->second->Update();
			++iter;
		}
	}
	//ナローフェイズコライダー判定更新
	if (narrowColliders.size() != 0)
	{
		auto iter = narrowColliders.begin();
		while (iter != narrowColliders.end()) {
			iter->second->Update();
			++iter;
		}
	}
}

void GameObjCommon::Move()
{
	oldPos = pos;
	if (isGravity) {
		velocity.y -= gravityPow;
	}
	pos += velocity;

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

	//ブロードフェイズコライダー判定更新
	if (broadColliders.size() != 0)
	{
		auto iter = broadColliders.begin();
		while (iter != broadColliders.end()) {
			iter->second->Update();
			++iter;
		}
	}
	//ナローフェイズコライダー判定更新
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
	if (isInvisible) return;	//透明なときと削除タイミングでは描画しない
	objectData->Draw(DirectXCommon::GetInstance()->GetCommandList());
}

void GameObjCommon::SetBroadCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	broadColliders[collider->GetCollisionName()] = collider;
	// コリジョンマネージャに追加
	CollisionManager::GetInstance()->AddCollider(collider);
	//行列の更新
	//objectData->UpdateWorldMatrix();
	//コライダーを更新しておく
	collider->Update();
}

void GameObjCommon::SetNarrowCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	narrowColliders[collider->GetCollisionName()] = collider;
	collider->Update();
}