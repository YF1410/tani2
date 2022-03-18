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
	isStop(false),
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
	//���S�ɒ�~
	if (moveVec.Length() <= 1.0f) {
		isStop = true;
		moveVec = { 0,0,0 };
	}
	//��C��R����
	else {
		airResistance = moveVec * 0.1f;
		moveVec -= airResistance;
	}
	//�U���I��
	if (moveVec.Length() <= 10.0f) {
		isAttack = false;
	}
	
	
	afterPos = pos + moveVec;
	//�����蔻��X�V
	UpdateCollider();
}

void Debris::Reflection()
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
		debris[i]->Reflection();
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
	//�����ڂ��厖�p
	collider.realSphere.center = afterPos;
	collider.realSphere.radius = scale * 150.0f;
	//�z���p
	collider.absorbedSphere.center = afterPos;
	collider.absorbedSphere.radius = scale * 150.0f;
}

void Debris::Bounse(
	const XMVECTOR &hitPos,		//�Փˈʒu
	const Vector3 &normal	//�Փ˂������Ƃ̌����x�N�g��
)
{
	pos = hitPos + normal * collider.realSphere.radius;
	moveVec = CalcReflectVector(moveVec, normal)*1.5f;
	UpdateCollider();
}

void Debris::SuckedPlayer(const Vector3 &playerPos,const float &suckedRadius)
{
	//�����񂹂��鑬�x
	float suckedPow =  Vector3(playerPos - pos).Length() - suckedRadius;
	if (suckedPow < 0) { return; }
	moveVec += Vector3(pos - playerPos).Normalize() * suckedPow * 0.01f;
}

float Debris::AbsorbedToPlayer()
{
	isAlive = false;
	return size;
}
