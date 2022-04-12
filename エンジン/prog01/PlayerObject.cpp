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
#include "Enemy.h"


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

	//ブロード
	broadSphereCollider = new SphereCollider("hitCollider", { 0,scalef * 180.0f,0 }, scalef * 180.0f);
	SetBroadCollider(broadSphereCollider);
	//押し返し用
	pushBackCollider = new SphereCollider("hitCollider", { 0,scalef * 180.0f,0 }, scalef * 180.0f);
	SetNarrowCollider(pushBackCollider);
	//攻撃用
	attackCollider = new SphereCollider("hitCollider", { 0,scalef * 180.0f,0 }, scalef * 180.0f + 50.0f);
	SetNarrowCollider(pushBackCollider);

	//マップチップ用
	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);

	coreUp = new GameObjCommon(ModelManager::SLIME_CORE, Notag, false,{ 0,0,0 }, {1.5f,1.5f,1.5f });

	
}


void PlayerObject::Initialize()
{
	//サイズ初期化
	size = 500.0f;
	//サイズ初期化
	scalef = ConvertSizeToScale(size);
	objectData->SetScale(scalef);
	toMapChipCollider->SetRadius(scalef * 180.0f, scalef * 180.0f);
	//吸引範囲
	suction = scalef * suctionRatio;
	//ポジション初期化
	pos = startPos;

	//攻撃関係
	attack = {
		true,
		false,
		30,
		0
	};
	//回収関係
	collect = {
		true,
		false,
		300,
		0
	};


	objectData->SetAlpha(0.5f);

	//アニメーション開始
	objectData->PlayAnimation();


}

void PlayerObject::Update()
{
	Input *input = Input::GetInstance();
	//スケールから移動量決定
	moveSpead = scalef * 4;
	//ペナルティリセット
	penalty = { 0,0,0 };

	//移動量減衰処理
	VelocityReset(0.9f);
	if (velocity.Length() >= 200) {
		velocity = velocity.Normal() * 200;
	}
	if (attack.is && velocity.Length() < 60) {
		attack.is = false;
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


	//デバッグ用サイズ変更
	if (input->PushKey(DIK_UP)) {
		size += 10.0f;
	}
	if (input->PushKey(DIK_DOWN)) {
		size -= 10.0f;
	}

	//自爆
	if ((input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(BUTTON_A))&&
		attack.can)
	{
		//攻撃開始
		attack.Start();

		//ショットに使う総合サイズ
		float maxSize = size * 0.5f;

		//破片生成
		for (int i = 0; i < destructPow; i++) {
			Vector3 startVec;		//速度*向きベクトル
			float shotRad;			//角度決定用
			//発射スピード
			float shotSpeed = rand() % 20 + scalef * 40;
			//残骸のサイズ
			float shotSize = maxSize / destructPow;

			//-15~15度で計算
			shotRad = XMConvertToRadians(rand() % 120 - 60);

			startVec = -velocity.Normal();

			startVec.AddRotationY(shotRad);
			//startVec = startVec + offset;

			velocity += velocity.Normal() * 50;
			//Debrisのコンテナに追加
			Debris::debris.push_back(new Debris(pos, startVec * shotSpeed, shotSize));
		}
		//爆発終了
		size -= maxSize;
		
	}

	//回収
	if (input->TriggerKey(DIK_Q)|| input->TriggerPadButton(BUTTON_B)) {
		if (collect.Start()) {
			for (int i = 0; i < Debris::debris.size(); i++) {
				Debris::debris[i]->ReturnStart();
			}
		}
	}
	//攻撃インターバル
	attack.Intervel();

	//回収インターバル
	collect.Intervel();


	//サイズからスケールへ変換
	scalef = ConvertSizeToScale(size);
	scale = scalef;
	pushBackCollider->SetRadius(scalef * 120.0f);
	toMapChipCollider->SetRadius( scalef * 120.0f, scalef * 120.0f);
	//移動量を適応
	Move();
	//移動量からブロードコライダーを更新
	broadSphereCollider->SetRadius(/*velocity.Length() + pushBackCollider->GetRadius()*/scalef * 120.0f);

	DebugText::GetInstance()->Print("WASD stick : Move",600,0,3);
	DebugText::GetInstance()->Print("Qkey Bbutton: ReturnDebri",600,40,3);
	DebugText::GetInstance()->Print("SPACEkey Abutton : Boom",600,80,3);
	DebugText::GetInstance()->VariablePrint(0, 0, "playerSize", size, 3);
	DebugText::GetInstance()->VariablePrint(0, 40, "DebrisCount", Debris::debris.size(), 3);
	DebugText::GetInstance()->VariablePrint(0, 80, "StayTimer", collect.timer, 3);
	DebugText::GetInstance()->VariablePrint(0, 160, "Speed", velocity.Length(), 3);
	
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

void PlayerObject::FinalUpdate()
{
	//マップチップとの当たり判定
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	//上下左右
	if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos)) {
		Vector3 normal = { 0,0,0 };

		if (hitPos.x != 0) {
			int vec = 1;	//向き
			if (0 < moveVec.x) {
				vec = -1;
			}
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * vec;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//向き
			if (moveVec.z < 0) {
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

}



void PlayerObject::OnCollision(const CollisionInfo &info)
{
	Debris *debri;
	Enemy *enemy;
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
		enemy = dynamic_cast<Enemy*>(info.object);
		//エネミーが攻撃可能状態ならダメージ処理をする
		if (enemy->attack.can) {
			Damage(enemy->Attack());
		}

		//位置修正
		DebugText::GetInstance()->Print("HitEnemy", 0, 80, 3);
		
		//攻撃中でなければ押し返し処理
		if (!attack.is) {
			penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.2f;
		}

		break;
	default:
		break;
	}

	penalty.y = 0;
	pos += penalty;
	GameObjCommon::Update();
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
	//isInvincible = true;
	size -= damage;
	invincibleCounter = 60;

	//sizeが0になったら死亡処理
	if (size <= 0) {
		size = 0;
		
	}

}


