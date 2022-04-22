#include "BoundEnemy.h"
#include "yMath.h"

BoundEnemy::BoundEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos,targetPos){
}

void BoundEnemy::OnCollision(const CollisionInfo &info)
{

	//�_�C�i�~�b�N�L���X�g�p
	PlayerObject *player;
	Enemy *enemy;
	Debris *debri;

	//�Փ˂����R���C�_�[�ŕ���
	switch (info.object->Tag)
	{
	case PLAYER:
		player = dynamic_cast<PlayerObject *>(info.object);
		//�ʒu�C��
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
		penalty.y = 0;

		//�v���C���[���U����ԂȂ�
		if (player->attack.is) {
			//�_���[�W���󂯂�
			Damage(1.0f);
			
			////�Փˎ��x�N�g��
			//Vector3 normal = pos - player->pos;
			//normal.Normalize();
			////����
			//float dot = Vector3(player->pos - pos).VDot(normal);
			////�萔�x�N�g��
			//Vector3 constVec = 2 * dot / 2 * normal;

		}
		break;
	case DEBRIS:
		//�f�u�����U����ԂȂ�_���[�W���󂯂�
		debri = dynamic_cast<Debris *>(info.object);
		if (debri->isAttack) {
			Damage(1.0f);
		}
		break;

	case ENEMY:
		//�G�l�~�[���m�Ȃ牟���Ԃ�����
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.2f;
		break;
	default:
		break;
	}

	penalty.y = 0;
	pos += penalty;
	GameObjCommon::Update();
}

void BoundEnemy::Damage(float damage)
{
	//���G���Ԓ��͏����𒆒f
	if (isInvincible) { return; }
	//�_���[�W���󂯂�
	HP -= damage;
	//���G���Ԃ��Z�b�g����
	isInvincible = true;
	InvincibleTimer = 0;
	velocity.z *= -10;
}
