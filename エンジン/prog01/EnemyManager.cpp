#include "EnemyManager.h"
#include "GameScene.h"
#include "DebugText.h"


std::vector<Enemy *> EnemyManager::enemys[MapChip::MAP_NAME::MAX];

EnemyManager::EnemyManager(PlayerObject *player)
{
	this->player = player;

}

EnemyManager::~EnemyManager()
{
	Finalize();
}

void EnemyManager::Initialize()
{
	nowWave = 0;
	waveStartTime = 0;
	//ここでエネミーを追加

	//ウェーブ1
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[0], 2));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[1], 2));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[2], 2));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[3], 2));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[0], 2));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[1], 2));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[2], 2));
	spawnData[0].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[3], 2));

	for (int i = 0; i < spawnData[0].size(); i++) {
		waveEnemyNum[0] += spawnData[0][i]->num;
	}


	//spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[0], 5));
	//spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[1], 5));
	//spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[2], 5));
	//spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[3], 5));
	//spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[0], 5));
	//spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[1], 5));
	//spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[2], 5));
	//spawnData[1].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[3], 5));

	/*for (int i = 0; i < spawnData[1].size(); i++) {
		waveEnemyNum[1] += spawnData[1][i]->num;
	}*/


	//spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[0], 5));
	//spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[1], 5));
	//spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[2], 5));
	//spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 0, spawnPos[3], 5));
	//spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[0], 5));
	//spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[1], 5));
	//spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[2], 5));
	//spawnData[2].push_back(new SPAWN_DATA(BOUNCE, 1, spawnPos[3], 5));

	/*for (int i = 0; i < spawnData[2].size(); i++) {
		waveEnemyNum[2] += spawnData[2][i]->num;
	}*/


	endFlag = false;

}

void EnemyManager::Update()
{
	//ウェーブ進行
	if (enemys[MapChip::GetInstance()->nowMap].size() == 0 && spawnData[nowWave].size() == 0) {
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
					enemys[MapChip::GetInstance()->nowMap].push_back(new MimicEnemy(spawnPos, player));
					break;
				case CUSHION:
					enemys[MapChip::GetInstance()->nowMap].push_back(new CushionEnemy(spawnPos, player));
					break;
				case BOUNCE:
					enemys[MapChip::GetInstance()->nowMap].push_back(new BoundEnemy(spawnPos, player));
					break;
				case AVOIDANCE:
					enemys[MapChip::GetInstance()->nowMap].push_back(new AvoidanceEnemy(spawnPos, player));
					break;
				case RANDOM_MOVE:
					enemys[MapChip::GetInstance()->nowMap].push_back(new RandomMoveEnemy(spawnPos, player));
					break;
				case ESCAPE:
					enemys[MapChip::GetInstance()->nowMap].push_back(new EscapeEnemy(spawnPos, player));
					break;
				case DEFENSE:
					enemys[MapChip::GetInstance()->nowMap].push_back(new DefenseEnemy(spawnPos, player));
					break;
				case KITE:
					enemys[MapChip::GetInstance()->nowMap].push_back(new KiteEnemy(spawnPos, player));
					break;
				case SUCTION:
					enemys[MapChip::GetInstance()->nowMap].push_back(new SuctionEnemy(spawnPos, player));
					break;
				case GETAWAY:
					enemys[MapChip::GetInstance()->nowMap].push_back(new GetawayEnemy(spawnPos, player));
					break;
				case ROUTEMOVE:
					enemys[MapChip::GetInstance()->nowMap].push_back(new RouteMoveEnemy(spawnPos, player));
					break;
				default:
					enemys[MapChip::GetInstance()->nowMap].push_back(new Enemy(spawnPos, player));
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
	for (int i = enemys[MapChip::GetInstance()->nowMap].size() - 1; i >= 0; i--) {
		if (!enemys[MapChip::GetInstance()->nowMap][i]->isAlive) {
			delete enemys[MapChip::GetInstance()->nowMap][i];
			enemys[MapChip::GetInstance()->nowMap].erase(enemys[MapChip::GetInstance()->nowMap].begin() + i);
		}
	}
	//更新
	for (int i = 0; i < enemys[MapChip::GetInstance()->nowMap].size(); i++) {
		enemys[MapChip::GetInstance()->nowMap][i]->Update();
	}
	//DebugText::GetInstance()->VariablePrint(0, 120, "EnemyCount", enemys.size(), 3);


}

void EnemyManager::FinalUpdate()
{
	for (int i = 0; i < enemys[MapChip::GetInstance()->nowMap].size(); i++) {
		enemys[MapChip::GetInstance()->nowMap][i]->LustUpdate();
	}
}

void EnemyManager::Adaptation()
{
	for (int i = 0; i < enemys[MapChip::GetInstance()->nowMap].size(); i++) {
		enemys[MapChip::GetInstance()->nowMap][i]->Adaptation();
	}
}

void EnemyManager::Draw()
{
	for (int i = 0; i < enemys[MapChip::GetInstance()->nowMap].size(); i++) {
		enemys[MapChip::GetInstance()->nowMap][i]->Draw();
	}
}

void EnemyManager::Finalize()
{
	for (auto& a : spawnData)
	{
		a.clear();
	}
	for (auto& a : enemys[MapChip::GetInstance()->nowMap])
	{
		delete a;
	}
	enemys[MapChip::GetInstance()->nowMap].clear();
}

void EnemyManager::CsvLoad(MapChip::MAP_NAME mapName, std::string fName)
{

	//std::ifstream ifs("" + fName + ".csv");
	////MAP_DATA loadData;
	//std::string line;
	//while (getline(ifs, line))
	//{
	//	std::istringstream stream(line);
	//	std::string field;
	//	std::vector<int> result;
	//	while (std::getline(stream, field, ','))
	//	{
	//		result.push_back(stoi(field));
	//		//x++;
	//	}
	//	for (auto i : result)
	//	{
	//		loadData.mapChip.push_back(i);
	//	}
	//	y++;
	//}
	////ファイル名を保存
	//loadData.mapName = "Resources/EnemySpawnData/" + fName + ".csv";
	////Xの最大値を保存
	//loadData.wide = x / y;
	////Yの最大値を保存
	//loadData.high = y;

	////マップネームに対応した位置に格納
	//mapData[nowMap] = loadData;
}
