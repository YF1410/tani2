#include "Debris.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "SlimeMath.h"

std::vector<Debris *> Debris::debris;

Debris::Debris(XMFLOAT3 startPos, Vector3 startVec, float size):
	pos(startPos),
	moveVec(startVec),
	size(size),
	isAttack(true),
	isAlive(true),
	airResistance(0,0,0)
{
	// ������
	debriObj = FbxObject3d::Create(ModelManager::GetIns()->GetModel(SLIME));
	//�T�C�Y����X�P�[���փR���o�[�g
	scale = ConvertSizeToScale(size);
	UpdateCollider();
}

void Debris::Update()
{
	{
		//��C��R
		airResistance = moveVec *0.01f;
		moveVec -= airResistance;
		//�d��
		moveVec.y -= 0.5f;
	}
	//�U���I��
	if (moveVec.Length() <= 10.0f && isAttack) {
		isAttack = false;
	}
	
	
	afterPos = pos + moveVec;
	//�����蔻��X�V
	UpdateCollider();
}

void Debris::Adaptation()
{
	//�`��ʒu����
	pos = afterPos;
	debriObj->SetPosition(pos);
	debriObj->SetScale(scale);
	//�S�ēK��
	debriObj->Update();
}

void Debris::Draw()
{
	debriObj->Draw(DirectXCommon::GetInstance()->GetCommandList());
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

void Debris::StaticReflection()
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
	//�ړ���̈ʒu�\��
	afterPos = pos + moveVec;
	//������n�ɋ߂�����
	collider.realSphere.center = afterPos;
	collider.realSphere.radius = scale * 150.0f;
	//�U���p����
	collider.attackSphere.center = afterPos;
	collider.attackSphere.radius = scale * 180.0f;

	//�z���p
	collider.hitSphere.center = afterPos;
	collider.hitSphere.radius = scale * 150.0f;
}

void Debris::Bounse(
	const Vector3 &hitPos,		//�Փˈʒu
	const Vector3 &normal	//�Փ˂������Ƃ̌����x�N�g��
)
{
	pos = hitPos + normal * collider.realSphere.radius;
	moveVec = CalcReflectVector(moveVec, normal);
	UpdateCollider();
}

void Debris::SuckedPlayer(const Vector3 &playerPos,const float &suckedRadius)
{
	//�ړ��J�n
	moveVec += Vector3(playerPos- pos).Normalize() * 3.0f;
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
