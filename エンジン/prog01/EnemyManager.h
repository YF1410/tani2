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
			this->type = type;	//エネミーの種類
			this->time = time;	//ウェーブ開始からの生成タイミング
			this->pos = pos;	//プレイヤーから見たスポーン位置
			this->num = num;	//スポーン数
		}
	};

private:
	//スポーンデータ
	static const int MAX_WAVE = 2;
	std::vector<SPAWN_DATA*> spawnData[MAX_WAVE + 1];
	//プレイヤーポインタ
	PlayerObject *player;

	//スポーン位置
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
	//敵全体のデータ
	static std::vector<Enemy *> enemys;

	//現在のウェーブ数
	int nowWave;
	//ウェーブごとの秒数
	int waveStartTime = 0;

	bool endFlag;
};
