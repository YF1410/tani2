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
		{0,0,0},
		{0,0,0},
		{0,0,0},
		true
	)
{
	srand(time(NULL));			//���j�p�����̃V�[�h�l���Z�b�g
	//�����ʒu�ɐݒ�
	this->startPos = startPos;
	pos = startPos;

	//�u���[�h
	broadSphereCollider = new SphereCollider("hitCollider", { 0,scalef * 180.0f,0 }, scalef * 180.0f);
	SetBroadCollider(broadSphereCollider);
	//�����Ԃ��p
	pushBackCollider = new SphereCollider("hitCollider", { 0,scalef * 180.0f,0 }, scalef * 180.0f);
	SetNarrowCollider(pushBackCollider);
	//�U���p
	attackCollider = new SphereCollider("hitCollider", { 0,scalef * 180.0f,0 }, scalef * 180.0f + 50.0f);
	SetNarrowCollider(pushBackCollider);

	//�}�b�v�`�b�v�p
	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);

	coreUp = new GameObjCommon(ModelManager::SLIME_CORE, Notag, false,{ 0,0,0 }, {1.5f,1.5f,1.5f });

	
}


void PlayerObject::Initialize()
{
	//�T�C�Y������
	size = 500.0f;
	//�T�C�Y������
	scalef = ConvertSizeToScale(size);
	objectData->SetScale(scalef);
	toMapChipCollider->SetRadius(scalef * 180.0f, scalef * 180.0f);
	//�z���͈�
	suction = scalef * suctionRatio;
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


	objectData->SetAlpha(0.5f);

	//�A�j���[�V�����J�n
	objectData->PlayAnimation();


}

void PlayerObject::Update()
{
	Input *input = Input::GetInstance();
	//�X�P�[������ړ��ʌ���
	moveSpead = scalef * 4;
	//�y�i���e�B���Z�b�g
	penalty = { 0,0,0 };

	//�ړ��ʌ�������
	VelocityReset(0.9f);
	if (velocity.Length() >= 200) {
		velocity = velocity.Normal() * 200;
	}
	if (attack.is && velocity.Length() < 60) {
		attack.is = false;
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
		size += 10.0f;
	}
	if (input->PushKey(DIK_DOWN)) {
		size -= 10.0f;
	}

	//����
	if ((input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(BUTTON_A))&&
		attack.can)
	{
		//�U���J�n
		attack.Start();

		//�V���b�g�Ɏg�������T�C�Y
		float maxSize = size * 0.5f;

		//�j�А���
		for (int i = 0; i < destructPow; i++) {
			Vector3 startVec;		//���x*�����x�N�g��
			float shotRad;			//�p�x����p
			//���˃X�s�[�h
			float shotSpeed = rand() % 20 + scalef * 40;
			//�c�[�̃T�C�Y
			float shotSize = maxSize / destructPow;

			//-15~15�x�Ōv�Z
			shotRad = XMConvertToRadians(rand() % 120 - 60);

			startVec = -velocity.Normal();

			startVec.AddRotationY(shotRad);
			//startVec = startVec + offset;

			velocity += velocity.Normal() * 50;
			//Debris�̃R���e�i�ɒǉ�
			Debris::debris.push_back(new Debris(pos, startVec * shotSpeed, shotSize));
		}
		//�����I��
		size -= maxSize;
		
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


	//�T�C�Y����X�P�[���֕ϊ�
	scalef = ConvertSizeToScale(size);
	scale = scalef;
	pushBackCollider->SetRadius(scalef * 120.0f);
	toMapChipCollider->SetRadius( scalef * 120.0f, scalef * 120.0f);
	//�ړ��ʂ�K��
	Move();
	//�ړ��ʂ���u���[�h�R���C�_�[���X�V
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
	//�}�b�v�`�b�v�Ƃ̓����蔻��
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	//�㉺���E
	if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos)) {
		Vector3 normal = { 0,0,0 };

		if (hitPos.x != 0) {
			int vec = 1;	//����
			if (0 < moveVec.x) {
				vec = -1;
			}
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * vec;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//����
			if (moveVec.z < 0) {
				vec = -1;
			}
			pos.z = hitPos.z - toMapChipCollider->GetRadiusY() * vec;
			normal.z = vec;
		}
		normal.Normalize();
		HitWall(hitPos, normal);
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
			size += debri->GetSize();
		}
		break;
	case ENEMY:
		enemy = dynamic_cast<Enemy*>(info.object);
		//�G�l�~�[���U���\��ԂȂ�_���[�W����������
		if (enemy->attack.can) {
			Damage(enemy->Attack());
		}

		//�ʒu�C��
		DebugText::GetInstance()->Print("HitEnemy", 0, 80, 3);
		
		//�U�����łȂ���Ή����Ԃ�����
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
	const XMVECTOR &hitPos,		//�Փˈʒu
	const Vector3 &normal)
{
	velocity = CalcReflectVector(velocity, normal);
}

void PlayerObject::Damage(float damage)
{
	//���G��������_���[�W���󂯂Ȃ�
	if (isInvincible) return;
	//isInvincible = true;
	size -= damage;
	invincibleCounter = 60;

	//size��0�ɂȂ����玀�S����
	if (size <= 0) {
		size = 0;
		
	}

}


