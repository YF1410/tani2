#include "Debris.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "SlimeMath.h"

std::vector<Debris *> Debris::debris;

Debris::Debris(Vector3 startPos, Vector3 startVec, float size) :
	GameObjCommon(
		ModelManager::SLIME_BREAK,
		GameObjCommon::DEBRIS,
		true,
		startPos
	),
	size(size),
	isAlive(true),
	isAttack(true)
{
	//�T�C�Y����X�P�[���փR���o�[�g
	scale = ConvertSizeToScale(size);
	//���������x�Z�b�g
	velocity = startVec;
	UpdateCollider();
}

void Debris::Update()
{
	VelocityReset();

	//�U���I��
	if (velocity.Length() <= 10.0f && isAttack) {
		isAttack = false;
	}
	
	
	//�����蔻��X�V
	UpdateCollider();
}

void Debris::VelocityReset()
{
	//��C��R
	airResistance = velocity * 0.01f;
	velocity -= airResistance;
	//�d��
	velocity.y -= gravityPow;

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

void Debris::UpdateCollider()
{
	Move();
	//������n�ɋ߂�����
	collider.realSphere.center = pos;
	collider.realSphere.radius = scale.x * 150.0f;
	//�U���p����
	collider.attackSphere.center = pos;
	collider.attackSphere.radius = scale.x * 180.0f;

	//�z���p
	collider.hitSphere.center = pos;
	collider.hitSphere.radius = scale.x * 150.0f;
}

void Debris::Bounse(
	const Vector3 &hitPos,		//�Փˈʒu
	const Vector3 &normal	//�Փ˂������Ƃ̌����x�N�g��
)
{
	pos = hitPos + normal * collider.realSphere.radius;
	velocity = CalcReflectVector(velocity, normal);
	//���˕Ԃ肪���ȉ��Ȃ�o�E���h���~
	if (velocity.Length() <= 10.0f) {
		velocity = 0;
	}

	UpdateCollider();
}

void Debris::SuckedPlayer(const Vector3 &playerPos,const float &suckedRadius)
{
	//�ړ��J�n
	velocity += Vector3(playerPos- pos).Normalize() * 3.0f;
}

float Debris::AbsorbedToPlayer()
{
	isAlive = false;
	return size;
}

void Debris::Damage(float damage)
{
	size -= damage;
	//HP��0�ȉ��ɂȂ����玀�S��Ԃֈȍ~
	if (size < 0) {
		isAlive = false;
	}
	else {
	}
}
