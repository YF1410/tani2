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

	// 当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void GameObjCommon::Draw() const
{
	if (isInvisible) return;	//透明なときと削除タイミングでは描画しない
	objectData->Draw(DirectXCommon::GetInstance()->GetCommandList());
}

void GameObjCommon::SetCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	this->collider = collider;
	// コリジョンマネージャに追加
	CollisionManager::GetInstance()->AddCollider(collider);
	//行列の更新
	//objectData->UpdateWorldMatrix();
	//コライダーを更新しておく
	collider->Update();
}

