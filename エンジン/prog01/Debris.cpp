#include "Debris.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "SlimeMath.h"
#include "MapChip.h"

std::vector<Debris*> Debris::debris;
PlayerObject* Debris::playerData;

Debris::Debris(Vector3 startPos, Vector3 startVec, float size) :
	GameObjCommon(
		ModelManager::DEBRI,
		GameObjCommon::DEBRIS,
		false,
		startPos
	),
	size(size),
	startSize(size),
	isAlive(true),
	isAttack(true),
	isFirstAttack(true),
	reversFlag(false)
{
	//�T�C�Y����X�P�[���փR���o�[�g
	scale = 1.0f/*= ConvertSizeToScale(size)*/;
	rotate = { (float)(rand() % 361) ,(float)(rand() % 361) ,(float)(rand() % 361) };
	//���������x�Z�b�g
	velocity = startVec;
	//�ŏ�����U�����
	state = ATTACK;

	isBoost = false;

	////�}�b�v�`�b�v�p�R���C�_�[
	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, scale.x * 150.0f, scale.x * 150.0f);
	SetNarrowCollider(toMapChipCollider);

	//�����Ԃ�����p�R���C�_�[
	hitCollider = new SphereCollider("hitCollider");
	hitCollider->SetRadius(scale.x * 120.0f);
	hitCollider->SetOffset({ 0,hitCollider->GetRadius(),0 });
	SetBroadCollider(hitCollider);
	//�U������p�R���C�_�[
	attackCollider = new SphereCollider("attackCollider");
	attackCollider->SetRadius(scale.x * 150.0f);
	attackCollider->SetOffset({ 0,attackCollider->GetRadius(),0 });
	SetNarrowCollider(attackCollider);


	exclusionList.push_back(DEBRIS);
}

Debris::~Debris()
{
}

void Debris::Update()
{
	oldPos = pos;
	VelocityReset();
	//��������t���O����������X�e�[�g��ύX
	//�U���I��
	if (velocity.Length() <= 10.0f && isAttack) {
		isFirstAttack = false;
		isAttack = false;
		isBoost = false;
	}
	if (velocity.Length() >= 10.0f) {
		isAttack = true;
	}
	//�ړ��ʐ���
	if (velocity.Length() >= 1000) {
		velocity = velocity.Normal() * 1000;
	}

	if (!isAttack) {
		damage = size - (size * 0.995f);
		size -= damage;
		scale *= 0.995f;
		//scale = ConvertSizeToScale(size);

	}
	if (scale.x < 0.5f) {
		damage = size;
		isAlive = false;
	}

	switch (state)
	{
	case Debris::NOUPDATE:
		break;
	case Debris::STAY:

		break;
	case Debris::ATTACK:

		break;
	case Debris::RETURN:
		if (velocity.Length() <= 200) {
			Vector3 targetVec = playerData->pos - pos;
			targetVec.y = 0;
			velocity = targetVec.Normal() * 100;
		}
		//
		if (returnTimer-- <= 0) {
			state = STAY;
		}
		break;
	case Debris::SUCTION:
		velocity *= 10;

		break;
	default:
		break;
	}

	//�ړ��ʂ�K��
	//�q�b�g�X�g�b�v
	/*if (isHitStop) {
		velocity = 0;
	}
	else if (!isHitStop) {
		PosAddVelocity();
	}*/
	PosAddVelocity();
	//pos.y = 0;

	//�q�b�g�X�g�b�v
	if (isHitStop) {
		hitStopTimer++;
		if (hitStopTimer >= 5) {
			hitStopTimer = 0;
			isHitStop = false;
		}
	}

}

void Debris::LustUpdate()
{
	//�}�b�v�`�b�v�Ƃ̓����蔻��
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	Vector3 normal = { 0,0,0 };
	//�㉺���E
	if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal, &oldPos)) {
		if (hitPos.x != 0) {
			pos.x = hitPos.x + toMapChipCollider->GetRadiusX() * normal.x;
		}
		if (hitPos.z != 0) {
			pos.z = hitPos.z + toMapChipCollider->GetRadiusY() * normal.z;
		}
		normal.Normalize();
		HitWall(hitPos, normal.Normal());
		state = ATTACK;
	}
	//�p
	//else if (MapChip::GetInstance()->CheckMapChipToSphere2d(broadSphereCollider, &velocity, &hitPos)) {
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
	//}
}

void Debris::VelocityReset()
{
	//��C��R
	airResistance = velocity * 0.05f;
	velocity -= airResistance;
}


void Debris::StaticInitialize(PlayerObject* player)
{
	playerData = player;
}

void Debris::StaticUpdate()
{

	//�폜
	for (int i = debris.size() - 1; i >= 0; i--) {
		if (!debris[i]->isAlive) {
			delete debris[i];
			debris.erase(debris.begin() + i);
		}
	}
	//�X�V
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->Update();
	}
}

void Debris::StaticLustUpdate()
{
	
	//�X�V
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->LustUpdate();
	}
}

void Debris::StaticAdaptation()
{
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->Adaptation();
	}
}

void Debris::StaticDraw()
{
	for (int i = 0; i < debris.size(); i++) {
		debris[i]->Draw();
	}
}

void Debris::Finalize()
{

	//�폜
	for (auto& a : debris)
	{
		delete a;
	}
	debris.clear();
}

float Debris::GetTotarStartSize()
{
	float total = 0.0f;
	for (int i = 0; i < debris.size(); i++) {
		total += debris[i]->startSize;
	}
	return total;
}

float Debris::GetTotarNowSize()
{
	float total = 0.0f;
	for (int i = 0; i < debris.size(); i++) {
		total += debris[i]->size;
	}
	return total;
}

void Debris::OnCollision(const CollisionInfo& info)
{
	Vector3 normal;
	switch (info.object->Tag)
	{
	case PLAYER:
		if (!isFirstAttack)
		{
			//���
			if (info.myName == "hitCollider" &&
				info.collider->GetCollisionName() == "absorptionCollider" &&
				!isFirstAttack) {
				velocity += Vector3(playerData->GetPos() - GetPos()).Normalize() * 5.0f;
			}
			//�����蔻��p�̃R���C�_�[�ƃv���C���[�̓����蔻��R���C�_�[���������Ă�����폜
			if (info.myName == "hitCollider" &&
				info.collider->GetCollisionName() == "hitCollider" &&
				(!isFirstAttack || state == RETURN)) {
				isAlive = false;
			}
		}
		break;
	}

}


void Debris::ReturnStart()
{
	//�X�e�[�g�����^�[����
	state = RETURN;
	velocity *= Vector3(pos - playerData->pos).Normal();
	isFirstAttack = false;
	returnTimer = 90;
}

void Debris::SuckedPlayer(const Vector3& playerPos, const float& suckedRadius)
{
	//�ړ��J�n
	velocity += Vector3(playerPos - pos).Normal() * 3.0f;
}

void Debris::HitWall(const XMVECTOR& hitPos, const Vector3& normal)
{
	velocity = CalcReflectVector(velocity, normal);
	if (!isBoost) {
		velocity *= 3.0f;
		isBoost = true;
	}
}


void Debris::Damage(float damage)
{
}
