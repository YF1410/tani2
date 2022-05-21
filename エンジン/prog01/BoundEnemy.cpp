#include "BoundEnemy.h"
#include "Easing.h"
#include "yMath.h"

BoundEnemy::BoundEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos,targetPos){
	objectData.get()->SetModel(ModelManager::GetIns()->GetModel(ModelManager::ENEMY_ROBO_3));
	scale = { 1.5f,1.5f,1.5f };
	startScale = scale.x;
}

void BoundEnemy::Update() {
	//���|�W�V����
	oldPos = pos;
	//�ړ��ʏ�����
	VelocityReset(0.95f);
	if (!isInvincible && velocity.Length() > maxMoveSpeed) {
		//�ō����x�𒴂��Ă����琧������
		velocity = velocity.Normal() * maxMoveSpeed;
	}
	if (isInvincible && velocity.Length() > maxMoveSpeed * 10.0f) {
		velocity = velocity.Normal() * maxMoveSpeed * 4.0f;

	}
	penalty = { 0,0,0 };

	//�ʏ펞�����i������������΃t���O�ŊǗ�����j
	if (isHitStop) {
		velocity = 0;
	}
	Move();


	//���ʏ���
	//���G���ԃ^�C�}�[���Ǘ�
	if (isInvincible) {
		InvincibleTimer++;

		//�_���[�W�󂯂����̂�����Ƃł����Ȃ���������
		/*if (InvincibleTimer <= 10) {
			scale = Ease(In, Back, (float)(InvincibleTimer / 10.0f), 1.0f, 3.0f) * defScale;
		}
		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP > 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), 3.0f, 1.0f) * defScale;
		}

		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP <= 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), 3.0f, 0.0f) * defScale;
		}*/
		//�����܂�
		
		//�^�C�}�[��30�ɂȂ����疳�G������
		if (InvincibleTimer >= 30) {
			isInvincible = false;
			//HP��0�ȉ��ɂȂ����玀�S��Ԃֈȍ~
			if (HP <= 0) {
				isAlive = false;
				//���̊m���ŃA�C�e���h���b�v
				if (rand() % 101 <= 30) {
					Debris::debris.push_back(new Debris(pos, { 0,0,0 }, 5));
				}
			}
			else {
				scale = 1.5f;
			}
		}
	}

	//�U���C���^�[�o������
	attack.Intervel();
	//�ړ�����������K��
	PosAddVelocity();

	if (isHitStop) {
		hitStopTimer++;
		if (hitStopTimer >= 20) {
			hitStopTimer = 0;
			isHitStop = false;
		}
	}
}

void BoundEnemy::HitPlayer(const CollisionInfo &info)
{
	PlayerObject *player;
	player = dynamic_cast<PlayerObject *>(info.object);
	//�ʒu�C��
	penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
	penalty.y = 0;
	//�v���C���[���U����ԂȂ�
	if (player->attack.is) {
		//�_���[�W���󂯂�
		Damage(player->attackPow);

		Vector3 nextPos = info.inter + Vector3(pos - info.object->pos).Normal() * (broadSphereCollider->GetRadius() /*+ player->broadSphereCollider->GetRadius()*/);
		nextPos.y = 0;
		velocity = CalcReflectVector(velocity, Vector3(pos - player->pos).Normal())* 100.0f;
		velocity.y = 0;
		pos = nextPos;
	}
	else {
		//�ʒu�C��
		penalty += Vector3(info.reject).Normal() * Vector3(info.reject).Length() * 0.4f;
		penalty.y = 0;
	}
}
