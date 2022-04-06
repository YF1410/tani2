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
		enemys.size() <= 10) {
		for (int i = 0; i < 5; i++) {
			float Rad = XMConvertToRadians(rand() % 360);
			Vector3 spawnPos = {
				static_cast<float>(cos(Rad)),
				0,
				static_cast<float>(sin(Rad))
			};
			spawnPos = spawnPos * 1500 + player->GetPos();
			//マップ外に生成しない
			if (spawnPos.x < 400) {
				spawnPos.x = 400;
			}
			if (MapChip::GetInstance()->GetMapData().wide * 200 - 400 < spawnPos.x) {
				spawnPos.x = MapChip::GetInstance()->GetMapData().wide * 200 - 400;
			}
			if (-(MapChip::GetInstance()->GetMapData().high * 200 - 400) > spawnPos.z) {
				spawnPos.z = -(MapChip::GetInstance()->GetMapData().high * 200 ) + 400;
			}
			if (spawnPos.z > -400) {
				spawnPos.z = -400;
			}


			SpawnEnemy(spawnPos);
		}
	}


	//削除
	for (int i = enemys.size() - 1; i >= 0; i--) {
		if (!enemys[i]->isAlive) {
			delete enemys[i];
			enemys.erase(enemys.begin() + i);
		}
	}
	//更新
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Update();
	}

}

void EnemySpawnManager::Adaptation()
{
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Adaptation();
	}
}

void EnemySpawnManager::Draw()
{
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw();
	}
}

void EnemySpawnManager::SpawnEnemy(Vector3 spawnPos)
{
	enemys.push_back(new Enemy(spawnPos,player));
}
