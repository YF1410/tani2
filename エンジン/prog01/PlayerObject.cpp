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
	energy = 2000.0f;
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
	//����֌W
	collect = {
		true,
		false,
		300,
		0
	};


	//�A�j���[�V�����J�n
	objectData->PlayAnimation();

	// �p�[�e�B�N���}�l�[�W������
	healParticle1 = healParticle1->Create(L"heal3");
	healParticle2 = healParticle2->Create(L"heal4");
	healParticle1->SetStartScale(200.0f);
	healParticle2->SetStartScale(200.0f);
	healParticle1->SetCenter(400.0f);
	healParticle2->SetCenter(400.0f);

	boomParticle = boomParticle->Create(L"boom");
	boomParticle->SetStartScale(300.0f);
	boomParticle->SetCenter(400.0f);

	refParticle = refParticle->Create();
	refParticle->SetStartScale(300.0f);
	refParticle->SetCenter(400.0f);

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


	//�L�[�{�[�h�ړ�
	if (input->PushKey(DIK_A))	//��
	{
		velocity.x -= moveSpead;
	}
	else if (input->PushKey(DIK_D))	//�E
	{
		velocity.x += moveSpead;
	}
	if (input->PushKey(DIK_S))	//���
	{
		velocity.z -= moveSpead;
	}
	else if (input->PushKey(DIK_W))	//�O
	{
		velocity.z += moveSpead;

	}
	//�R���g���[���[�ł̈ړ�
	velocity.x += input->PadStickGradient().x * moveSpead;
	velocity.z += -input->PadStickGradient().y * moveSpead;


	//�f�o�b�O�p�T�C�Y�ύX
	if (input->PushKey(DIK_UP)) {
		energy += 10.0f;
	}
	if (input->PushKey(DIK_DOWN)) {
		energy -= 10.0f;
	}

	//����
	if ((input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(BUTTON_A))&&
		attack.can)
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
		energy -= SHOT_ENERGY;
		
	}

	//���
	if (input->TriggerKey(DIK_Q)|| input->TriggerPadButton(BUTTON_B)) {
		if (collect.Start()) {
			for (int i = 0; i < Debris::debris.size(); i++) {
				Debris::debris[i]->ReturnStart();
			}
		}
	}



	//�U���C���^�[�o��
	attack.Intervel();

	//����C���^�[�o��
	collect.Intervel();
	
	//�U���͍X�V
	attackPow = velocity.Length();


	//�T�C�Y����X�P�[���֕ϊ�
	scale = ConvertSizeToScale(energy / 2.0f);
	//�ړ��ʂ�K��
	PosAddVelocity();
	//�ړ��ʂ���u���[�h�R���C�_�[���X�V
	broadSphereCollider->SetRadius(/*velocity.Length() + pushBackCollider->GetRadius()*/scale.x * 120.0f);
	toMapChipCollider->SetRadius(scale.x * 120.0f, scale.x * 120.0f);

	DebugText::GetInstance()->Print("WASD stick : Move",600,0,3);
	DebugText::GetInstance()->Print("Qkey Bbutton: ReturnDebri",600,40,3);
	DebugText::GetInstance()->Print("SPACEkey Abutton : Boom",600,80,3);
	DebugText::GetInstance()->VariablePrint(0, 0, "playerSize", energy, 3);
	DebugText::GetInstance()->VariablePrint(0, 40, "DebrisCount", Debris::debris.size(), 3);
	DebugText::GetInstance()->VariablePrint(0, 80, "StayTimer", collect.timer, 3);
	DebugText::GetInstance()->VariablePrint(0, 160, "Speed", velocity.Length(), 3);
	
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
	//if (attack.is) {
	//	refParticle->AddRef(2, 40, pos, velocity);
	//}

	healParticle1->Update();
	healParticle2->Update();
	boomParticle->Update();
	refParticle->Update();
	
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
	healParticle1->Draw(cmdList);
	healParticle2->Draw(cmdList);
	boomParticle->Draw(cmdList);
	refParticle->Draw(cmdList);

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
	//�㉺���E
	if (MapChip::GetInstance()->CheckMapChipAreaToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal)) {
		normal.Normalize();
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

		normal.Normalize();
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
		DebugText::GetInstance()->Print("HitEnemy", 0, 80, 3);
		
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


