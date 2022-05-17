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
#include "ShotBoss.h"

#include "MapChip.h"


class EnemyManager
{
public:
	EnemyManager(PlayerObject *player);
	~EnemyManager();
	void Initialize();
	void Update();
	void FinalUpdate();
	void Adaptation();
	void Draw();
	void Finalize();

	bool isEndFlag() { return endFlag; }

	//スポーンデータ読み込み
	void CsvLoad(MapChip::MAP_NAME mapName, std::string fName);

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
		Boss,
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

	int MAX_WAVE[MapChip::MAP_NAME::MAX];
	int waveEnemyNum[10];
private:
	//スポーンデータ
	std::vector<SPAWN_DATA*> spawnData[MapChip::MAP_NAME::MAX][10];
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
	static std::vector<Enemy*> enemys[MapChip::MAP_NAME::MAX];

	//現在のウェーブ数
	int nowWave;
	//ウェーブごとの秒数
	int waveStartTime = 0;

	bool endFlag = false;
	ParticleEmitter* defeatParticle1;
	ParticleEmitter* defeatParticle2;
};
