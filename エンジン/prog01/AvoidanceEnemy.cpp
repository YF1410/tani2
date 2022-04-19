#include "AvoidanceEnemy.h"

AvoidanceEnemy::AvoidanceEnemy(XMFLOAT3 startPos, PlayerObject *targetPos):
	Enemy(startPos, targetPos){
	avoidance = {
		true,
		false,
		90,
		0
	};
	avoidanceRange = 500.0f;
}

void AvoidanceEnemy::Move()
{
	if (player->attack.is && avoidance.can &&
		Vector3(pos - player->pos).Length() <= avoidanceRange) {
		avoidance.Start();
		avoidancePos = player->pos;
	}

	if (player->attack.is && avoidance.is) {
		if (player->velocity.Normal().VDot(Vector3(pos - avoidancePos).Normal()) < 0) {
			velocity += Vector3(-player->velocity.z, 0, player->velocity.x).Normal() * 60.0f;
		}
		else {
			velocity += Vector3(player->velocity.z, 0, -player->velocity.x).Normal() * 60.0f;
		}
	}
	else{
		avoidance.is = false;
		targetVec = Vector3(player->GetPos() - pos);
		targetVec.y = 0;
		velocity += targetVec.Normal() * moveSpeed;
	}
	avoidance.Intervel();

}
