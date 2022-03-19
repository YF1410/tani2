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
	slimeObj = FbxObject3d::Create(ModelManager::GetIns()->GetModel(SLIME));
	


	srand(time(NULL));
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::Init()
{
	//�����t���O
	destructFlag = false;
	destructType = CIRCLE;
	destructPow = WEAK;
	//�z���͈�
	suction = scale * suctionRatio;
	//�|�W�V����������
	pos = { 0,100,0 };
	//�T�C�Y������
	size = 500.0f;
	scale = ConvertSizeToScale(size);
	slimeObj->SetScale(scale);
	//�����蔻�菉����
	collider.absorbSphere.radius = 100.0f;
}

void PlayerObject::Update()
{
	Input* input = Input::GetInstance();
	//�X�P�[������ړ��ʌ���
	moveSpead = scale * 20;

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
	//debug�p����
	if (input->PushKey(DIK_Z))	//��
	{
		moveVec.y -= moveSpead;
	}
	else if (input->PushKey(DIK_X))	//��
	{
		moveVec.y+= moveSpead;

	}

	//���j�З͕ύX
	if (input->PushKey(DIK_1)) {
		destructPow = STRONG;
	}
	if (input->PushKey(DIK_2)) {
		destructPow = WEAK;
	}
	//���j�^�C�v�ύX
	if (input->PushKey(DIK_3)) {
		destructType = CIRCLE;
	}
	if (input->PushKey(DIK_4)) {
		destructType = DIRECTIVITY;
	}



	//�f�o�b�O�p�T�C�Y�ύX
	if (input->PushKey(DIK_Q)) {
		size += 10.0f;
	}
	if (input->PushKey(DIK_E)) {
		size -= 10.0f;
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
			Debris::debris.push_back(new Debris(pos, startVec.Normalize() * shotSpeed, shotSize));
		}
		//�����I��
		size -= maxSize;

		destructFlag = false;
	}

	//�T�C�Y����X�P�[���֕ϊ�
	scale = ConvertSizeToScale(size);

	//�R���C�_�[�X�V
	UpdateCollider();
}

void PlayerObject::Adaptation()
{
	//�`��ʒu����
	pos = afterPos;

	slimeObj->SetPosition(pos);
	slimeObj->SetScale(scale);

	slimeObj->Update();
}

void PlayerObject::Draw()
{
	slimeObj->Draw(DirectXCommon::GetInstance()->GetCommandList());
}

void PlayerObject::Absorb(float size)
{
	this->size += size;
}

void PlayerObject::UpdateCollider()
{
	//�ړ���̈ʒu�\��
	afterPos = pos + moveVec;

	//�ړ���̋z���͈�
	suction = scale * suctionRatio;
	
	//�����ڂɋ߂�����
	collider.realSphere.center = afterPos;
	collider.realSphere.radius = scale * 150.0f;
	//�z���񂹗p
	collider.suctionSphere.center = afterPos;
	collider.suctionSphere.radius = suction;
	//�z���p
	collider.absorbSphere.center = afterPos;
	collider.absorbSphere.radius = scale * 100.0f;
}

void PlayerObject::HitWall(
	const XMVECTOR &hitPos,		//�Փˈʒu
	const Vector3 &normal)
{
	pos = hitPos + normal * collider.realSphere.radius;
	moveVec = CalcWallScratchVector(moveVec, normal);
	//�R���C�_�[�X�V
	UpdateCollider();
}
