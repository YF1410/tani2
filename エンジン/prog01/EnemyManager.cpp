#include "EnemyManager.h"
#include "GameScene.h"
#include "DebugText.h"


std::vector<Enemy *> EnemyManager::enemys[MapChip::MAP_NAME::MAX];

EnemyManager::EnemyManager(PlayerObject *player)
{
	this->player = player;
	CsvLoad(MapChip::MAP_NAME::TEST_MAP, "testSpawn");
	CsvLoad(MapChip::MAP_NAME::Stage1, "testSpawn");
	CsvLoad(MapChip::MAP_NAME::Stage2, "testSpawn");
	CsvLoad(MapChip::MAP_NAME::Stage3, "testSpawn");
	CsvLoad(MapChip::MAP_NAME::Stage4, "testSpawn");
	CsvLoad(MapChip::MAP_NAME::Stage5, "testSpawn");

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

	int spawnDataSize = spawnData[MapChip::GetInstance()->nowMap]->size() / spawnData[MapChip::GetInstance()->nowMap][0].size();
	for (int size = 0; size < spawnDataSize; size++) {
		for (int i = 0; i < spawnData[MapChip::GetInstance()->nowMap][size].size(); i++) {
			waveEnemyNum[size] += spawnData[MapChip::GetInstance()->nowMap][size][i]->num;
		}
	}

	defeatParticle1 = defeatParticle1->Create(L"defeat1");
	defeatParticle2 = defeatParticle2->Create(L"defeat2");
	defeatParticle1->SetStartScale(100.0f);
	defeatParticle1->SetEndScale(700.0f);
	defeatParticle1->SetStartColor({1,1,1,1});
	defeatParticle1->SetEndColor({ 1,1,1,0 });
	defeatParticle2->SetStartScale(300.0f);
	defeatParticle2->SetCenter(500.0f);

	endFlag = false;

	ParticleManager::GetInstance()->SetParticleEmitter(defeatParticle1);
	ParticleManager::GetInstance()->SetParticleEmitter(defeatParticle2);
}

void EnemyManager::Update()
{
	//ウェーブ進行
	if (enemys[MapChip::GetInstance()->nowMap].size() == 0 && spawnData[MapChip::GetInstance()->nowMap][nowWave].size() == 0) {
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
	if (spawnData[MapChip::GetInstance()->nowMap][nowWave].size() != 0) {
		//ウェーブ開始から指定の秒数が経過したら
		while (((int)GameScene::counter - waveStartTime) / 60 == spawnData[MapChip::GetInstance()->nowMap][nowWave][0]->time) {
			//指定された数だけエネミーをスポーンさせる
			for (int i = 0; i < spawnData[MapChip::GetInstance()->nowMap][nowWave][0]->num; i++) {
				Vector3 spawnPos = spawnData[MapChip::GetInstance()->nowMap][nowWave][0]->pos + Vector3((rand() % 50 - 25),0, (rand() % 50 - 25));
				switch (spawnData[MapChip::GetInstance()->nowMap][nowWave][0]->type)
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

			delete spawnData[MapChip::GetInstance()->nowMap][nowWave][0];
			spawnData[MapChip::GetInstance()->nowMap][nowWave].erase(spawnData[MapChip::GetInstance()->nowMap][nowWave].begin());
			if (spawnData[MapChip::GetInstance()->nowMap][nowWave].size() == 0) {
				break;
			}
		}
	}

	//削除
	for (int i = enemys[MapChip::GetInstance()->nowMap].size() - 1; i >= 0; i--) {
		if (!enemys[MapChip::GetInstance()->nowMap][i]->isAlive) {
			defeatParticle1->AddDefeat(1, 40, enemys[MapChip::GetInstance()->nowMap][i]->pos, ParticleEmitter::SHOCKWAVE);
			defeatParticle2->AddDefeat(3, 40, enemys[MapChip::GetInstance()->nowMap][i]->pos, ParticleEmitter::STAR);
			delete enemys[MapChip::GetInstance()->nowMap][i];
			enemys[MapChip::GetInstance()->nowMap].erase(enemys[MapChip::GetInstance()->nowMap].begin() + i);
		}
	}
	//更新
	for (int i = 0; i < enemys[MapChip::GetInstance()->nowMap].size(); i++) {
		enemys[MapChip::GetInstance()->nowMap][i]->Update();
	}
	//DebugText::GetInstance()->VariablePrint(0, 120, "EnemyCount", enemys.size(), 3);
	defeatParticle1->Update();
	defeatParticle2->Update();
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

void EnemyManager::Draw() {
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	for (int i = 0; i < enemys[MapChip::GetInstance()->nowMap].size(); i++) {
		enemys[MapChip::GetInstance()->nowMap][i]->Draw();
		defeatParticle1->Draw(cmdList);
		defeatParticle2->Draw(cmdList);
	}
}

void EnemyManager::Finalize()
{
	for (auto& a : spawnData)
	{
		a[MapChip::GetInstance()->nowMap].clear();
	}
	for (auto& a : enemys[MapChip::GetInstance()->nowMap])
	{
		delete a;
	}
	enemys[MapChip::GetInstance()->nowMap].clear();
}

void EnemyManager::CsvLoad(MapChip::MAP_NAME mapName, std::string fName)
{

	std::ifstream ifs("Resources/EnemySpawnData/" + fName + ".csv");
	//MAP_DATA loadData;
	std::string line;
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string field;
		std::vector<int> result;
		while (std::getline(stream, field, ','))
		{
			result.push_back(stoi(field));
		}
		spawnData[mapName][result[0]].push_back(new SPAWN_DATA((ENEMY_TYPE)result[1], result[2], Vector3(result[3],0, result[4]), result[5]));

		/*for (auto i : result)
		{
			loadData.mapChip.push_back(i);
		}*/
	}
	////ファイル名を保存
	//loadData.mapName = "Resources/EnemySpawnData/" + fName + ".csv";
	////Xの最大値を保存
	//loadData.wide = x / y;
	////Yの最大値を保存
	//loadData.high = y;

	////マップネームに対応した位置に格納
	//mapData[nowMap] = loadData;
}
