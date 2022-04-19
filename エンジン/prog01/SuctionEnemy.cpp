#include "SuctionEnemy.h"
#include "Debris.h"
#include <Collision.h>

SuctionEnemy::SuctionEnemy(XMFLOAT3 startPos, PlayerObject* targetPos) :
	Enemy(startPos, targetPos) {
	suctioningRange = 5000;
	attackRange = 300;
}

void SuctionEnemy::Move() {
	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
		//ƒGƒlƒ~[‚©‚ç”j•Ð‚Ö‚ÌUŒ‚
		float debrisRange = Vector3((Debris::debris[debrisNum]->pos-pos)).Length();
		if (!Debris::debris[debrisNum]->isAttack) {
			if (suctioningRange > debrisRange && Debris::debris[debrisNum]->GetState() != Debris::RETURN) {
				Debris::debris[debrisNum]->SetVelocity(Vector3(pos-Debris::debris[debrisNum]->pos).Normal());
				Debris::debris[debrisNum]->SetState(Debris::SUCTION);
			}
			if (attackRange > debrisRange) {
				Debris::debris[debrisNum]->Damage(0.1f);
			}
		}
	}
}