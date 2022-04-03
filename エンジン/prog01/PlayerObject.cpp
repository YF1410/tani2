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
		ModelManager::SLIME,	//�X���C���̃��f�����Z�b�g
		GameObjCommon::PLAYER,	//�v���C���[�Ƃ��Ĉ���
		false					//�d�͂̉e�����󂯂�
	)
{
	srand(time(NULL));			//���j�p�����̃V�[�h�l���Z�b�g
	//�����ʒu�ɐݒ�
	this->startPos = startPos;
	pos = startPos;

	//�����蔻��
	broadSphereCollider = new SphereCollider("hitCollider", { 0,scalef * 180.0f,0 }, scalef * 180.0f);
	SetBroadCollider(broadSphereCollider);

	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);
	//absorptionCollider->Update();
}


void PlayerObject::Initialize()
{
	isCheckPoint = false;
	//�T�C�Y������
	size = 500.0f;
	//�T�C�Y������
	scalef = ConvertSizeToScale(size);
	objectData->SetScale(scalef);
	toMapChipCollider->SetRadius(scalef * 180.0f, scalef * 180.0f);
	//�����t���O
	destructType = CIRCLE;
	//�z���͈�
	suction = scalef * suctionRatio;
	//�|�W�V����������
	pos = startPos;

	reverseArea = MIN;
}

void PlayerObject::Update()
{
	Input *input = Input::GetInstance();
	//�X�P�[������ړ��ʌ���
	moveSpead = scalef * 5;

	//�ړ��ʌ�������
	VelocityReset(false, 0.9f);

	//���Z�b�g
	if (input->PushKey(DIK_R)) {
		Initialize();
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

	velocity.x += input->PadStickGradient().x * moveSpead;
	velocity.z += -input->PadStickGradient().y * moveSpead;


	//���j�^�C�v�؂�ւ�
	if (input->TriggerKey(DIK_3) || input->TriggerPadButton(BUTTON_RIGHT_SHOULDER)) {
		if (destructType != CIRCLE) {
			destructType = CIRCLE;
		}
		else {
			destructType = DIRECTIVITY;
		}
	}
	//���ˋ����ύX
	if (input->TriggerKey(DIK_4)) {
		if (reverseArea != MIN) {
			reverseArea = MIN;
		}
		else {
			reverseArea = REVERSE_AREA::MAX;
		}
	}



	//�f�o�b�O�p�T�C�Y�ύX
	if (input->PushKey(DIK_UP)) {
		size += 10.0f;
	}
	if (input->PushKey(DIK_DOWN)) {
		size -= 10.0f;
	}

	//����
	if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(BUTTON_A))
	{
		//�V���b�g�Ɏg�������T�C�Y
		float maxSize = size * 0.5f;
		//�����З͂Ɣ�����������j�Ђ̐����v�Z
		for (int i = 0; i < destructPow * destructType; i++) {
			Vector3 startVec;		//���x*�����x�N�g��
			float shotRad;			//�p�x����p
			//���˃X�s�[�h
			float shotSpeed = rand() % 12 + scalef * 60;
			//�c�[�̃T�C�Y
			float shotSize = maxSize / (destructPow * destructType);

			switch (destructType)
			{
			case CIRCLE:		//�~�`����
				//���ˊp�x��360�x�Ōv�Z
				shotRad = XMConvertToRadians(rand() % 360);
				startVec = {
					static_cast<float>(cos(shotRad)),
					0,
					static_cast<float>(sin(shotRad))
				};
				break;

			case DIRECTIVITY:	//�w��������
				//-15~15�x�Ōv�Z
				shotRad = XMConvertToRadians(rand() % 30 - 15);

				startVec = velocity.Normal();

				startVec.AddRotationY(shotRad);
				//startVec = startVec + offset;
				break;
			default:
				break;
			}

			velocity += velocity.Normal() * 10;
			//Debris�̃R���e�i�ɒǉ�
			Debris::debris.push_back(new Debris(pos, startVec * shotSpeed, shotSize, &pos,reverseArea));
		}
		//�����I��
		size -= maxSize;

	}

	//���
	if (input->TriggerKey(DIK_1)) {
		for (int i = 0; i < Debris::debris.size(); i++) {
			Debris::debris[i]->ReturnStart();
		}
	}



	//�T�C�Y����X�P�[���֕ϊ�
	scalef = ConvertSizeToScale(size);
	scale = scalef;
	broadSphereCollider->SetRadius(scalef * 180.0f + 20.0f);
	toMapChipCollider->SetRadius( scalef * 150.0f, scalef * 150.0f);
	//�ړ��ʂ�K��
	Move();
	DebugText::GetInstance()->VariablePrint(800, 0, "pos.x", pos.x, 3);
	DebugText::GetInstance()->VariablePrint(800, 40, "pos.y", pos.z, 3);
	DebugText::GetInstance()->VariablePrint(800, 320, "R", toMapChipCollider->GetRadiusX(), 3);

	//�}�b�v�`�b�v�Ƃ̓����蔻��
	toMapChipCollider->Update();
	Vector3 hitPos = {0,0,0};
	Vector3 oldPos;
	if (MapChip::GetInstance()->CheckHitMapChip(toMapChipCollider, &velocity, &hitPos)) {
		Vector3 normal = {0,0,0};
		
		if (hitPos.x != 0) {
			int vec = 1;	//����
			if (0 < velocity.x) {
				vec = -1;
			}
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * vec;
			normal.x = vec;
		}
		if (hitPos.z != 0) {
			int vec = 1;	//����
			if (velocity.z < 0) {
				vec = -1;
			}
			pos.z = hitPos.z - toMapChipCollider->GetRadiusY() * vec;
			normal.z = vec;
		}
		normal.Normalize();
		HitWall(hitPos, normal);
	}


}



void PlayerObject::OnCollision(const CollisionInfo &info)
{
	Debris *debri;
	switch (info.object->Tag)
	{
	case DEBRIS:
		//�j�Ђ��z���͈͂ƏՓ˂�����
		debri = dynamic_cast<Debris *>(info.object);
		if (info.myName == "hitCollider" &&
			info.collider->GetCollisionName() == "hitCollider" &&
			!debri->isFirstAttack) {
			//�z��
			size += debri->GetSize();
		}
		break;
	case ENEMY:
		DebugText::GetInstance()->Print("HitEnemy", 0, 80, 3);
		break;

	case DEFAULT_BLOACK:
		/*

		pos = (info.object->pos );
		velocity = CalcWallScratchVector(velocity, info);*/

		break;

	default:
		break;
	}
}

void PlayerObject::HitWall(
	const XMVECTOR &hitPos,		//�Փˈʒu
	const Vector3 &normal)
{
	velocity = CalcWallScratchVector(velocity, normal);
}

