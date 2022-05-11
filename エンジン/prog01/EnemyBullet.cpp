#include "EnemyBullet.h"
#include "MapChip.h"

EnemyBullet::EnemyBullet(Vector3 startPos, Vector3 startVec, ModelManager::ModelName modelName):
	EnemyHelper(startPos, startVec,targetPos,modelName) {
	toMapChipCollider = new Box2DCollider("toMapChip", { 0,0,0 }, 100, 100);
	SetNarrowCollider(toMapChipCollider);

	broadSphereCollider = new SphereCollider("BroadSphere", XMVECTOR{ 0,100,0 }, 100);
	SetBroadCollider(broadSphereCollider);
}

void EnemyBullet::Update()
{
	oldPos = pos;
	velocity.y = 0;
	pos += velocity;
	GameObjCommon::Update();
	GameObjCommon::Adaptation();
}

void EnemyBullet::LustUpdate()
{

	//マップチップとの当たり判定
	toMapChipCollider->Update();
	Vector3 hitPos = { 0,0,0 };
	Vector3 moveVec = velocity + penalty;
	Vector3 normal = { 0,0,0 };
	//上下
	if (MapChip::GetInstance()->CheckMapChipAreaToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal)) {
		isAlive = false;
	}
	else if (MapChip::GetInstance()->CheckMapChipToBox2d(toMapChipCollider, &moveVec, &hitPos, &normal,&oldPos)) {
		isAlive = false;

	}

}

void EnemyBullet::OnCollision(const CollisionInfo &info)
{
	if (info.object->Tag == PLAYER) {
		isAlive = false;

	}
}
