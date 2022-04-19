#pragma once
#include "Enemy.h"
class AvoidanceEnemy :
	public Enemy
{
public:
	AvoidanceEnemy(XMFLOAT3 startPos, PlayerObject *targetPos);
	void Move() override;
private:
	//���p�X�e�[�g
	STATE avoidance;
	//�����J�n���̃v���C���[�ʒu
	Vector3 avoidancePos;
	//���͈�
	float avoidanceRange;
};

