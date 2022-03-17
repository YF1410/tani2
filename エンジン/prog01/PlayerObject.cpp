#include "PlayerObject.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Debris.h"
#include "SlimeMath.h"

using namespace DirectX;

PlayerObject::PlayerObject(FbxModel *coreModel)
{
	slime = FbxObject3d::Create(ModelManager::GetIns()->GetModel(SLIME));
	slime->SetScale(1.0f);
	


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
	//�|�W�V����������
	pos = { 0,0,0 };
	//�T�C�Y������
	size = 100.0f;
	scale = 1.0f;
	//�����蔻�菉����
	collider.absorbSphere.radius = 100.0f;
}

void PlayerObject::Update()
{
	Input* input = Input::GetInstance();

	//�ړ��ʌ�������
	moveVec = {0,0,0};

	//���Z�b�g
	if (input->PushKey(DIK_R)) {
		Init();
	}

	//�L�[�{�[�h�ړ�
	if (input->PushKey(DIK_A))	//��
	{
		moveVec.x -= moveSpead;
	}
	else if (input->PushKey(DIK_D))	//�E
	{
		moveVec.x += moveSpead;
	}
	if (input->PushKey(DIK_S))	//���
	{
		moveVec.z -= moveSpead;
	}
	else if (input->PushKey(DIK_W))	//�O
	{
		moveVec.z += moveSpead;

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
		float maxSize;
		if (destructPow == WEAK) {
			maxSize = size * 0.2f;
		}
		else if (destructPow == STRONG ){
			maxSize = size * 0.4f;
		}

		//�����З͂Ɣ�����������j�Ђ̐����v�Z
		for (int i = 0; i < destructPow; i++) {
			Vector3 startVec;		//���x*�����x�N�g��
			float shotRad;			//�p�x����p
			float shotSpeed ;		//���ˑ��x
			float shotSize;		//�c�[�̃T�C�Y

			if (destructPow == WEAK) {
				shotSpeed = rand() % 10 + size * 0.5f + 10;
			}
			else if (destructPow == STRONG) {
				shotSpeed = rand() % 10 + size + 40;
			}
			shotSize = maxSize / destructPow;

			switch (destructType)
			{
			case CIRCLE:		//�~�`����
				shotRad = XMConvertToRadians(rand() % 360);		//360�x�Ōv�Z
				startVec = {
					static_cast<float>(cos(shotRad)),
					0,
					static_cast<float>(sin(shotRad))
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

	//�T�C�Y����X�P�[���֕ϊ�
	scale = SizeToScaleConvert(size);
	//�R���C�_�[�X�V
	collider.absorbSphere.center = pos;
	collider.absorbSphere.radius = scale *1.1f * 100.0f;

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

void PlayerObject::Absorb(float size)
{
	this->size += size;
}
