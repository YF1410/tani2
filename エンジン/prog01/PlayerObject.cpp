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
		ModelManager::SLIME,	//�X���C���̃��f�����Z�b�g
		GameObjCommon::PLAYER,	//�v���C���[�Ƃ��Ĉ���
		false,					//�d�͂̉e�����󂯂�
		startPos,
		{1,1,1},
		{0,0,0},
		true
	)
{
	srand(time(NULL));			//���j�p�����̃V�[�h�l���Z�b�g
	
	scale = ConvertSizeToScale(energy / 2.0f);

	//�����ʒu�ɐݒ�
	this->startPos = startPos;

	isBounce = false;

	//�u���[�h
	broadSphereCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f,0 }, scale.x * 180.0f);
	SetBroadCollider(broadSphereCollider);
	//�����Ԃ��p
	pushBackCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f,0 }, scale.x * 180.0f);
	SetNarrowCollider(pushBackCollider);
	//�U���p
	attackCount = 2;
	attackCollider = new SphereCollider("hitCollider", { 0,scale.x * 180.0f,0 }, scale.x * 180.0f + 50.0f);
	SetNarrowCollider(pushBackCollider);

	//�}�b�v�`�b�v�p
	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);

	coreUp = new GameObjCommon(ModelManager::SLIME_CORE, Notag, false,{ 0,0,0 }, {1.5f,1.5f,1.5f });
	
}

PlayerObject::~PlayerObject()
{
	delete coreUp;
}


void PlayerObject::Initialize()
{
	//�T�C�Y������
	energy = 1520.0f;
	//�T�C�Y������
	toMapChipCollider->SetRadius(scale.x * 180.0f, scale.z * 180.0f);
	//�|�W�V����������
	pos = startPos;

	//�U���֌W
	attack = {
		true,
		false,
		30,
		0
	};
	attackCount = 3;
	//����֌W
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


	//�A�j���[�V�����J�n
	objectData->PlayAnimation();

	// �p�[�e�B�N���}�l�[�W������
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

	atkParticle = atkParticle->Create();
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
	//�X�P�[������ړ��ʌ���
	moveSpead = scale.x * 5;
	//�y�i���e�B���Z�b�g
	penalty = { 0,0,0 };

	//�ړ��ʌ�������
	VelocityReset(0.95f);
	if (!attack.is && velocity.Length() >= 60) {
		velocity = velocity.Normal() * 60;
	}
	if (attack.is && velocity.Length() >= 300) {
		velocity = velocity.Normal() * 300;
	}
	if (attack.is && velocity.Length() < energy / 500 * 60) {
		attack.is = false;
		isBounce = false;
	}

	//���Z�b�g
	if (input->PushKey(DIK_R)) {
		Initialize();
	}

	//���G����
	if (isInvincible) {
		invincibleCounter--;
		if (invincibleCounter <= 0) {
			isInvincible = false;
		}
	}

	//�R���g���[���[�ł̈ړ�
	velocity.x += input->PadStickGradient().x * moveSpead;
	velocity.z += -input->PadStickGradient().y * moveSpead;

	//����
	if ((input->TriggerPadButton(BUTTON_A))&&
		attack.can&&
		attackCount > 0)
	{
		//�U���J�n
		attack.Start();

		//�j�А���
		for (int i = 0; i < DESTRUCT_POW; i++) {
			Vector3 startVec;		//���x*�����x�N�g��
			float shotRad;			//�p�x����p
			//���˃X�s�[�h
			float shotSpeed = rand() % 20 + energy / 500 * 30;
			//�c�[�̃T�C�Y
			float shotSize = SHOT_ENERGY / DESTRUCT_POW;

			//-15~15�x�Ōv�Z
			shotRad = XMConvertToRadians(rand() % 90 - 45);

			startVec = -velocity.Normal();

			startVec.AddRotationY(shotRad);
			//startVec = startVec + offset;

			velocity += velocity.Normal() * 60;
			//Debris�̃R���e�i�ɒǉ�
			Debris::debris.push_back(new Debris(pos, startVec * shotSpeed, shotSize));
		}
		//�����I��
		attackCount--;
		energy -= SHOT_ENERGY;
		attackGage.Start();
	}

	if (attackCount < 3) {
		if (attackGage.can) {
			attackCount++;
			attackGage.Start();
		}
		attackGage.Intervel();
	}


	//���
	if (input->TriggerPadButton(BUTTON_B)) {
		if (recovery.Start()) {
			for (int i = 0; i < Debris::debris.size(); i++) {
				Debris::debris[i]->ReturnStart();
			}
		}
		else {
			dontRecovery = true;
		}
	}



	//�U���C���^�[�o��
	attack.Intervel();

	//����C���^�[�o��
	recovery.Intervel();
	
	//�U���͍X�V
	attackPow = velocity.Length();


	//�T�C�Y����X�P�[���֕ϊ�
	scale = ConvertSizeToScale(energy / 2.0f);
	//�ړ��ʂ�K��
	PosAddVelocity();
	//�ړ��ʂ���u���[�h�R���C�_�[���X�V
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
	if (attack.is) {
		atkParticle->AddRef(5,20,pos,-velocity);
		input->GetInstance()->SetVibration(true);
	}
	if (!attack.is) {
		atkParticle->AddRef(5, 20, pos, -velocity);
		input->GetInstance()->SetVibration(false);
	}

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

	// �p�[�e�B�N���̕`��
	//healParticle1->Draw(cmdList);
	//healParticle2->Draw(cmdList);
	//boomParticle->Draw(cmdList);
	//refParticle->Draw(cmdList);
	//atkParticle->Draw(cmdList);
	//Object3d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	//flont.get()->Draw();
	//Object3d::PostDraw();
}

void PlayerObject::LustUpdate()
{
	//�}�b�v�`�b�v�Ƃ̓����蔻��
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	Vector3 normal = { 0,0,0 };
	//�㉺���E
	if (MapChip::GetInstance()->CheckMapChipAreaToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal)) {
		if (hitPos.x != 0) {
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * normal.x + normal.x;
		}
		if (hitPos.z != 0) {
			pos.z = hitPos.z + toMapChipCollider->GetRadiusY() * normal.z + normal.z;
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
	


	//�p
	else if (MapChip::GetInstance()->CheckMapChipToSphere2d(broadSphereCollider, &velocity, &hitPos)) {
		//Vector3 normal = { 0,0,0 };
		//if (hitPos.x != 0) {
		//	int vec = 1;	//����
		//	if (0 < velocity.x) {
		//		vec = -1;
		//	}
		//	pos.x = hitPos.x;
		//	normal.x = vec;
		//}
		//if (hitPos.z != 0) {
		//	int vec = 1;	//����
		//	if (velocity.z < 0) {
		//		vec = -1;
		//	}
		//	pos.z = hitPos.z;
		//	normal.z = vec;
		//}
		//normal.Normalize();
		//velocity = CalcWallScratchVector(velocity, normal);
	}
}



void PlayerObject::OnCollision(const CollisionInfo &info)
{
	Debris *debri;
	Enemy *enemy;
	switch (info.object->Tag)
	{
	case DEBRIS:
		//�j�Ђ��z���͈͂ƏՓ˂�����
		debri = dynamic_cast<Debris *>(info.object);
		if (info.myName == "hitCollider" &&
			info.collider->GetCollisionName() == "hitCollider" &&
			(!debri->isFirstAttack || debri->state == Debris::RETURN)) {
			//�z��
			energy += debri->GetSize();
		}

		isHealFrameIncrease = true;

		if (isHealFrameIncrease) {
			nowHealFrame++;
		}

		if (nowHealFrame <= maxHealFrame && isHealFrameIncrease){
			healParticle1->AddHeal(3, 40, pos, velocity);
			healParticle2->AddHeal(4, 40, pos, velocity);
			//boomParticle->AddBoom(2, 40, pos);
			//refParticle->AddRef(2, 40, pos,velocity);
		}
		else {
			isHealFrameIncrease = false;
			nowHealFrame = 0;
		}

		break;
	case ENEMY:
		enemy = dynamic_cast<Enemy*>(info.object);
		//�G�l�~�[���U���\��ԂȂ�_���[�W����������
		if (enemy->attack.can && !attack.is) {
			Damage(enemy->CauseDamage());
		}

		//�ʒu�C��
		
		//�U�����łȂ���Ή����Ԃ�����
		if (!attack.is) {
			penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.02f;
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
	const XMVECTOR &hitPos,		//�Փˈʒu
	const Vector3 &normal)
{
	velocity = CalcReflectVector(velocity, normal);
	if (!isBounce && attack.is) {
		velocity *= 3.0f;
		isBounce = true;
		refParticle->AddRef(20, 60, pos, velocity);
	}
}

void PlayerObject::Damage(float damage)
{
	//���G��������_���[�W���󂯂Ȃ�
	if (isInvincible) return;
	//isInvincible = true;
	energy -= damage;
	invincibleCounter = 60;

	//size��0�ɂȂ����玀�S����
	if (energy <= 0) {
		energy = 0;
	}

	boomParticle->AddBoom(2, 10, pos);
}


