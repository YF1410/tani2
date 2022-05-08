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
#include "ParticleManager.h"


using namespace DirectX;

PlayerObject::PlayerObject(XMFLOAT3 startPos) :
	GameObjCommon(
		ModelManager::SLIME,	//スライムのモデルをセット
		GameObjCommon::PLAYER,	//プレイヤーとして扱う
		false,					//重力の影響を受ける
		startPos,
		{ 1,1,1 },
		{ 0,0,0 },
		true
	)
{
	srand(time(NULL));			//爆破用乱数のシード値をセット

	scale = ConvertSizeToScale(energy / 2.0f);

	//初期位置に設定
	this->startPos = startPos;

	isBounce = false;

	//ブロード
	broadSphereCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f,0 }, scale.x * 180.0f);
	SetBroadCollider(broadSphereCollider);
	//押し返し用
	pushBackCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f,0 }, scale.x * 180.0f);
	SetNarrowCollider(pushBackCollider);
	//攻撃用
	attackCount = 2;
	attackCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f,0 }, scale.x * 180.0f + 50.0f);
	SetNarrowCollider(pushBackCollider);

	//マップチップ用
	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);

	coreUp = new GameObjCommon(ModelManager::SLIME_CORE, Notag, false, { 0,0,0 }, { 1.5f,1.5f,1.5f });

}

PlayerObject::~PlayerObject()
{
	delete coreUp;
}


void PlayerObject::Initialize()
{
	//サイズ初期化
	energy = 1508.0f;
	//サイズ初期化
	toMapChipCollider->SetRadius(scale.x * 180.0f, scale.z * 180.0f);
	//ポジション初期化
	pos = startPos;

	//攻撃関係
	attack = {
		true,
		false,
		30,
		0
	};
	attackCount = 3;
	//回収関係
	recovery = {
		true,
		false,
		300,
		0
	};
	//
	attackGage = {
		true,
		false,
		60 * 2,
		0
	};
	debrisCooldown = {
		true,
		false,
		5,
		0
	};

	//アニメーション開始
	objectData->PlayAnimation();

	// パーティクルマネージャ生成
	healParticle1 = healParticle1->Create(L"heal3");
	healParticle2 = healParticle2->Create(L"heal4");
	healParticle1->SetStartScale(300.0f);
	healParticle2->SetStartScale(300.0f);
	healParticle1->SetCenter(500.0f);
	healParticle2->SetCenter(500.0f);

	boomParticle = boomParticle->Create(L"boom");
	boomParticle->SetStartScale(300.0f);
	boomParticle->SetCenter(500.0f);

	refParticle = refParticle->Create();
	refParticle->SetStartScale(300.0f);
	refParticle->SetCenter(400.0f);

	atkParticle = atkParticle->Create(L"attack");
	atkParticle->SetStartScale(300.0f);
	atkParticle->SetCenter(400.0f);

	atkParticle->SetStartColor({ 0.8f, 0.8f, 2.0f, 1.0f });



	ParticleManager::GetInstance()->SetParticleEmitter(healParticle1);
	ParticleManager::GetInstance()->SetParticleEmitter(healParticle2);
	ParticleManager::GetInstance()->SetParticleEmitter(boomParticle);
	ParticleManager::GetInstance()->SetParticleEmitter(refParticle);
	ParticleManager::GetInstance()->SetParticleEmitter(atkParticle);
}

void PlayerObject::Update()
{
	Input *input = Input::GetInstance();
	//スケールから移動量決定
	moveSpead = scale.x * 5;
	//ペナルティリセット
	penalty = { 0,0,0 };

	//移動量減衰処理
	VelocityReset(0.95f);
	if (!attack.is && velocity.Length() >= 60) {
		velocity = velocity.Normal() * 60;
	}
	if (attack.is && velocity.Length() >= 180) {
		velocity = velocity.Normal() * 180;
	}
	if (attack.is && velocity.Length() < 100) {
		attack.is = false;
		isBounce = false;
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

	//コントローラーでの移動
	if (input->PushKey(DIK_A)) {
		velocity.x -= moveSpead;
	}
	else if (input->PushKey(DIK_D)) {
		velocity.x += moveSpead;
	}
	else {
		velocity.x += input->PadStickGradient().x * moveSpead;
	}
	if (input->PushKey(DIK_W)) {
		velocity.z += moveSpead;
	}
	else if (input->PushKey(DIK_S)) {
		velocity.z -= moveSpead;
	}
	else {
		velocity.z += -input->PadStickGradient().y * moveSpead;
	}


	//自爆
	if (((input->TriggerPadButton(BUTTON_A)) ||input->TriggerKey(DIK_SPACE))&&
		attack.can &&
		attackCount > 0)
	{
		//攻撃開始
		attack.Start();


		////破片生成
		//for (int i = 0; i < DESTRUCT_POW; i++) {
		//	Vector3 startVec;		//速度*向きベクトル
		//	float shotRad;			//角度決定用
		//	//発射スピード
		//	float shotSpeed = rand() % 20 + energy / 500 * 30;
		//	//残骸のサイズ
		//	float shotSize = SHOT_ENERGY / DESTRUCT_POW;

		//	//-15~15度で計算
		//	shotRad = XMConvertToRadians(rand() % 90 - 45);

		//	startVec = -velocity.Normal();

		//	startVec.AddRotationY(shotRad);
		//	//startVec = startVec + offset;

		//	velocity += velocity.Normal() * 60;2
		//	//Debrisのコンテナに追加
		//	Debris::debris.push_back(new Debris(pos, startVec * shotSpeed, shotSize));
		//}
		//energy -= SHOT_ENERGY;

		velocity += velocity.Normal() * 600;
		//爆発終了
		attackCount--;
		attackGage.Start();
	}

	
	if (attackCount < 3) {
		if (attackGage.can) {
			attackCount++;
			attackGage.Start();
		}
		attackGage.Intervel();
	}


	//回収
	if (input->TriggerPadButton(BUTTON_B)) {
		if (recovery.Start()) {
			for (int i = 0; i < Debris::debris.size(); i++) {
				Debris::debris[i]->ReturnStart();
			}
		}
		else {
			if (!dontRecovery) {
				shakePos = pos;
			}
			dontRecovery = true;
		}
	}

	if(dontRecovery){
		timer++;
		if (timer >= maxTimer) {
			timer = 0;
			shake = { 0,0 };
		}

		shake = {
			(float)((rand() % ((timer - maxTimer) - (timer - maxTimer) / 2)) * 5),
			(float)((rand() % ((timer - maxTimer) - (timer - maxTimer) / 2)) * 5)
		};

		shakePos = { pos.x + shake.x ,shakePos.y,pos.z + shake.y };
	}

	//攻撃インターバル
	attack.Intervel();

	//回収インターバル
	recovery.Intervel();

	//攻撃力更新
	attackPow = velocity.Length();


	//サイズからスケールへ変換
	scale = ConvertSizeToScale(energy / 2.0f);
	//移動量を適応
	PosAddVelocity();
	//移動量からブロードコライダーを更新
	broadSphereCollider->SetRadius(/*velocity.Length() + pushBackCollider->GetRadius()*/scale.x * 120.0f);
	toMapChipCollider->SetRadius(scale.x * 120.0f, scale.x * 120.0f);

	//DebugText::GetInstance()->VariablePrint(0, 80, "StayTimer", recovery.timer, 3);

	//if (input->TriggerKey(DIK_H)) {
	//	frameF = true;
	//}

	//if (frameF) {
	//	frame++;
	//}

	//if (frame <= 2 && frameF) {
	//	healParticle1->AddHeal(3, 40, pos);
	//	healParticle2->AddHeal(4, 40, pos);
	//	//boomParticle->AddBoom(2, 40, pos);
	//	//refParticle->AddRef(2, 40, pos,velocity);
	//}
	//else {
	//	frameF = false;
	//	frame = 0;
	//}
	/*healParticle1->Update();
	healParticle2->Update();
	boomParticle->Update();
	refParticle->Update();
	atkParticle->Update();*/
}

void PlayerObject::Draw() const
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	Object3d::PreDraw(cmdList);

	coreUp->pos = pos;
	coreUp->Adaptation();
	coreUp->Draw();

	Object3d::PostDraw();

	GameObjCommon::Draw();

	// パーティクルの描画
	/*healParticle1->Draw(cmdList);
	healParticle2->Draw(cmdList);
	boomParticle->Draw(cmdList);
	refParticle->Draw(cmdList);
	atkParticle->Draw(cmdList);*/
	//Object3d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	//flont.get()->Draw();
	//Object3d::PostDraw();
}

void PlayerObject::LustUpdate()
{
	//マップチップとの当たり判定
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	Vector3 normal = { 0,0,0 };
	//上下
	if (MapChip::GetInstance()->CheckMapChipAreaToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal)) {
		if (hitPos.x != 0) {
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * normal.x;
		}
		if (hitPos.z != 0) {
			pos.z = hitPos.z + toMapChipCollider->GetRadiusY() * normal.z;
		}
		normal.Normalize();
		HitWall(hitPos, normal.Normal());

	}
	else if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal)) {

		if (hitPos.x != 0) {
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * normal.x;
		}
		if (hitPos.z != 0) {
			pos.z = hitPos.z + toMapChipCollider->GetRadiusY() * normal.z;
		}
		normal.Normalize();
		HitWall(hitPos, normal.Normal());
	}
	


	//角
	//else if (MapChip::GetInstance()->CheckMapChipToSphere2d(broadSphereCollider, &velocity, &hitPos)) {
		//Vector3 normal = { 0,0,0 };
		//if (hitPos.x != 0) {
		//	int vec = 1;	//向き
		//	if (0 < velocity.x) {
		//		vec = -1;
		//	}
		//	pos.x = hitPos.x;
		//	normal.x = vec;
		//}
		//if (hitPos.z != 0) {
		//	int vec = 1;	//向き
		//	if (velocity.z < 0) {
		//		vec = -1;
		//	}
		//	pos.z = hitPos.z;
		//	normal.z = vec;
		//}
		//normal.Normalize();
		//velocity = CalcWallScratchVector(velocity, normal);
	//}


	//移動中残骸生成
	if (attack.is && debrisCooldown.can) {
		debrisCooldown.Start();

		//残骸のサイズ
		float shotSize = energy / SHOT_ENERGY;
		Vector3 shotVec = -velocity.Normal();
		//startVec = startVec + offset;

		//Debrisのコンテナに追加
		Debris::debris.push_back(new Debris(pos/* + offsetS + offsetF * scale.x*/, shotVec * 20.0f, shotSize));
		energy -= shotSize;

	}
	debrisCooldown.Intervel();

	Input* input = Input::GetInstance();
	Vector3 beforePos = pos + velocity;
	if (attack.is) {
		atkParticle->AddAttack(3, 20, pos, velocity, (atan2(pos.z - beforePos.z, pos.x - beforePos.x) + 3.14/2));
		input->GetInstance()->SetVibration(true);
	}
	else if (!attack.is) {
		input->GetInstance()->SetVibration(false);
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
		if (!debri->isFirstAttack)
		{
			if (info.myName == "hitCollider" &&
				info.collider->GetCollisionName() == "hitCollider" &&
				(!debri->isFirstAttack || debri->state == Debris::RETURN)) {
				//吸収
				energy += debri->GetSize();
			}

			isHealFrameIncrease = true;

			if (isHealFrameIncrease) {
				nowHealFrame++;
			}

			if (nowHealFrame <= maxHealFrame && isHealFrameIncrease) {
				healParticle1->AddHeal(3, 40, pos, velocity);
				healParticle2->AddHeal(4, 40, pos, velocity);
				//boomParticle->AddBoom(2, 40, pos);
				//refParticle->AddRef(2, 40, pos,velocity);
			}
			else {
				isHealFrameIncrease = false;
				nowHealFrame = 0;
			}
		}
		

		break;
	case ENEMY:
		enemy = dynamic_cast<Enemy*>(info.object);
		//エネミーが攻撃可能状態で、自分が攻撃していなければダメージを負う
		if (enemy->attack.can && !attack.is) {
			Damage(enemy->CauseDamage());
		}

		//位置修正

		//攻撃中でなければ押し返し処理
		if (!attack.is) {
			penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.02f;
		}
		else {
			Vector3 nextPos = info.inter + Vector3(pos - info.object->pos).Normal() * (broadSphereCollider->GetRadius() /*+ enemy->pushBackCollider->GetRadius()*/);
			nextPos.y = 0;
			velocity = CalcReflectVector(velocity, Vector3(pos - enemy->pos).Normal());
			pos = nextPos;
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
	if (!isBounce && attack.is) {
		velocity *= 3.0f;
		isBounce = true;
		refParticle->AddRef(20, 40, pos, velocity);
	}
}

void PlayerObject::Damage(float damage)
{
	//無敵だったらダメージを受けない
	if (isInvincible) return;
	//isInvincible = true;
	energy -= damage;
	invincibleCounter = 60;

	//sizeが0になったら死亡処理
	if (energy <= 0) {
		energy = 0;
	}

	boomParticle->AddBoom(2, 10, pos);
}


