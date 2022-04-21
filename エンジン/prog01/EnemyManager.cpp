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
	nowWave = -1;
	this->player = player;
	//ここでエネミーを追加

	//ウェーブ1
	spawnData[0].push_back(new SPAWN_DATA(AVOIDANCE, 1, Vector3(1, 0, 0), 5));
	spawnData[0].push_back(new SPAWN_DATA(AVOIDANCE, 2, Vector3(-1, 0, 0), 5));
	spawnData[0].push_back(new SPAWN_DATA(AVOIDANCE, 3, Vector3(-1, 0, 1), 5));
	spawnData[0].push_back(new SPAWN_DATA(AVOIDANCE, 3, Vector3(-1, 0, 1), 5));
	
	//ウェーブ2
	spawnData[1].push_back(new SPAWN_DATA(AVOIDANCE, 1, Vector3(1, 0, 0), 5));
	spawnData[1].push_back(new SPAWN_DATA(AVOIDANCE, 2, Vector3(1, 0, 0), 5));
	//ウェーブ3
	spawnData[2].push_back(new SPAWN_DATA(AVOIDANCE, 1, Vector3(1, 0, 0), 5));
	spawnData[2].push_back(new SPAWN_DATA(AVOIDANCE, 2, Vector3(1, 0, 0), 5));

}

void EnemyManager::Update()
{
	//ウェーブ進行
	if (GameScene::counter % CHANGE_WAVE_TIME == 0) {
		if (nowWave < 2) {
			nowWave++;
		}
	}

	//追加
	if (spawnData[nowWave].size() != 0) {
		//ウェーブ開始から指定の秒数が経過したら
		while (((int)GameScene::counter - nowWave * CHANGE_WAVE_TIME) / 60 == spawnData[nowWave][0]->time) {
			//指定された数だけエネミーをスポーンさせる
			for (int i = 0; i < spawnData[nowWave][0]->num; i++) {
				Vector3 spawnPos = spawnData[nowWave][0]->pos.Normal() * 2500 + Vector3((rand() % 100 - 50),0, (rand() % 100 - 50)) + player->GetPos();
				switch (spawnData[nowWave][0]->type)
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
				case DEFENSE:
					enemys.push_back(new DefenseEnemy(spawnPos, player));
					break;
				case KITE:
					enemys.push_back(new KiteEnemy(spawnPos, player));
					break;
				case SUCTION:
					enemys.push_back(new SuctionEnemy(spawnPos, player));
					break;
				case GETAWAY:
					enemys.push_back(new GetawayEnemy(spawnPos, player));
					break;
				case ROUTEMOVE:
					enemys.push_back(new RouteMoveEnemy(spawnPos, player));
					break;
				default:
					enemys.push_back(new Enemy(spawnPos, player));
					break;
				}
				
			}

			delete spawnData[nowWave][0];
			spawnData[nowWave].erase(spawnData[nowWave].begin());
			if (spawnData[nowWave].size() == 0) {
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
	DebugText::GetInstance()->VariablePrint(0, 120, "EnemyCount", enemys.size(), 3);

	//ウェーブ数
	DebugText::GetInstance()->VariablePrint(0, 200, "weveCount", nowWave, 3);

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
