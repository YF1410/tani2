#include "WithdrawalEnemy.h"

WithdrawalEnemy::WithdrawalEnemy(XMFLOAT3 startPos, PlayerObject* targetPos) :
	Enemy(startPos, targetPos) {
	withdrawalPos = {6200,0,10000};
}

void WithdrawalEnemy::Move() {
	targetVec = Vector3(withdrawalPos - pos);
	targetVec.y = 0;
	velocity += targetVec.Normal() * moveSpeed * 1.1f;
}