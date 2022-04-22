#include "EnemyManager.h"
#include "GameScene.h"
#include "DebugText.h"


std::vector<Enemy *> EnemyManager::enemys;

EnemyManager::EnemyManager(PlayerObject *player)
{
	this->player = player;

}

EnemyManager::~EnemyManager()
{
	for (int i = enemys.size() - 1; i >= 0; i--) {
		delete enemys[i];
		enemys.erase(enemys.begin() + i);
	}
}

void EnemyManager::Initialize()
{
	nowWave = 0;
	waveStartTime = 0;
	//ここでエネミーを追加

	//ウェーブ1
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[0], 5));
	/*
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[1], 5));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[2], 5));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[3], 5));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[0], 5));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[1], 5));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[2], 5));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[3], 5));

	spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[0], 5));
	spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[1], 5));
	spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[2], 5));
	spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[3], 5));
	spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[0], 5));
	spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[1], 5));
	spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[2], 5));
	spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[3], 5));
	
	spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[0], 5));
	spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[1], 5));
	spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[2], 5));
	spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[3], 5));
	spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[0], 5));
	spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[1], 5));
	spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[2], 5));
	spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[3], 5));
	*/
	endFlag = false;

}

void EnemyManager::Update()
{
	//ウェーブ進行
	if (enemys.size() == 0 && spawnData[nowWave].size() == 0) {
		if (nowWave < MAX_WAVE - 1) {
			nowWave++;
			waveStartTime = GameScene::counter;
		}
		else {
			//終了フラグ
			endFlag = true;
		}
	}

	//追加
	if (spawnData[nowWave].size() != 0) {
		//ウェーブ開始から指定の秒数が経過したら
		while (((int)GameScene::counter - waveStartTime) / 60 == spawnData[nowWave][0]->time) {
			//指定された数だけエネミーをスポーンさせる
			for (int i = 0; i < spawnData[nowWave][0]->num; i++) {
				Vector3 spawnPos = spawnData[nowWave][0]->pos + Vector3((rand() % 50 - 25),0, (rand() % 50 - 25));
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

void EnemyManager::Finalize()
{
	for (int i = enemys.size() - 1; i >= 0; i--) {
		delete enemys[i];
		enemys.erase(enemys.begin() + i);
	}
	enemys.clear();
	spawnData->clear();
}
