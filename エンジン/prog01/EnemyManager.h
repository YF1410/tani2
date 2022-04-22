#pragma once
#include "Enemy.h"
#include "Vector3.h"
#include "PlayerObject.h"
#include "Time.h"
#include <vector>

#include "MimicEnemy.h"
#include "CushionEnemy.h"
#include "AvoidanceEnemy.h"
#include "RandomMoveEnemy.h"
#include "EscapeEnemy.h"
#include "BoundEnemy.h"
#include "DefenseEnemy.h"
#include "KiteEnemy.h"
#include "SuctionEnemy.h"
#include "GetawayEnemy.h"
#include "RouteMoveEnemy.h"
#include "MapChip.h"

class EnemyManager
{
private:
	EnemyManager() = default;
	~EnemyManager() = default;
	EnemyManager(const EnemyManager & mapChip) = delete;
	EnemyManager &operator=(const EnemyManager & mapChip) = delete;
public:
	static EnemyManager *GetIns();

	void Initialize(PlayerObject *player);
	void Update();
	void FinalUpdate();
	void Adaptation();
	void Draw();

	enum ENEMY_TYPE
	{
		HOMING,
		CUSHION,
		BOUNCE,
		MIMIC,
		AVOIDANCE,
		RANDOM_MOVE,
		ESCAPE,
		DEFENSE,
		KITE,
		SUCTION,
		GETAWAY,
		ROUTEMOVE,
		MAX
	};

public:
	struct SPAWN_DATA {
		ENEMY_TYPE type;
		int time;
		Vector3 pos;
		int num;

		SPAWN_DATA(ENEMY_TYPE type,int time,Vector3 pos,int num) {
			this->type = type;	//�G�l�~�[�̎��
			this->time = time;	//�E�F�[�u�J�n����̐����^�C�~���O
			this->pos = pos;	//�v���C���[���猩���X�|�[���ʒu
			this->num = num;	//�X�|�[����
		}
	};

private:
	//�X�|�[���f�[�^
	static const int MAX_WAVE = 2;
	std::vector<SPAWN_DATA*> spawnData[MAX_WAVE + 1];
	//�v���C���[�|�C���^
	PlayerObject *player;

	//�X�|�[���ʒu
	const Vector3 spawnPos[4] =
	{
		Vector3(
			6 * MapChip::GetInstance()->chipSize,
			0,
			-6 * MapChip::GetInstance()->chipSize
		),

		Vector3(
			6 * MapChip::GetInstance()->chipSize,
			0,
			(-63 + 6) *MapChip::GetInstance()->chipSize
		),

		Vector3(
			(63 - 6) *MapChip::GetInstance()->chipSize,
			0,
			-6 * MapChip::GetInstance()->chipSize
		),

		Vector3(
			(63 -6) * MapChip::GetInstance()->chipSize,
			0,
			(-63 + 6) *MapChip::GetInstance()->chipSize
		)
	};

public:
	//�G�S�̂̃f�[�^
	static std::vector<Enemy *> enemys;

	//���݂̃E�F�[�u��
	int nowWave;
	//�E�F�[�u���Ƃ̕b��
	int waveStartTime = 0;

	bool endFlag;
};
