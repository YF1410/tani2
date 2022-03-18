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
	isAlive(true),
	isStop(false),
	airResistance(0,0,0)
{
	// 初期化
	debriObj = FbxObject3d::Create(ModelManager::GetIns()->GetModel(SLIME));
	//サイズからスケールへコンバート
	scale = ConvertSizeToScale(size);
	UpdateCollider();
}

void Debris::Update()
{
	//完全に停止
	if (moveVec.Length() <= 1.0f) {
		isStop = true;
		moveVec = { 0,0,0 };
	}
	//空気抵抗処理
	else {
		airResistance = moveVec * 0.1f;
		moveVec -= airResistance;
	}
	//攻撃終了
	if (moveVec.Length() <= 10.0f) {
		isAttack = false;
	}
	
	
	afterPos = pos + moveVec;
	//当たり判定更新
	UpdateCollider();
}

void Debris::Reflection()
{
	//描画位置決定
	pos = afterPos;
	debriObj->SetPosition(pos);
	debriObj->SetScale(scale);
	//全て適応
	debriObj->Update();
}

void Debris::Draw()
{
	debriObj->Draw(DirectXCommon::GetInstance()->GetCommandList());
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

void Debris::StaticReflection()
{
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->Reflection();
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
	//移動後の位置予測
	afterPos = pos + moveVec;
	//見た目が大事用
	collider.realSphere.center = afterPos;
	collider.realSphere.radius = scale * 150.0f;
	//吸収用
	collider.absorbedSphere.center = afterPos;
	collider.absorbedSphere.radius = scale * 150.0f;
}

void Debris::Bounse(
	const XMVECTOR &hitPos,		//衝突位置
	const Vector3 &normal	//衝突した物との向きベクトル
)
{
	pos = hitPos + normal * collider.realSphere.radius;
	moveVec = CalcReflectVector(moveVec, normal)*1.5f;
	UpdateCollider();
}

void Debris::SuckedPlayer(const Vector3 &playerPos,const float &suckedRadius)
{
	//引き寄せられる速度
	float suckedPow =  Vector3(playerPos - pos).Length() - suckedRadius;
	if (suckedPow < 0) { return; }
	moveVec += Vector3(pos - playerPos).Normalize() * suckedPow * 0.01f;
}

float Debris::AbsorbedToPlayer()
{
	isAlive = false;
	return size;
}
