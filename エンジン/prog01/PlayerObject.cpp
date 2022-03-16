#include "PlayerObject.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Debris.h"

using namespace DirectX;

PlayerObject::PlayerObject(FbxModel *coreModel)
{
	slime = FbxObject3d::Create(ModelManager::GetIns()->GetModel(SLIME));
	slime->SetScale(1.0f);
	pos = { 0,0,0 };
	


	srand(time(NULL));
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::Init()
{
	destructFlag = false;
	destructType = CIRCLE;
	destructPow = WEAK;
	//�T�C�Y������
	size = 100.0f;
	scale = 1.0f;
}

void PlayerObject::Update()
{
	Input* input = Input::GetInstance();

	//�ړ��ʌ�������
	moveVec = {0,0,0};

	//�L�[�{�[�h�ړ�
	if (input->PushKey(DIK_A))	//��
	{
		moveVec.x -= speed;
	}
	else if (input->PushKey(DIK_D))	//�E
	{
		moveVec.x += speed;
	}
	if (input->PushKey(DIK_S))	//���
	{
		moveVec.z -= speed;
	}
	else if (input->PushKey(DIK_W))	//�O
	{
		moveVec.z += speed;

	}

	//���j�З͕ύX
	if (input->PushKey(DIK_1)) {
		destructPow = STRONG;
	}
	if (input->PushKey(DIK_2)) {
		destructPow = WEAK;
	}

	//�f�o�b�O�p�T�C�Y�ύX
	if (input->PushKey(DIK_Q)) {
		size += 0.1f;
	}
	if (input->PushKey(DIK_E)) {
		size -= 0.1f;
	}


	//����
	if (input->TriggerKey(DIK_SPACE) && !destructFlag)
	{
		destructFlag = true;
	}
	
	

	//���j����
	if (destructFlag) {
		//�V���b�g�Ɏg�������T�C�Y
		float maxSize = size * shotPercentage;

		//�����З͂Ɣ�����������j�Ђ̐����v�Z
		for (int i = 0; i < destructPow; i++) {
			Vector3 startVec;		//���x*�����x�N�g��
			float shotRad;			//�p�x����p
			float shotSpeed = rand() % destructPow + size * 0.5f;		//���ˑ��x
			float shotSize;		//�c�[�̃T�C�Y
			
				shotSize = maxSize / destructPow;
			
			switch (destructType)
			{
			case CIRCLE:		//�~�`����
				shotRad = XMConvertToRadians(rand() % 360);		//360�x�Ōv�Z
				startVec = {
					cos(shotRad),
					0,
					sin(shotRad)
				};
				break;
			case DIRECTIVITY:	//�w��������
				//shotRad = XMConvertToRadians(rand() % 60);		//360�x�Ōv�Z
				//startVec = {
				//	cos(shotRad),
				//	0,
				//	sin(shotRad)
				//};

				break;
			default:
				break;
			}


			//Debris�̃R���e�i�ɒǉ�
			Debris::debris.push_back(new Debris(pos, startVec.Normalize() * shotSpeed, shotSize));
		}
		//�����I��
		size -= maxSize;

		destructFlag = false;
	}


	scale = size / 100.0f;

	//�X���C���̈ړ��K��
	pos += moveVec;
	slime->SetPosition(pos);
	slime->SetScale(scale);

	slime->Update();
}

void PlayerObject::Draw()
{
	slime->Draw(DirectXCommon::GetInstance()->GetCommandList());
}