#include "GetawayEnemy.h"

GetawayEnemy::GetawayEnemy(XMFLOAT3 startPos, PlayerObject* targetPos) :
	Enemy(startPos, targetPos) {
	getingaway = false;
	getingawayRange = 1500;
}

void GetawayEnemy::Move(){
	if (Vector3(pos - player->pos).Length() <= getingawayRange) {
		getingaway = true;
	}
	else {
		getingaway = false;
	}

	if (getingaway) {
		targetVec = Vector3(player->GetPos() - pos);
		targetVec.y = 0;
		velocity -= targetVec.Normal() * moveSpeed * 0.5f;
	}
}
