#pragma once
#include "Enemy.h"
#include "Vector3.h"
#include "PlayerObject.h"
#include "Time.h"
#include <vector>

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

public:
	struct SPAWN_DATA {
		int time;
		Vector3 pos;
		int num;
		SPAWN_DATA(int time,Vector3 pos,int num) {
			this->time = time;
			this->pos = pos;
			this->num = num;
		}
	};

private:
	//スポーンデータ
	std::vector<SPAWN_DATA*> spawnData;

public:
	PlayerObject *player;
	std::vector<Enemy *> enemys;

};

