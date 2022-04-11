#include "PlayerObject.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Debris.h"
#include "SlimeMath.h"
#include "DebugText.h"
#include "MapChip.h"


using namespace DirectX;

PlayerObject::PlayerObject(XMFLOAT3 startPos) :
	GameObjCommon(
		ModelManager::SLIME,	//スライムのモデルをセット
		GameObjCommon::PLAYER,	//プレイヤーとして扱う
		false,					//重力の影響を受ける
		{0,0,0},
		{0,0,0},
		{0,0,0},
		true
	)
{
	srand(time(NULL));			//爆破用乱数のシード値をセット
	//初期位置に設定
	this->startPos = startPos;
	pos = startPos;

	//当たり判定
	broadSphereCollider = new SphereCollider("hitCollider", { 0,scalef * 180.0f,0 }, scalef * 180.0f);
	SetBroadCollider(broadSphereCollider);

	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);
	//absorptionCollider->Update();

	//flontModel = Model::CreateFromObject("Sphere", true);
	//flontModel.get()->SetAlpha(0.2f);
	//flont = Object3d::Create(flontModel.get());
	//flont.get()->SetScale({ 10,10,10 });

	coreUp = new GameObjCommon(ModelManager::SLIME_CORE, Notag, false,{ 0,0,0 }, {1.5f,1.5f,1.5f });

}


void PlayerObject::Initialize()
{
	isCheckPoint = false;
	//サイズ初期化
	size = 500.0f;
	//サイズ初期化
	scalef = ConvertSizeToScale(size);
	objectData->SetScale(scalef);
	toMapChipCollider->SetRadius(scalef * 180.0f, scalef * 180.0f);
	//自爆フラグ
	destructType = DIRECTIVITY;
	//吸引範囲
	suction = scalef * suctionRatio;
	//ポジション初期化
	pos = startPos;

	canReturn = true;
	returnCounter = 0;
	objectData->SetAlpha(0.5f);

	//アニメーション開始
	objectData->PlayAnimation();

	//無敵フラグ


}

void PlayerObject::Update()
{
	Input *input = Input::GetInstance();
	//スケールから移動量決定
	moveSpead = scalef * 5;

	//移動量減衰処理
	VelocityReset(0.9f);
	if (velocity.Length() >= 1000) {
		velocity = velocity.Normal() * 1000;
	}
	if (isAttack && velocity.Length() < 10) {
		isAttack = false;
	}

	//リセット
	if (input->PushKey(DIK_R)) {
		Initialize();
	}

	//無敵処理
	if (isInvincible) {
		invincibleCounter--;
		if (invincibleCounter <= 0) {
			isInvincible = false;
		}
	}


	//キーボード移動
	if (input->PushKey(DIK_A))	//左
	{
		velocity.x -= moveSpead;
	}
	else if (input->PushKey(DIK_D))	//右
	{
		velocity.x += moveSpead;
	}
	if (input->PushKey(DIK_S))	//後ろ
	{
		velocity.z -= moveSpead;
	}
	else if (input->PushKey(DIK_W))	//前
	{
		velocity.z += moveSpead;

	}
	//コントローラーでの移動
	velocity.x += input->PadStickGradient().x * moveSpead;
	velocity.z += -input->PadStickGradient().y * moveSpead;


	//爆破タイプ切り替え
	if (input->TriggerKey(DIK_3) || input->TriggerPadButton(BUTTON_RIGHT_SHOULDER)) {
		if (destructType != CIRCLE) {
			destructType = CIRCLE;
		}
		else {
			destructType = DIRECTIVITY;
		}
	}
	



	//デバッグ用サイズ変更
	if (input->PushKey(DIK_UP)) {
		size += 10.0f;
	}
	if (input->PushKey(DIK_DOWN)) {
		size -= 10.0f;
	}

	//自爆
	if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(BUTTON_A))
	{
		//ショットに使う総合サイズ
		float maxSize = size * 0.5f;
		//爆発威力と爆発方向から破片の数を計算
		for (int i = 0; i < destructPow * destructType; i++) {
			Vector3 startVec;		//速度*向きベクトル
			float shotRad;			//角度決定用
			//発射スピード
			float shotSpeed = rand() % 20 + scalef * 60;
			//残骸のサイズ
			float shotSize = maxSize / (destructPow * destructType);

			switch (destructType)
			{
			case CIRCLE:		//円形爆発
				//放射角度を360度で計算
				shotRad = XMConvertToRadians(rand() % 360);
				startVec = {
					static_cast<float>(cos(shotRad)),
					0,
					static_cast<float>(sin(shotRad))
				};
				break;

			case DIRECTIVITY:	//指向性爆発
				//-15~15度で計算
				shotRad = XMConvertToRadians(rand() % 90 - 45);

				startVec = -velocity.Normal();

				startVec.AddRotationY(shotRad);
				//startVec = startVec + offset;
				break;
			default:
				break;
			}

			velocity += velocity.Normal() * 30;
			isAttack = true;
			//Debrisのコンテナに追加
			Debris::debris.push_back(new Debris(pos, startVec * shotSpeed, shotSize));
		}
		//爆発終了
		size -= maxSize;
		
	}

	//回収
	if ((input->TriggerKey(DIK_Q)|| input->TriggerPadButton(BUTTON_B))&&
		canReturn == true) {
		canReturn = false;
		returnCounter = 120;
		for (int i = 0; i < Debris::debris.size(); i++) {
			Debris::debris[i]->ReturnStart();
		}
	}
	if (!canReturn) {
		returnCounter--;
		if (returnCounter <= 0) {
			canReturn = true;
		}
	}


	//サイズからスケールへ変換
	scalef = ConvertSizeToScale(size);
	scale = scalef;
	broadSphereCollider->SetRadius(scalef * 120.0f);
	toMapChipCollider->SetRadius( scalef * 120.0f, scalef * 120.0f);
	//移動量を適応
	Move();
	DebugText::GetInstance()->Print("WASD stick : Move",600,0,3);
	DebugText::GetInstance()->Print("Qkey Bbutton: ReturnDebri",600,40,3);
	DebugText::GetInstance()->Print("SPACEkey Abutton : Boom",600,80,3);
	DebugText::GetInstance()->VariablePrint(0, 0, "playerSize", size, 3);
	DebugText::GetInstance()->VariablePrint(0, 40, "DebrisCount", Debris::debris.size(), 3);
	DebugText::GetInstance()->VariablePrint(0, 80, "StayTimer", returnCounter, 3);
	
	//マップチップとの当たり判定
	toMapChipCollider->Update();
	Vector3 hitPos = {0,0,0};
	//上下左右
	if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &velocity, &hitPos)) {
		Vector3 normal = {0,0,0};
		
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
	//角
	else if (MapChip::GetInstance()->CheckMapChipToSphere2d(broadSphereCollider, &velocity, &hitPos)) {
		Vector3 normal = { 0,0,0 };
		if (hitPos.x != 0) {
			int vec = 1;	//向き
			if (0 < velocity.x) {
				vec = -1;
			}
			pos.x = hitPos.x;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//向き
			if (velocity.z < 0) {
				vec = -1;
			}
			pos.z = hitPos.z;
			normal.z = vec;
		}
		normal.Normalize();
		velocity = CalcWallScratchVector(velocity, normal);
	}

	//flont.get()->SetPosition(Vector3(pos + velocity.Normal() * 500));
}

void PlayerObject::Draw() const
{
	Object3d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	coreUp->pos = pos;
	coreUp->Adaptation();
	coreUp->Draw();

	Object3d::PostDraw();

	GameObjCommon::Draw();


	//Object3d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	//flont.get()->Draw();
	//Object3d::PostDraw();
}



void PlayerObject::OnCollision(const CollisionInfo &info)
{
	Debris *debri;
	switch (info.object->Tag)
	{
	case DEBRIS:
		//破片が吸収範囲と衝突したら
		debri = dynamic_cast<Debris *>(info.object);
		if (info.myName == "hitCollider" &&
			info.collider->GetCollisionName() == "hitCollider" &&
			(!debri->isFirstAttack || debri->state == Debris::RETURN)) {
			//吸収
			size += debri->GetSize();
		}
		break;
	case ENEMY:
		//位置修正
		Damage(1.0f);
		DebugText::GetInstance()->Print("HitEnemy", 0, 80, 3);
		break;


		break;

	default:
		break;
	}
}

void PlayerObject::HitWall(
	const XMVECTOR &hitPos,		//衝突位置
	const Vector3 &normal)
{
	velocity = CalcReflectVector(velocity, normal);
}

void PlayerObject::Damage(float damage)
{
	//無敵だったらダメージを受けない
	if (isInvincible) return;
	isInvincible = true;
	size -= damage;
	invincibleCounter = 60;


}


