#include "PlayerObject.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Debris.h"
#include "SlimeMath.h"
#include "DebugText.h"

using namespace DirectX;

PlayerObject::PlayerObject():
	GameObjCommon(
		ModelManager::SLIME,	//�X���C���̃��f�����Z�b�g
		GameObjCommon::Player,	//�v���C���[�Ƃ��Ĉ���
		false					//�d�͂̉e�����󂯂�
	)
{
	srand(time(NULL));			//���j�p�����̃V�[�h�l���Z�b�g

	//�T�C�Y������
	size = 500.0f;
	scale = ConvertSizeToScale(size);
	objectData->SetScale(scale);
	//�����蔻�菉����
	hitSphere = new SphereCollider();
	hitSphere->SetRadius(/*scale * 180.0f*/1.0f);
	hitSphere->SetOffset({ 0,hitSphere->GetRadius(),0 });
	SetCollider(hitSphere);
}


void PlayerObject::Initialize()
{
	//�����t���O
	destructFlag = false;
	destructType = CIRCLE;
	destructPow = WEAK;
	//�z���͈�
	suction = scale * suctionRatio;
	//�|�W�V����������
	pos = { 0,100,0 };

}

void PlayerObject::Update()
{
	Input* input = Input::GetInstance();
	//�X�P�[������ړ��ʌ���
	moveSpead = scale * 5;

	//�ړ��ʌ�������
	VelocityReset(false,0.5f);

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
	if (input->PushKey(DIK_S))	//��
	{
		velocity.y -= moveSpead;
	}
	else if (input->PushKey(DIK_W))	//��
	{
		velocity.y += moveSpead;

	}

	velocity.x += input->PadStickGradient().x * moveSpead;
	velocity.z += -input->PadStickGradient().y * moveSpead;


	//���j�З͕ύX
	if (input->TriggerKey(DIK_1) || input->TriggerPadButton(BUTTON_LEFT_SHOULDER)) {
		if (destructType != STRONG) {
			destructPow = STRONG;
		}
		else {
			destructPow = WEAK;
		}
	}
	//���j�^�C�v�؂�ւ�
	if (input->TriggerKey(DIK_3)||input->TriggerPadButton(BUTTON_RIGHT_SHOULDER)) {
		if (destructType != CIRCLE) {
			destructType = CIRCLE;
		}
		else {
			destructType = DIRECTIVITY;
		}
	}



	//�f�o�b�O�p�T�C�Y�ύX
	if (input->PushKey(DIK_Q)) {
		size += 10.0f;
	}
	if (input->PushKey(DIK_E)) {
		size -= 10.0f;
	}


	//����
	if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(BUTTON_A) && !destructFlag)
	{
		destructFlag = true;
	}
	
	

	//���j����
	if (destructFlag) {
		//�V���b�g�Ɏg�������T�C�Y
		float maxSize;
		if (destructPow == WEAK) {
			maxSize = size * 0.2f;
		}
		else if (destructPow == STRONG ){
			maxSize = size * 0.5f;
		}

		//�����З͂Ɣ�����������j�Ђ̐����v�Z
		for (int i = 0; i < destructPow * destructType; i++) {
			Vector3 startVec;		//���x*�����x�N�g��
			float shotRad;			//�p�x����p
			float shotSpeed ;		//���ˑ��x
			//�c�[�̃T�C�Y
			float shotSize = maxSize / (destructPow * destructType);

			switch (destructType)
			{
			case CIRCLE:		//�~�`����
				//�T�C�Y�ƃX�s�[�h
				if (destructPow == WEAK) {
					shotSpeed = rand() % 4 + scale * 10;
				}
				else if (destructPow == STRONG) {
					shotSpeed = rand() % 12 + scale * 100;
				}
				

				shotRad = XMConvertToRadians(rand() % 360);		//360�x�Ōv�Z
				startVec = {
					static_cast<float>(cos(shotRad)),
					0,
					static_cast<float>(sin(shotRad))
				};
				break;

			case DIRECTIVITY:	//�w��������
				//�T�C�Y�ƃX�s�[�h
				if (destructPow == WEAK) {
					shotSpeed = rand() % 4 + scale * 10;
				}
				else if (destructPow == STRONG) {
					shotSpeed = rand() % 12 + scale * 100;
				}


				shotRad = XMConvertToRadians(rand() % 15 - 15);		//360�x�Ōv�Z
				startVec = {
					static_cast<float>(cos(shotRad)),
					0,
					static_cast<float>(sin(shotRad))
				};
				break;
				break;
			default:
				break;
			}

			
			//Debris�̃R���e�i�ɒǉ�
			Debris::debris.push_back(new Debris(pos, startVec.Normalize() *shotSpeed, shotSize));
		}
		//�����I��
		size -= maxSize;

		destructFlag = false;
	}

	
	//�T�C�Y����X�P�[���֕ϊ�
	scale = ConvertSizeToScale(size);

	//�ړ��ʂ�K��
	Move();
	
	//�R���C�_�[�X�V
	//ColliderUpdate();
}


void PlayerObject::Absorb(float size)
{
	this->size += size;
}


void PlayerObject::OnCollision(const CollisionInfo &info)
{
	switch (info.object->Tag)
	{
	case ENEMY:
		DebugText::GetInstance()->Print("HitEnemy", 0, 60,3);
		break;

	case DEFAULT_BLOACK:
		DebugText::GetInstance()->Print("HitBox", 0, 60, 3);
		break;
		
	default:
		break;
	}
}

void PlayerObject::HitWall(
	const XMVECTOR &hitPos,		//�Փˈʒu
	const Vector3 &normal)
{
	//velocity = CalcWallScratchVector(velocity, normal);
	////�R���C�_�[�X�V
	//ColliderUpdate();
}
