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
#include "Audio.h"
#include "MoveCheck.h"

using namespace DirectX;

PlayerObject::PlayerObject(XMFLOAT3 startPos) :
	GameObjCommon(
		ModelManager::PLAYER,	//クラゲモデルをセット
		GameObjCommon::PLAYER,	//プレイヤーとして扱う
		false,					//重力の影響を受ける
		startPos,
		{ 1,1,1 },
		{ 1,1,1 },
		true
	)
{
	srand(time(NULL));			//爆破用乱数のシード値をセット

	//scale = ConvertSizeToScale(energy / 2.0f);

	//初期位置に設定
	this->startPos = startPos;

	isBounce = false;

	//ブロード
	broadSphereCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f - 200,0 }, scale.x * 180.0f);
	SetBroadCollider(broadSphereCollider);
	//押し返し用
	pushBackCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f + -200,0 }, scale.x * 180.0f);
	SetNarrowCollider(pushBackCollider);
	//攻撃用
	attackCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f - 200,0 }, scale.x * 180.0f + 50.0f);
	SetNarrowCollider(pushBackCollider);

	//マップチップ用
	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);


}

PlayerObject::~PlayerObject()
{
}


void PlayerObject::Initialize()
{
	//サイズ初期化
	maxHp = 1.0f;
	hp = maxHp;
	//サイズ初期化
	toMapChipCollider->SetRadius(scale.x * 180.0f, scale.z * 180.0f);
	//ポジション初期化
	pos = startPos;
	oldPos = pos;
	savePos = pos;
	//攻撃関係
	attack = {
		true,
		false,
		30,
		0
	};
	//攻撃用
	attackCount = 3;
	//回収関係
	recovery = {
		true,
		false,
		150,
		0
	};
	//
	attackGage = {
		true,
		false,
		90,
		0
	};
	debrisCooldown = {
		true,
		false,
		5,
		0
	};

	//アニメーション開始
	animationType = MOVE;
	animationChangeFrag = true;

	// パーティクルマネージャ生成
	healParticle1 = healParticle1->Create(L"heal1");
	healParticle2 = healParticle2->Create(L"heal2");
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

	//atkParticle->SetStartColor({ 0.8f, 0.8f, 2.0f, 1.0f });

	ParticleManager::GetInstance()->SetParticleEmitter(healParticle1);
	ParticleManager::GetInstance()->SetParticleEmitter(healParticle2);
	ParticleManager::GetInstance()->SetParticleEmitter(boomParticle);
	ParticleManager::GetInstance()->SetParticleEmitter(refParticle);
	ParticleManager::GetInstance()->SetParticleEmitter(atkParticle);
}

void PlayerObject::Update()
{
	pos = savePos;
	pos.y = 200;
	velocity.y = 0;
	Input* input = Input::GetInstance();
	healChack = false;
	//旧ポジション
	oldPos = pos;
	//スケールから移動量決定
	moveSpead = 5.5f;
	//ペナルティリセット
	penalty = { 0,0,0 };

	//移動量減衰処理
	VelocityReset(0.96f);
	if (!attack.is && velocity.Length() >= 60) {
		velocity = velocity.Normal() * 60;
	}
	if (attack.is && velocity.Length() >= 150) {
		velocity = velocity.Normal() * 150;
	}
	//ヒットストップ無いときにvelosity120未満になったら
	if (attack.is && velocity.Length() < 120 && !isHitStop) {
		attack.is = false;
		isBounce = false;
		animationType = MOVE;
		animationChangeFrag = true;
	}
	if (recoveryEndTimer == 0) {
		hitStopCount = 0;
		isFirstHitStop = false;
		animationType = MOVE;
		animationChangeFrag = true;
	}

	//無敵処理
	if (isInvincible) {
		invincibleCounter--;
		if (invincibleCounter <= 0) {
			isInvincible = false;
		}
	}

	if (!endFlag && MoveCheck::GetInstance()->IsMoveFlag())
	{
		//コントローラーでの移動(before
		/*velocity.x += input->PadStickGradient().x * moveSpead;
		velocity.z += -input->PadStickGradient().y * moveSpead;*/

		//コントローラーでの移動(デバッグ用やぶなか
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

		if (velocity.Length() > 0.0f)
		{
			saveAngleFlag = true;
		}
		else
		{
			saveAngleFlag = false;
		}

		if (!saveAngleFlag)
		{
			rotate.y = saveAngle;
		}
		else
		{
			saveAngle = ConvertNormalToDeg(velocity.Normal(), Vector3{ 0,0,1 }).y + 180;
			rotate.y = saveAngle;
		}
		//rotate.x = 90;

		//自爆(before
		/*if ((input->TriggerPadButton(BUTTON_A)) &&
			attack.can &&
			attackCount > 0)*/
			//自爆(デバッグ用やぶなか
		if ((input->TriggerPadButton(BUTTON_A)) || input->TriggerKey(DIK_SPACE)) {
			//attack.can &&
			if (attackCount > 0 &&
				velocity.Length() != 0.0f) {
				Audio::GetInstance()->LoopStopWave(1);

				volume = 3.0f;

				Audio::GetInstance()->LoopPlayWave(10, 3);
				boostFlag = true;
				//攻撃開始
				attack.Start();

				animationType = BOOST;
				animationChangeFrag = true;

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

				//	velocity += velocity.Normal() * 60;
				//	//Debrisのコンテナに追加
				//	Debris::debris.push_back(new Debris(pos, startVec * shotSpeed, shotSize));
				//}
				//energy -= SHOT_ENERGY;

				velocity += velocity.Normal() * 200;
				//爆発終了
				attackCount--;
				attackGage.Start();
			}
			else {
				dontBoost = true;
			}
		}

		//回収
		if (input->TriggerPadButton(BUTTON_B) ||
			input->TriggerKey(DIK_Q)) {
			if (Debris::debris.size() != 0 && recovery.Start()) {
				Audio::GetInstance()->PlayWave(14, 0.5f);
				for (int i = 0; i < Debris::debris.size(); i++) {
					Debris::debris[i]->ReturnStart();

					animationType = RETRIEVE;
					animationChangeFrag = true;
					recoveryEndTimer = 30;
				}
			}
			else {
				dontRecovery = true;
			}
		}
		else if (recoveryEndTimer >= 0) {
			recoveryEndTimer--;
		}
	}
	else if (endFlag)
	{
		if (!resetFlag)
		{
			Audio::GetInstance()->LoopStopWave(0);
			input->GetInstance()->SetVibration(false);
			resetFlag = true;
		}
	}

	if (attackCount < 3) {
		if (attackGage.can) {
			attackCount++;
			attackGage.Start();
		}
		attackGage.Intervel();
	}

	if (!attack.is && boostFlag)
	{
		volume -= 0.1f;
		Audio::GetInstance()->LoopPlayWaveVolume(1, volume);
		if (volume <= 0.00f)
		{
			Audio::GetInstance()->LoopStopWave(1);
			boostFlag = false;
		}
	}

	//攻撃インターバル
	attack.Intervel(true);

	//回収インターバル
	recovery.Intervel();

	//攻撃力更新
	attackPow = velocity.Length();

	//サイズからスケールへ変換
	//scale = ConvertSizeToScale(energy / 2.0f);
	//移動量を適応

	for (int i = 0; i < Debris::debris.size(); i++) {
		//if (Debris::debris[i]->isHitStop && !isHitStopCoolDown && !isFirstHitStop) {
		//上が初回だけヒットストップ入れる場合、下が3回までヒットストップ入れる場合
		if (Debris::debris[i]->isHitStop && !isHitStopCoolDown && hitStopCount < 3) {
			Audio::GetInstance()->PlayWave(18, 0.5f);
			hitStopCount++;
			saveVelocity = velocity;
			isHitStop = true;
			isHitStopCoolDown = true;
			isFirstHitStop = true;
		}
	}
	if (isHitStop) {
		velocity = 0;
	}
	else if (!isHitStop) {
		PosAddVelocity();
	}
	//移動量からブロードコライダーを更新
	//toMapChipCollider->SetRadius(scale.x * 120.0f, scale.x * 120.0f);


	if (animationChangeFrag) {
		if (animationType == DEATH) {
			objectData->PlayAnimation(animationType, false);
		}
		else {
			objectData->PlayAnimation(animationType);
		}
		animationChangeFrag = false;
	}
	//ヒットストップ
	if (isHitStop) {
		hitStopTimer++;
		if (hitStopTimer >= 7) {
			hitStopTimer = 0;
			velocity = saveVelocity;
			isHitStop = false;
		}
	}
	//ヒットストップ(クールダウン
	if (isHitStopCoolDown) {
		hitStopCoolDown--;
		if (hitStopCoolDown <= 0) {
			hitStopCoolDown = 9;
			isHitStopCoolDown = false;
		}
	}
}

void PlayerObject::Draw() const
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	Object3d::PreDraw(cmdList);

	Object3d::PostDraw();

	GameObjCommon::Draw();

}

void PlayerObject::LustUpdate()
{
	//マップチップとの当たり判定
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	Vector3 normal = { 0,0,0 };

	//マップチップ検出
	if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal, &oldPos)) {

		if (hitPos.x != 0) {
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * normal.x;
		}
		if (hitPos.z != 0) {
			pos.z = hitPos.z + toMapChipCollider->GetRadiusY() * normal.z;
		}
		normal.Normalize();
		HitWall(hitPos, normal.Normal());
	}
	//範囲外検出
	else if (MapChip::GetInstance()->CheckMapChipAreaToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal)) {
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


	//移動中残骸生成
	if (attack.is && debrisCooldown.can) {
		debrisCooldown.Start();

		//残骸のサイズ
		float shotSize = hp / SHOT_ENERGY;
		Vector3 shotVec = -velocity.Normal();
		//startVec = startVec + offset;

		//Debrisのコンテナに追加
		Debris::debris.push_back(new Debris(pos - Vector3(0, 200, 0)/* + offsetS + offsetF * scale.x*/, shotVec * 20.0f, shotSize));
		hp -= shotSize;

	}
	debrisCooldown.Intervel();

	Input* input = Input::GetInstance();
	Vector3 beforePos = pos + velocity;
	if (attack.is && !endFlag) {
		atkParticle->AddAttack(3, 20, pos, velocity, (atan2(pos.z - beforePos.z, pos.x - beforePos.x) + 3.14 / 2));
		input->GetInstance()->SetVibration(true);
		input->GetInstance()->SetVibrationPower(10000);
	}
	else if (!attack.is && !endFlag) {
		input->GetInstance()->SetVibration(false);
	}

	//揺れ
	savePos = pos;
	if (dontBoost || dontRecovery) {
		Vector3 shake = { 0,0,0 };
		
		shake.x = (rand() % (shakeCount) - (shakeCount / 2));//(rand() % (int)(Ease(In,Quad,(float)(shakeTimer /20),100,1)));
		//shake.y = (rand() % (shakeCount - attenuation) - (shakeCount / 2));
		shake.z = (rand() % (shakeCount) - (shakeCount / 2));//(rand() % (int)Ease(In, Quad, (float)(shakeTimer / 20), 100, 1));
		pos += shake;
	}
	velocity.y = 0;

}


void PlayerObject::OnCollision(const CollisionInfo& info)
{
	Debris* debri;
	Enemy* enemy;
	switch (info.object->Tag)
	{
	case DEBRIS:
		//破片が吸収範囲と衝突したら
		debri = dynamic_cast<Debris*>(info.object);
		if (!debri->isFirstAttack)
		{
			if (info.myName == "hitCollider" &&
				info.collider->GetCollisionName() == "hitCollider" &&
				(!debri->isFirstAttack || debri->state == Debris::RETURN)) {
				//吸収
				hp += debri->GetSize();
				if (hp > maxHp) {
					hp = maxHp;
				}
				Audio::GetInstance()->PlayWave(13, 0.4f);
			}
			healChack = true;
			healParticle1->AddHeal(3, 40, pos, velocity);
			healParticle2->AddHeal(4, 40, pos, velocity);
		}


		break;
	case ENEMY:
		enemy = dynamic_cast<Enemy*>(info.object);
		//エネミーが攻撃可能状態で、自分が攻撃していなければダメージを負う
		if (enemy->attack.can && !attack.is) {
			Damage(enemy->CauseDamage());
		}
		//位置修正


	/*if (velocity.Length() >= 40 && oldVec.VDot(velocity) < 0.1f) {
	}*/
		if (attack.is) {
			//ヒットストップ
			if (!isHitStopCoolDown) {
				saveVelocity = velocity;
				isFirstHitStop = true;
				isHitStop = true;
				isHitStopCoolDown = true;
				Audio::GetInstance()->PlayWave(18, 0.5f);
			}
		}

		//攻撃中でなければ押し返し処理
		if (!attack.is) {
			penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.02f;
		}
		else if (enemy->isBounce) {
			Vector3 nextPos = info.inter + Vector3(pos - info.object->pos).Normal() * (broadSphereCollider->GetRadius() /*+ enemy->pushBackCollider->GetRadius()*/);
			nextPos.y = 0;
			velocity = CalcReflectVector(velocity, Vector3(pos - enemy->pos).Normal());
			pos = nextPos;
		}

		break;
	case BULLET:
		if (!attack.is) {
			Damage(5.0f);
		}
		break;
	default:
		break;
	}

	penalty.y = 0;
	pos += penalty;
	GameObjCommon::Update();
}

void PlayerObject::SetEndFlag(bool cFlag, bool gFlag)
{
	if (cFlag && !endFlag)
	{
		endFlag = cFlag;
	}
	else if (gFlag && !endFlag)
	{
		endFlag = gFlag;
	}
}

void PlayerObject::HitWall(
	const XMVECTOR& hitPos,		//衝突位置
	const Vector3& normal)
{
	Input* input = Input::GetInstance();
	Vector3 oldVec = velocity;
	velocity = CalcReflectVector(velocity, normal);
	/*if (velocity.Length() >= 40 && oldVec.VDot(velocity) < 0.1f) {
	}*/
	if (attack.is) {
		refParticle->AddRef(20, 40, pos, velocity);
		input->GetInstance()->SetVibrationPower(65535);
		Audio::GetInstance()->PlayWave(17, 0.4f);
		if (!isBounce) {
			isBounce = true;
		}
	}
}

void PlayerObject::StopState()
{
	velocity = { 0,0,0 };
	Audio::GetInstance()->LoopStopWave(1);
	Input::GetInstance()->SetVibration(false);
}

void PlayerObject::Damage(float damage)
{
	//無敵だったらダメージを受けない
	if (isInvincible) return;
	//isInvincible = true;
	hp -= damage;
	invincibleCounter = 60;
	Audio::GetInstance()->PlayWave(11, 0.4f);

	//sizeが0になったら死亡処理
	if (hp <= 0) {
		hp = 0;

		animationType = DEATH;
		animationChangeFrag = true;
		boomParticle->AddBoom(2, 10, pos, 5);
	}
	else {
		boomParticle->AddBoom(2, 10, pos);
	}
}