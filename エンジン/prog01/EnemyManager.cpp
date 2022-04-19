#include "EnemyManager.h"
#include "GameScene.h"
#include "DebugText.h"


std::vector<Enemy *> EnemyManager::enemys;

EnemyManager *EnemyManager::GetIns()
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize(PlayerObject *player)
{
	this->player = player;
	//ここでエネミーを追加しておく
	spawnData.push_back(new SPAWN_DATA(AVOIDANCE,5,Vector3(1,0,0),1));
	//spawnData.push_back(new SPAWN_DATA(ESCAPE,5,Vector3(0,0,1),5));
	//spawnData.push_back(new SPAWN_DATA(ESCAPE,5,Vector3(1,0,1),5));
	//spawnData.push_back(new SPAWN_DATA(ESCAPE,5,Vector3(-1,0,0),5));
	//spawnData.push_back(new SPAWN_DATA(ESCAPE,5,Vector3(0,0,-1),5));
	//spawnData.push_back(new SPAWN_DATA(ESCAPE,5,Vector3(-1,0,-1),5));
	
}

void EnemyManager::Update()
{
	//追加
	if (spawnData.size() != 0) {
		while ((int)GameScene::counter / 60 == spawnData[0]->time) {
			for (int i = 0; i < spawnData[0]->num; i++) {
				Vector3 spawnPos = spawnData[0]->pos.Normal() * 2500 + Vector3((rand() % 100 - 50),0, (rand() % 100 - 50)) + player->GetPos();
				switch (spawnData[0]->type)
				{
				case MIMIC:
					enemys.push_back(new MimicEnemy(spawnPos, player));
					break;
				case CUSHION:
					enemys.push_back(new CushionEnemy(spawnPos, player));
					break;
				case BOUNCE:
					enemys.push_back(new BoundEnemy(spawnPos, player));
					break;
				case AVOIDANCE:
					enemys.push_back(new AvoidanceEnemy(spawnPos, player));
					break;
				case RANDOM_MOVE:
					enemys.push_back(new RandomMoveEnemy(spawnPos, player));
					break;
				case ESCAPE:
					enemys.push_back(new EscapeEnemy(spawnPos, player));
					break;
				default:
					enemys.push_back(new Enemy(spawnPos, player));
					break;
				}
				
			}

			delete spawnData[0];
			spawnData.erase(spawnData.begin());
			if (spawnData.size() == 0) {
				break;
			}
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
	DebugText::GetInstance()->VariablePrint(0, 120, "EnemySize", enemys.size(), 3);

}

void EnemyManager::FinalUpdate()
{
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->LustUpdate();
	}
}

void EnemyManager::Adaptation()
{
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Adaptation();
	}
}

void EnemyManager::Draw()
{
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw();
	}
}
