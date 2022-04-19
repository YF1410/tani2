#include "SuctionEnemy.h"

SuctionEnemy::SuctionEnemy(XMFLOAT3 startPos, PlayerObject* targetPos) :
	Enemy(startPos, targetPos) {
	suctioning = false;
	suctioningRange = 2000;
}

void SuctionEnemy::Suction() {

}