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

PlayerObject::PlayerObject(XMFLOAT3 startPos):
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

	//�T�C�Y������
	size = 500.0f;

	scalef = ConvertSizeToScale(size);
	objectData->SetScale(scalef);
	//�����蔻��
	hitCollider = new SphereCollider("hitCollider", { 0,scalef * 180.0f,0 }, scalef * 180.0f);
	SetCollider(hitCollider);
	absorptionCollider = new SphereCollider("absorptionCollider", { 0,scalef * 180.0f,0 }, (scalef + 1) * 180.0f);
	SetCollider(absorptionCollider);
}


void PlayerObject::Initialize()
{
	//�T�C�Y������
	size = 500.0f;
	//�����t���O
	destructType = CIRCLE;
	//�z���͈�
	suction = scalef * suctionRatio;
	//�|�W�V����������
	pos = startPos;

}

void PlayerObject::Update()
{
	Input* input = Input::GetInstance();
	//�X�P�[������ړ��ʌ���
	moveSpead = scalef * 5;

	//�ړ��ʌ�������
	VelocityReset(false,0.8f);

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
	if (input->TriggerKey(DIK_3)||input->TriggerPadButton(BUTTON_RIGHT_SHOULDER)) {
		if (destructType != CIRCLE) {
			destructType = CIRCLE;
		}
		else {
			destructType = DIRECTIVITY;
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
			float shotSpeed = rand() % 12 + scalef * 20;
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
			
				startVec = velocity.Normalize();

				startVec.AddRotationY(shotRad);
				//startVec = startVec + offset;
				break;
			default:
				break;
			}


			//Debris�̃R���e�i�ɒǉ�
			Debris::debris.push_back(new Debris(pos, startVec * shotSpeed, shotSize,&pos));
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
	//�ړ��ʂ�K��
	Move();

	//�}�b�v�`�b�v�p�̔�����ړ�
	rect2d.Top		= -(int)((pos.z + scalef * 100.0f));
	rect2d.Bottom	= -(int)((pos.z - scalef * 100.0f));
	rect2d.Right	= (int)((pos.x + scalef * 100.0f) );
	rect2d.Left		= (int)((pos.x - scalef * 100.0f) );


	//�}�b�v�`�b�v�Ƃ̓����蔻��
	EdgeType contact_edge = EdgeType::EdgeTypeNon;
	float contact_pos = 0.0f;
	//X��
	if (MapChip::GetInstance()->CollisionRectAndMapchipEdgeVersion(
		rect2d,
		Vector3(velocity.x, 0, 0),
		contact_edge,
		contact_pos,
		MapChip::TEST_MAP
	)) {
		Vector3 hitPos = {
			contact_pos,
			0,
			pos.z,
		};
		Vector3 normal;
		if (contact_edge == EdgeType::EdgeTypeLeft) {
			DebugText::GetInstance()->Print("hitLeftMap", 0, 90, 3);

			normal = { -1,0,0 };
		}
		else {
			DebugText::GetInstance()->Print("hitRigthMap", 0, 90, 3);
			normal = { 1,0,0 };
		}
		HitWall(hitPos, normal);
	}
	//Y��
	if (MapChip::GetInstance()->CollisionRectAndMapchipEdgeVersion(
		rect2d,
		Vector3(0, 0, velocity.z),
		contact_edge,
		contact_pos,
		MapChip::TEST_MAP
	)) {
		Vector3 hitPos = {
			pos.x,
			0,
			contact_pos
		};
		Vector3 normal;
		if (contact_edge == EdgeType::EdgeTypeBottom) {
			normal = { 0,0,-1 };
			DebugText::GetInstance()->Print("hitBottomMap", 0, 120, 3);

		}
		else {
			DebugText::GetInstance()->Print("hitTopMap", 0, 120, 3);
			normal = { 0,0,1 };
		}
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
		DebugText::GetInstance()->Print("HitEnemy", 0, 60,3);
		break;

	case DEFAULT_BLOACK:
		DebugText::GetInstance()->Print("HitBox", 0, 60, 3);
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
	Vector3 HitPos = hitPos;
	pos = HitPos + normal * (rect2d.Bottom - rect2d.Top);
	velocity = CalcWallScratchVector(velocity, normal);


	//�}�b�v�`�b�v�p�̔�����ړ�
	rect2d.Top = -(int)((pos.z + scalef * 100.0f));
	rect2d.Bottom = -(int)((pos.z - scalef * 100.0f));
	rect2d.Right = (int)((pos.x + scalef * 100.0f));
	rect2d.Left = (int)((pos.x - scalef * 100.0f));
}

void PlayerObject::AdjustToMapchipEdgePosition(EdgeType contact_edge, float contact_pos)
{
	struct Vec2 {
		float x;
		float y;
		Vec2(float x, float y) {
			this->x = x;
			this->y = y;
		}
	};
		
	Vec2 offset = Vec2(rect2d.Bottom - rect2d.Top, rect2d.Bottom - rect2d.Top);

	switch (contact_edge)
	{
	case EdgeTypeLeft:
		pos.x = contact_pos - offset.x/2;
		break;
	case EdgeTypeRight:
		pos.x = contact_pos + offset.x / 2;
		break;
	case EdgeTypeTop:
		pos.z = contact_pos - offset.y/2;
		break;
	case EdgeTypeBottom:
		pos.z = contact_pos + offset.y /2;
		break;
	}
}
