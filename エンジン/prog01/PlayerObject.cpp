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
		ModelManager::PLAYER,	//�N���Q���f�����Z�b�g
		GameObjCommon::PLAYER,	//�v���C���[�Ƃ��Ĉ���
		false,					//�d�͂̉e�����󂯂�
		startPos,
		{ 1,1,1 },
		{ 1,1,1 },
		true
	)
{
	srand(time(NULL));			//���j�p�����̃V�[�h�l���Z�b�g

	//scale = ConvertSizeToScale(energy / 2.0f);

	//�����ʒu�ɐݒ�
	this->startPos = startPos;

	isBounce = false;

	//�u���[�h
	broadSphereCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f - 200,0 }, scale.x * 180.0f);
	SetBroadCollider(broadSphereCollider);
	//�����Ԃ��p
	pushBackCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f + -200,0 }, scale.x * 180.0f);
	SetNarrowCollider(pushBackCollider);
	//�U���p
	attackCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f - 200,0 }, scale.x * 180.0f + 50.0f);
	SetNarrowCollider(pushBackCollider);

	//�}�b�v�`�b�v�p
	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);


}

PlayerObject::~PlayerObject()
{
}


void PlayerObject::Initialize()
{
	//�T�C�Y������
	maxHp = 1.0f;
	hp = maxHp;
	//�T�C�Y������
	toMapChipCollider->SetRadius(scale.x * 180.0f, scale.z * 180.0f);
	//�|�W�V����������
	pos = startPos;
	oldPos = pos;
	savePos = pos;
	//�U���֌W
	attack = {
		true,
		false,
		30,
		0
	};
	//�U���p
	attackCount = 3;
	//����֌W
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

	//�A�j���[�V�����J�n
	animationType = MOVE;
	animationChangeFrag = true;

	// �p�[�e�B�N���}�l�[�W������
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
	//���|�W�V����
	oldPos = pos;
	//�X�P�[������ړ��ʌ���
	moveSpead = 5.5f;
	//�y�i���e�B���Z�b�g
	penalty = { 0,0,0 };

	//�ړ��ʌ�������
	VelocityReset(0.96f);
	if (!attack.is && velocity.Length() >= 60) {
		velocity = velocity.Normal() * 60;
	}
	if (attack.is && velocity.Length() >= 150) {
		velocity = velocity.Normal() * 150;
	}
	//�q�b�g�X�g�b�v�����Ƃ���velosity120�����ɂȂ�����
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

	//���G����
	if (isInvincible) {
		invincibleCounter--;
		if (invincibleCounter <= 0) {
			isInvincible = false;
		}
	}

	if (!endFlag && MoveCheck::GetInstance()->IsMoveFlag())
	{
		//�R���g���[���[�ł̈ړ�(before
		/*velocity.x += input->PadStickGradient().x * moveSpead;
		velocity.z += -input->PadStickGradient().y * moveSpead;*/

		//�R���g���[���[�ł̈ړ�(�f�o�b�O�p��ԂȂ�
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

		//����(before
		/*if ((input->TriggerPadButton(BUTTON_A)) &&
			attack.can &&
			attackCount > 0)*/
			//����(�f�o�b�O�p��ԂȂ�
		if ((input->TriggerPadButton(BUTTON_A)) || input->TriggerKey(DIK_SPACE)) {
			//attack.can &&
			if (attackCount > 0 &&
				velocity.Length() != 0.0f) {
				Audio::GetInstance()->LoopStopWave(1);

				volume = 3.0f;

				Audio::GetInstance()->LoopPlayWave(10, 3);
				boostFlag = true;
				//�U���J�n
				attack.Start();

				animationType = BOOST;
				animationChangeFrag = true;

				////�j�А���
				//for (int i = 0; i < DESTRUCT_POW; i++) {
				//	Vector3 startVec;		//���x*�����x�N�g��
				//	float shotRad;			//�p�x����p
				//	//���˃X�s�[�h
				//	float shotSpeed = rand() % 20 + energy / 500 * 30;
				//	//�c�[�̃T�C�Y
				//	float shotSize = SHOT_ENERGY / DESTRUCT_POW;

				//	//-15~15�x�Ōv�Z
				//	shotRad = XMConvertToRadians(rand() % 90 - 45);

				//	startVec = -velocity.Normal();

				//	startVec.AddRotationY(shotRad);
				//	//startVec = startVec + offset;

				//	velocity += velocity.Normal() * 60;
				//	//Debris�̃R���e�i�ɒǉ�
				//	Debris::debris.push_back(new Debris(pos, startVec * shotSpeed, shotSize));
				//}
				//energy -= SHOT_ENERGY;

				velocity += velocity.Normal() * 200;
				//�����I��
				attackCount--;
				attackGage.Start();
			}
			else {
				dontBoost = true;
			}
		}

		//���
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

	//�U���C���^�[�o��
	attack.Intervel(true);

	//����C���^�[�o��
	recovery.Intervel();

	//�U���͍X�V
	attackPow = velocity.Length();

	//�T�C�Y����X�P�[���֕ϊ�
	//scale = ConvertSizeToScale(energy / 2.0f);
	//�ړ��ʂ�K��

	for (int i = 0; i < Debris::debris.size(); i++) {
		//if (Debris::debris[i]->isHitStop && !isHitStopCoolDown && !isFirstHitStop) {
		//�オ���񂾂��q�b�g�X�g�b�v�����ꍇ�A����3��܂Ńq�b�g�X�g�b�v�����ꍇ
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
	//�ړ��ʂ���u���[�h�R���C�_�[���X�V
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
	//�q�b�g�X�g�b�v
	if (isHitStop) {
		hitStopTimer++;
		if (hitStopTimer >= 7) {
			hitStopTimer = 0;
			velocity = saveVelocity;
			isHitStop = false;
		}
	}
	//�q�b�g�X�g�b�v(�N�[���_�E��
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
	//�}�b�v�`�b�v�Ƃ̓����蔻��
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	Vector3 normal = { 0,0,0 };

	//�}�b�v�`�b�v���o
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
	//�͈͊O���o
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

	//�p
	else if (MapChip::GetInstance()->CheckMapChipToSphere2d(broadSphereCollider, &velocity, &hitPos)) {
		Vector3 normal = { 0,0,0 };
		if (hitPos.x != 0) {
			int vec = 1;	//����
			if (0 < velocity.x) {
				vec = -1;
			}
			pos.x = hitPos.x;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//����
			if (velocity.z < 0) {
				vec = -1;
			}
			pos.z = hitPos.z;
			normal.z = vec;
		}
		normal.Normalize();
		velocity = CalcWallScratchVector(velocity, normal);
	}


	//�ړ����c�[����
	if (attack.is && debrisCooldown.can) {
		debrisCooldown.Start();

		//�c�[�̃T�C�Y
		float shotSize = hp / SHOT_ENERGY;
		Vector3 shotVec = -velocity.Normal();
		//startVec = startVec + offset;

		//Debris�̃R���e�i�ɒǉ�
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

	//�h��
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
		//�j�Ђ��z���͈͂ƏՓ˂�����
		debri = dynamic_cast<Debris*>(info.object);
		if (!debri->isFirstAttack)
		{
			if (info.myName == "hitCollider" &&
				info.collider->GetCollisionName() == "hitCollider" &&
				(!debri->isFirstAttack || debri->state == Debris::RETURN)) {
				//�z��
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
		//�G�l�~�[���U���\��ԂŁA�������U�����Ă��Ȃ���΃_���[�W�𕉂�
		if (enemy->attack.can && !attack.is) {
			Damage(enemy->CauseDamage());
		}
		//�ʒu�C��


	/*if (velocity.Length() >= 40 && oldVec.VDot(velocity) < 0.1f) {
	}*/
		if (attack.is) {
			//�q�b�g�X�g�b�v
			if (!isHitStopCoolDown) {
				saveVelocity = velocity;
				isFirstHitStop = true;
				isHitStop = true;
				isHitStopCoolDown = true;
				Audio::GetInstance()->PlayWave(18, 0.5f);
			}
		}

		//�U�����łȂ���Ή����Ԃ�����
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
	const XMVECTOR& hitPos,		//�Փˈʒu
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
	//���G��������_���[�W���󂯂Ȃ�
	if (isInvincible) return;
	//isInvincible = true;
	hp -= damage;
	invincibleCounter = 60;
	Audio::GetInstance()->PlayWave(11, 0.4f);

	//size��0�ɂȂ����玀�S����
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