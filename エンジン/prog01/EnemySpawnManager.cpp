#include "EnemySpawnManager.h"
#include "GameScene.h"

EnemySpawnManager *EnemySpawnManager::GetIns()
{
	static EnemySpawnManager instance;
	return &instance;
}

void EnemySpawnManager::Update()
{
	//�\�b���ƂɓG�ǉ�
	if (/*GameScene::counter % 600 == 0 &&
		Enemy::enemys.size() <= 10*/false) {
		for (int i = 0; i < 5; i++) {
			float Rad = XMConvertToRadians(rand() % 360);
			Vector3 spawnPos = {
				static_cast<float>(cos(Rad)),
				0,
				static_cast<float>(sin(Rad))
			};
			spawnPos = spawnPos * 1500 + player->GetPos();
			//�}�b�v�O�ɐ������Ȃ�
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


	//�폜
	for (int i = Enemy::enemys.size() - 1; i >= 0; i--) {
		if (!Enemy::enemys[i]->Enemy::enemys[i]->isAlive) {
			delete Enemy::enemys[i];
			Enemy::enemys.erase(Enemy::enemys.begin() + i);
		}
	}
	//�X�V
	for (int i = 0; i < Enemy::enemys.size(); i++) {
		Enemy::enemys[i]->Update();
	}

}

void EnemySpawnManager::SpawnEnemy(Vector3 spawnPos)
{
	Enemy::enemys.push_back(new Enemy(spawnPos,player));
}
