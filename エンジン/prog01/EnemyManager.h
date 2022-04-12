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

	void SetPlayer(PlayerObject *player) { this->player = player;};
	void Update();
	void FinalUpdate();
	void Adaptation();
	void Draw();

	int GetEnemySize() { return enemys.size(); }

public:
	PlayerObject *player;
	std::vector<Enemy *> enemys;
	bool enemyAddFlag = true;
	int weveCount = 0;
	int enemyCount = 0;
};

