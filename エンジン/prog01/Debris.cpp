#include "Debris.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "SlimeMath.h"

std::vector<Debris *> Debris::debris;

Debris::Debris(Vector3 startPos, Vector3 startVec, float size,Vector3 *playerPos) :
	GameObjCommon(
		ModelManager::SLIME_BREAK,
		GameObjCommon::DEBRIS,
		false,
		startPos
	),
	size(size),
	isAlive(true),
	isAttack(true),
	isFirstAttack(true)
{
	//�T�C�Y����X�P�[���փR���o�[�g
	scale = ConvertSizeToScale(size);
	//���������x�Z�b�g
	velocity = startVec;
	//�ŏ�����U�����
	state = ATTACK;
	//�v���C���[�̈ʒu�Z�b�g
	this->playerPos = playerPos;

	//�����Ԃ�����p�R���C�_�[
	hitCollider = new SphereCollider("hitCollider");
	hitCollider->SetRadius(scale.x * 180.0f);
	hitCollider->SetOffset({ 0,hitCollider->GetRadius(),0 });
	//�U������p�R���C�_�[
	attackCollider = new SphereCollider("attackCollider");
	attackCollider->SetRadius(scale.x * 180.0f + 20.0f);
	attackCollider->SetOffset({ 0,attackCollider->GetRadius(),0 });
	SetCollider(hitCollider);
	SetCollider(attackCollider);
	//�c�[�ǂ����͔��肵�Ȃ�
	exclusionList.push_back(DEBRIS);
}

void Debris::Update()
{
	VelocityReset();
	//��������t���O����������X�e�[�g��ύX
	//�U���I��
	if (velocity.Length() <= 10.0f && isAttack) {
		isFirstAttack = false;
	}
	else if (!isActive) {
		isAttack = true;
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
		velocity += Vector3(*playerPos- pos).Normalize();
		if (returnTimer-- <= 0) {
			state = STAY;
		}
		break;
	default:
		break;
	}

	//�ړ��ʂ�K��
	Move();

	//�}�b�v�`�b�v�p�̔�����ړ�
	rect2d.Top = -(int)((pos.z + scale.x * 100.0f));
	rect2d.Bottom = -(int)((pos.z - scale.x * 100.0f));
	rect2d.Right = (int)((pos.x + scale.x * 100.0f));
	rect2d.Left = (int)((pos.x - scale.x * 100.0f));


}

void Debris::VelocityReset()
{
	//��C��R
	airResistance = velocity * 0.01f;
	velocity -= airResistance;
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

void Debris::OnCollision(const CollisionInfo &info)
{
	switch (info.object->Tag)
	{
	case ENEMY:
		if (isAttack) {

		}
		else {
			Damage(1.0f);
		}
		break;
	case PLAYER:
		if (info.myName == "hitCollider" &&
			info.collider->GetCollisionName() == "absorptionCollider" &&
			!isFirstAttack) {
			velocity += Vector3(*playerPos - pos).Normalize() * 5.0f;
		}
		//�����蔻��p�̃R���C�_�[�ƃv���C���[�̓����蔻��R���C�_�[���������Ă�����폜
		if (info.myName == "hitCollider" &&
			info.collider->GetCollisionName() == "hitCollider" &&
			!isFirstAttack) {
			isAlive = false;
		}
		break;
	}

}

void Debris::Bounse(
	const Vector3 &hitPos,		//�Փˈʒu
	const Vector3 &normal	//�Փ˂������Ƃ̌����x�N�g��
)
{
	//pos = hitPos + normal * collider.realSphere.radius;
	velocity = CalcReflectVector(velocity, normal);
	//���˕Ԃ肪���ȉ��Ȃ�o�E���h���~
	if (velocity.Length() <= 10.0f) {
		velocity = 0;
	}

}

void Debris::ReturnStart()
{
	//�X�e�[�g�����^�[����
	state = RETURN;
	velocity *= Vector3(pos - *playerPos).Normalize();
	returnTimer = 120;
}

void Debris::SuckedPlayer(const Vector3 &playerPos,const float &suckedRadius)
{
	//�ړ��J�n
	velocity += Vector3(playerPos- pos).Normalize() * 3.0f;
}

void Debris::HitWall(const XMVECTOR &hitPos, const Vector3 &normal)
{
	Vector3 HitPos = hitPos;
	pos = HitPos + normal * (rect2d.Bottom - rect2d.Top);
	velocity = CalcReflectVector(velocity, normal);

	//�}�b�v�`�b�v�p�̔�����ړ�
	rect2d.Top = -(int)((pos.z + scale.x * 100.0f));
	rect2d.Bottom = -(int)((pos.z - scale.x * 100.0f));
	rect2d.Right = (int)((pos.x + scale.x * 100.0f));
	rect2d.Left = (int)((pos.x - scale.x * 100.0f));
}


void Debris::Damage(float damage)
{
	size -= damage;
	//Size��0�ȉ��ɂȂ����玀�S��Ԃֈȍ~
	if (size < 0) {
		isAlive = false;
	}
	else {
	}
}
