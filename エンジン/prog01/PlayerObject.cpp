#include "PlayerObject.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>

using namespace DirectX;

PlayerObject::PlayerObject(Model* slimeModel, Model* coreModel)
{
	slime = Slime::Create(slimeModel);
	slime->SetPosition({ 0,0,0 });

	//�����蔻������p
	sphereModel = Model::CreateFromObject("sphere", true);
	sphereModel->SetAlpha(0.5f);
	sphereOBJ = Object3d::Create(sphereModel.get());
	sphereOBJ->SetPosition({ 0, 0.6f, 0 });

	srand(time(NULL));
}

PlayerObject::~PlayerObject()
{
	delete slime;
}

void PlayerObject::Init()
{
	destructFlag = false;
	destructType = CIRCLE;
	destructPow = WEAK;
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

	//����
	if (input->TriggerKey(DIK_SPACE) && !destructFlag)
	{
		destructFlag = true;
	}
	//������
	if (destructFlag) {
		

		//�����З͂Ɣ�����������j�Ђ̐����v�Z
		for (int i = 0; i < destructPow; i++) {
			Vector3 startVec;		//���x*�����x�N�g��
			float shotRad;			//�p�x����p
			float speed = 1.0f;		//���ˑ��x
			float size = 1.0f;		//�c�[�̃T�C�Y
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
			//Debris::debrisContainer.push_back(Debris::Create(pos, startVec.Normalize() * speed, size));
		}
		//�����I��
		destructFlag = false;
	}
	
	pos += moveVec;
	slime->SetPosition(pos);
	slime->Update();
	sphereOBJ->SetPosition(pos);
	sphereOBJ->Update();
}

void PlayerObject::Draw()
{
	slime->Draw();
	sphereOBJ->Draw();
}