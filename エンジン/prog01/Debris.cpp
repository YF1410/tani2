#include "Debris.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "SlimeMath.h"
#include "MapChip.h"

std::vector<Debris *> Debris::debris;

Debris::Debris(Vector3 startPos, Vector3 startVec, float size,Vector3 *playerPos, int reversAERA) :
	GameObjCommon(
		ModelManager::SLIME_BREAK,
		GameObjCommon::DEBRIS,
		false,
		startPos
	),
	size(size),
	isAlive(true),
	isAttack(true),
	isFirstAttack(true),
	reverseCenter(startPos),
	reversRagne(reversAERA)
{
	//サイズからスケールへコンバート
	scale = ConvertSizeToScale(size);
	//初期加速度セット
	velocity = startVec;
	//最初から攻撃状態
	state = ATTACK;
	//プレイヤーの位置セット
	this->playerPos = playerPos;

	////マップチップ用コライダー
	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, scale.x * 150.0f, scale.x*150.0f);
	SetNarrowCollider(toMapChipCollider);

	//押し返し判定用コライダー
	hitCollider = new SphereCollider("hitCollider");
	hitCollider->SetRadius(scale.x * 180.0f);
	hitCollider->SetOffset({ 0,hitCollider->GetRadius(),0 });
	SetBroadCollider(hitCollider);
	//攻撃判定用コライダー
	attackCollider = new SphereCollider("attackCollider");
	attackCollider->SetRadius(scale.x * 150.0f);
	attackCollider->SetOffset({ 0,attackCollider->GetRadius(),0 });
	SetNarrowCollider(attackCollider);
	//残骸どうしは判定しない
	exclusionList.push_back(DEBRIS);
}

void Debris::Update()
{
	VelocityReset();
	//強制回収フラグがたったらステートを変更
	//攻撃終了
	if (velocity.Length() <= 10.0f && isAttack) {
		isFirstAttack = false;
	}
	else if (!isActive) {
		isAttack = true;
	}

	switch (state)
	{
	case Debris::NOUPDATE:
		break;
	case Debris::STAY:
		break;
	case Debris::ATTACK:
		
		break;
	case Debris::RETURN:
		velocity = Vector3(*playerPos- pos).Normal() * 100;
		if (returnTimer-- <= 0) {
			state = STAY;
		}
		break;
	default:
		break;
	}

	//移動量を適応
	Move();

	//マップチップとの当たり判定
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 oldPos;
	if (MapChip::GetInstance()->CheckHitMapChip(toMapChipCollider, &velocity, &hitPos)) {
		Vector3 normal = { 0,0,0 };

		if (hitPos.x != 0) {
			int vec = 1;	//向き
			if (0 < velocity.x) {
				vec = -1;
			}
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * vec;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//向き
			if (velocity.z < 0) {
				vec = -1;
			}
			pos.z = hitPos.z - toMapChipCollider->GetRadiusY() * vec;
			normal.z = vec;
		}
		normal.Normalize();
		HitWall(hitPos, normal);
	}

	//反転
	if (Vector3(reverseCenter - pos).Length() > reversRagne) {
		velocity *= -1;
	}
}

void Debris::VelocityReset()
{
	//空気抵抗
	airResistance = velocity * 0.02f;
	velocity -= airResistance;
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

void Debris::OnCollision(const CollisionInfo &info)
{
	switch (info.object->Tag)
	{
	case ENEMY:
		if (isAttack) {

		}
		else {
			Damage(1.0f);
		}
		break;
	case PLAYER:
		if (info.myName == "hitCollider" &&
			info.collider->GetCollisionName() == "absorptionCollider" &&
			!isFirstAttack) {
			velocity += Vector3(*playerPos - pos).Normalize() * 5.0f;
		}
		//当たり判定用のコライダーとプレイヤーの当たり判定コライダーが当たっていたら削除
		if (info.myName == "hitCollider" &&
			info.collider->GetCollisionName() == "hitCollider" &&
			!isFirstAttack) {
			isAlive = false;
		}
		break;
	}

}


void Debris::ReturnStart()
{
	//ステートをリターンに
	state = RETURN;
	velocity *= Vector3(pos - *playerPos).Normal();
	returnTimer = 120;
}

void Debris::SuckedPlayer(const Vector3 &playerPos,const float &suckedRadius)
{
	//移動開始
	velocity += Vector3(playerPos- pos).Normal() * 3.0f;
}

void Debris::HitWall(const XMVECTOR &hitPos, const Vector3 &normal)
{
	velocity = CalcReflectVector(velocity, normal);
}


void Debris::Damage(float damage)
{
	size -= damage;
	//Sizeが0以下になったら死亡状態へ以降
	if (size < 0) {
		isAlive = false;
	}
	else {
	}
}
