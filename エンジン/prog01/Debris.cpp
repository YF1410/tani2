#include "Debris.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "SlimeMath.h"

std::vector<Debris *> Debris::debris;

Debris::Debris(Vector3 startPos, Vector3 startVec, float size) :
	GameObjCommon(
		ModelManager::SLIME_BREAK,
		GameObjCommon::DEBRIS,
		true,
		startPos
	),
	size(size),
	isAlive(true),
	isAttack(true)
{
	//サイズからスケールへコンバート
	scale = ConvertSizeToScale(size);
	//初期加速度セット
	velocity = startVec;
	UpdateCollider();
}

void Debris::Update()
{
	VelocityReset();

	//攻撃終了
	if (velocity.Length() <= 10.0f && isAttack) {
		isAttack = false;
	}
	
	
	//当たり判定更新
	UpdateCollider();
}

void Debris::VelocityReset()
{
	//空気抵抗
	airResistance = velocity * 0.01f;
	velocity -= airResistance;
	//重力
	velocity.y -= gravityPow;

}


void Debris::StaticUpdate()
{

	//削除
	for (int i = debris.size() - 1; i >= 0; i--) {
		if (!debris[i]->isAlive) {
			delete debris[i];
			debris.erase(debris.begin() + i);
		}
	}
	//更新
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->Update();
	}
}

void Debris::StaticAdaptation()
{
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->Adaptation();
	}
}

void Debris::StaticDraw()
{
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->Draw();
	}
}

void Debris::UpdateCollider()
{
	Move();
	//見た目nに近い判定
	collider.realSphere.center = pos;
	collider.realSphere.radius = scale.x * 150.0f;
	//攻撃用判定
	collider.attackSphere.center = pos;
	collider.attackSphere.radius = scale.x * 180.0f;

	//吸収用
	collider.hitSphere.center = pos;
	collider.hitSphere.radius = scale.x * 150.0f;
}

void Debris::Bounse(
	const Vector3 &hitPos,		//衝突位置
	const Vector3 &normal	//衝突した物との向きベクトル
)
{
	pos = hitPos + normal * collider.realSphere.radius;
	velocity = CalcReflectVector(velocity, normal);
	//跳ね返りが一定以下ならバウンドを停止
	if (velocity.Length() <= 10.0f) {
		velocity = 0;
	}

	UpdateCollider();
}

void Debris::SuckedPlayer(const Vector3 &playerPos,const float &suckedRadius)
{
	//移動開始
	velocity += Vector3(playerPos- pos).Normalize() * 3.0f;
}

float Debris::AbsorbedToPlayer()
{
	isAlive = false;
	return size;
}

void Debris::Damage(float damage)
{
	size -= damage;
	//HPが0以下になったら死亡状態へ以降
	if (size < 0) {
		isAlive = false;
	}
	else {
	}
}
