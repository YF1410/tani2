#include "EnemySpawnManager.h"
#include "GameScene.h"

EnemySpawnManager *EnemySpawnManager::GetIns()
{
	static EnemySpawnManager instance;
	return &instance;
}

void EnemySpawnManager::Update()
{
	//\•b‚²‚Æ‚É“G’Ç‰Á
	if (GameScene::counter % 360 == 0) {
		float Rad = XMConvertToRadians(rand() % 360);
		Vector3 spawnPos = {
			static_cast<float>(cos(Rad)),
			0,
			static_cast<float>(sin(Rad))
		};
		spawnPos = spawnPos * 1500 + player->GetPos();
		SpawnEnemy(spawnPos);
	}


	//íœ
	for (int i = Enemy::enemys.size() - 1; i >= 0; i--) {
		if (!Enemy::enemys[i]->Enemy::enemys[i]->isAlive) {
			delete Enemy::enemys[i];
			Enemy::enemys.erase(Enemy::enemys.begin() + i);
		}
	}
	//XV
	for (int i = 0; i < Enemy::enemys.size(); i++) {
		Enemy::enemys[i]->Update();
	}

}

void EnemySpawnManager::SpawnEnemy(Vector3 spawnPos)
{
	Enemy::enemys.push_back(new Enemy(spawnPos,player));
}
