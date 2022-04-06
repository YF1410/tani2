#pragma once
#include "Enemy.h"
#include "Vector3.h"
#include "PlayerObject.h"
#include <vector>

class EnemySpawnManager
{
private:
	EnemySpawnManager() = default;
	~EnemySpawnManager() = default;
	EnemySpawnManager(const EnemySpawnManager & mapChip) = delete;
	EnemySpawnManager &operator=(const EnemySpawnManager & mapChip) = delete;
public:
	static EnemySpawnManager *GetIns();

	void SetPlayer(PlayerObject *player) { this->player = player; };
	void Update();

	void Adaptation();
	void Draw();
	void SpawnEnemy(Vector3 spawnPos);

public:
	PlayerObject *player;
	std::vector<Enemy *> enemys;

};

