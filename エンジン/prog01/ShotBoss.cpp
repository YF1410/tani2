#include "ShotBoss.h"
#include "Easing.h"
#include "Collision.h"
#include "EnemyHelperManager.h"
#include "EnemyBullet.h"

ShotBoss::ShotBoss(XMFLOAT3 startPos, PlayerObject *targetPos,Camera *cam) :
	Enemy(Vector3(Vector3(0,2000,0)+ Vector3(startPos)), targetPos) {
	objectData.get()->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SLIME));
	HP = 1000;
	maxHP = HP;
	hpBer->offset = { 0,100,-1000 };
	defScale = 5.0f;
	scale = defScale;
	isBounce = false;


	this->startPos =startPos;
	this->startPos.y = 120;
	isSpawn = true;
	this->cam = cam;

	//�����蔻��ύX
	float radius = 800;
	broadSphereCollider->SetOffset({ 0,radius,0 });
	broadSphereCollider->SetRadius(radius);
	toMapChipCollider->SetRadius(radius, radius);


	core = std::make_unique<GameObjCommon>(ModelManager::SLIME_CORE, ENEMY, false, Vector3{ 0,0,0 }, Vector3{ 2,2,2 });
	core.get()->scale = 5.0f;
	offset = { 0,50,0 };
	coreBasePos = pos + offset;
	spinVec = { 1,0,0 };


	coreCollider = new SphereCollider("BroadSphere", XMVECTOR{ 0,100,0 }, 100);
	core.get()->SetNarrowCollider(coreCollider);
	core.get()->Adaptation();

	//�U���p�x
	attack.interval = 20;
}

void ShotBoss::Update()
{

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

	//�X�|�[������͉��o������
	if (isSpawn) {
		if (pos.y < startPos.y) {
			pos.y = startPos.y;
			if (startTimer < 60) {
				spinVec.AddRotationY(Ease(In, Quart, float(startTimer / 30.0f), 0, 2.0f));
			}
			else {
				spinVec.AddRotationY(Ease(Out, Quad, float((startTimer - 30) / 30.0f), 2.0f, 0.05f));
			}
			core.get()->pos = coreBasePos + spinVec * spinR * scale.x;
			startTimer++;
			cam->SetTarget(pos);

			if (startTimer >= 120) {
				isSpawn = false;
			}
		}
		else {
			velocity.y -= 0.9f;
			cam->SetEye(Vector3(startPos+ Vector3(0, 100, -2500)));
			cam->SetTarget(pos);
		}
	}
	else{
		//�ʏ펞�����i������������΃t���O�ŊǗ�����j
		Move();
	}
	//���ʏ���
	//���G���ԃ^�C�}�[���Ǘ�
	if (isInvincible) {
		InvincibleTimer++;
		//�����͌�ŃA�j���[�V�����ɕύX����
		if (InvincibleTimer <= 10) {
			scale = Ease(In, Back, (float)(InvincibleTimer / 10.0f), 1.0f, 1.5f) * defScale;
		}
		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP > 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), 1.5f, 1.0f) * defScale;
		}

		if (10 < InvincibleTimer && InvincibleTimer <= 30 && HP <= 0) {
			scale = Ease(In, Back, (float)((InvincibleTimer - 10.0f) / 20.0f), 1.5f, 0.0f) * defScale;
		}
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
				scale = defScale;
			}
		}
	}

	//�U���C���^�[�o������
	attack.Intervel();
	//�ړ�����������K��
	PosAddVelocity();
	coreBasePos = pos + offset;

}

void ShotBoss::Draw() const
{
	core.get()->Draw();
	Enemy::Draw();
}

void ShotBoss::Move()
{
	spinVec.AddRotationY(0.05f);
	core.get()->pos = coreBasePos + spinVec * spinR * scale.x;
	core.get()->Update();
	core.get()->Adaptation();

	//�U���\�Ȃ�
	if (attack.can) {
		attack.Start();
		//�ˌ�
		Vector3 shot = Vector3(core.get()->pos - pos).Normal() * 30.0f;
		shot.y = 0;
		EnemyHelperManager::enemyHelpers.push_back(
			new EnemyBullet(
				Vector3(core.get()->pos.x,0, core.get()->pos.z),
				shot,
				ModelManager::SHOTBOSS)
		);
	}
	attack.Intervel();


}

void ShotBoss::HitPlayer(const CollisionInfo &info)
{
	PlayerObject *player;
	player = dynamic_cast<PlayerObject *>(info.object);

	player->velocity += Vector3(info.reject).Normal() * player->velocity.Length() * -0.5f;
	player->velocity.y = 0.0f;
	//�v���C���[���R�A�ƐڐG������_���[�W����
	if(player->attack.is){
		if (Collision::CheckSphere2Sphere(
			*dynamic_cast<Sphere *>(player->broadSphereCollider),
			*dynamic_cast<Sphere *>(coreCollider))
			) {
			Damage(player->attackPow);
		}
	}
}

void ShotBoss::HitDebri(const CollisionInfo &info)
{
	Debris *debri;
	debri = dynamic_cast<Debris *>(info.object);

	debri->velocity += Vector3(info.reject).Normal() * debri->velocity.Length() * -0.5f;
	debri->velocity.y = 0.0f;
	//�v���C���[���R�A�ƐڐG������_���[�W����
	if (debri->state == Debris::ATTACK||
		debri->state == Debris::RETURN) {
		if (Collision::CheckSphere2Sphere(
			*dynamic_cast<Sphere *>(debri->attackCollider),
			*dynamic_cast<Sphere *>(coreCollider))
			) {
			//�_���[�W
			Damage(debri->velocity.Length());
		}
	}
}
