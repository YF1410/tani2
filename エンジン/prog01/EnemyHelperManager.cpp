#include "EnemyHelperManager.h"

std::vector<EnemyHelper *> EnemyHelperManager::enemyHelpers;


EnemyHelperManager *EnemyHelperManager::GetIns()
{
	static EnemyHelperManager instans;
	return &instans;
}

void EnemyHelperManager::Update()
{

	//íœ
	for (int i = enemyHelpers.size() - 1; i >= 0; i--) {
		if (!enemyHelpers[i]->isAlive) {
			delete enemyHelpers[i];
			enemyHelpers.erase(enemyHelpers.begin() + i);
		}
	}
	//XV
	for (int i = 0; i < enemyHelpers.size(); i++) {
		enemyHelpers[i]->Update();
	}
}

void EnemyHelperManager::Draw() const
{
	for (int i = 0; i < enemyHelpers.size(); i++) {
		enemyHelpers[i]->Adaptation();
		enemyHelpers[i]->Draw();
	}
}
