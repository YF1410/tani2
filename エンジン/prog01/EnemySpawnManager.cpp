#include "EnemySpawnManager.h"
#include "GameScene.h"

EnemySpawnManager *EnemySpawnManager::GetIns()
{
	static EnemySpawnManager instance;
	return &instance;
}

void EnemySpawnManager::Update()
{
	//十秒ごとに敵追加
	if (GameScene::counter % 600 == 0 &&
		Enemy::enemys.size() <= 10) {
		for (int i = 0; i < 15; i++) {
			float Rad = XMConvertToRadians(rand() % 360);
			Vector3 spawnPos = {
				static_cast<float>(cos(Rad)),
				0,
				static_cast<float>(sin(Rad))
			};
			spawnPos = spawnPos * 1500 + player->GetPos();
			SpawnEnemy(spawnPos);
		}
	}


	//削除
	for (int i = Enemy::enemys.size() - 1; i >= 0; i--) {
		if (!Enemy::enemys[i]->Enemy::enemys[i]->isAlive) {
			delete Enemy::enemys[i];
			Enemy::enemys.erase(Enemy::enemys.begin() + i);
		}
	}
	//更新
	for (int i = 0; i < Enemy::enemys.size(); i++) {
		Enemy::enemys[i]->Update();
	}

}

void EnemySpawnManager::SpawnEnemy(Vector3 spawnPos)
{
	Enemy::enemys.push_back(new Enemy(spawnPos,player));
}
