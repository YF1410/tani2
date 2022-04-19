#include "CushionEnemy.h"
#include "Debris.h"


CushionEnemy::CushionEnemy(XMFLOAT3 startPos, PlayerObject *targetPos) :
	Enemy(startPos, targetPos) {
	moveSpeed = 2.0f;
	//objectData.get()->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SLIME_BREAK));
}

void CushionEnemy::OnCollision(const CollisionInfo &info)
{
	//�_�C�i�~�b�N�L���X�g�p
	Debris *debri;
	PlayerObject *player;
	Enemy *enemy;

	//�Փ˂����R���C�_�[�ŕ���
	switch (info.object->Tag)
	{
	case PLAYER:
		player = dynamic_cast<PlayerObject *>(info.object);
		player->velocity *= 0.1f;
		//�ʒu�C��
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
		penalty.y = 0;

		//�v���C���[���U����ԂȂ�
		if (player->attack.is) {
			//�_���[�W���󂯂�
			Damage(1.0f);

			//�Փˎ��x�N�g��
			Vector3 normal = pos - player->pos;
			normal.Normalize();
			//����
			float dot = Vector3(player->pos - pos).VDot(normal);
			//�萔�x�N�g��
			Vector3 constVec = 2 * dot / 2 * normal;

			velocity = 2 * constVec + velocity;

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

void CushionEnemy::Move()
{
}
