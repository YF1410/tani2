#include "EnemyManager.h"
#include "GameScene.h"
#include "DebugText.h"


std::vector<Enemy*> EnemyManager::enemys;

EnemyManager::EnemyManager(PlayerObject* player) {
	this->player = player;

}

EnemyManager::~EnemyManager() {
	Finalize();
}

void EnemyManager::Initialize() {
	nowWave = 0;
	waveStartTime = 0;
	//ここでエネミーを追加

	//ウェーブ1
	spawnData[0].push_back(new SPAWN_DATA(HOMING, 0, spawnPos[0], 5));
	spawnData[0].push_back(new SPAWN_DATA(HOMING, 0, spawnPos[1], 5));
	spawnData[0].push_back(new SPAWN_DATA(HOMING, 0, spawnPos[2], 5));
	spawnData[0].push_back(new SPAWN_DATA(HOMING, 0, spawnPos[3], 5));
	spawnData[0].push_back(new SPAWN_DATA(HOMING, 1, spawnPos[0], 5));
	spawnData[0].push_back(new SPAWN_DATA(HOMING, 1, spawnPos[1], 5));
	spawnData[0].push_back(new SPAWN_DATA(HOMING, 1, spawnPos[2], 5));
	spawnData[0].push_back(new SPAWN_DATA(HOMING, 1, spawnPos[3], 5));

	for (int i = 0; i < spawnData[0].size(); i++) {
		waveEnemyNum[0] += spawnData[0][i]->num;
	}


	spawnData[1].push_back(new SPAWN_DATA(HOMING, 0, spawnPos[0], 5));
	spawnData[1].push_back(new SPAWN_DATA(HOMING, 0, spawnPos[1], 5));
	spawnData[1].push_back(new SPAWN_DATA(AVOIDANCE, 0, spawnPos[2], 5));
	spawnData[1].push_back(new SPAWN_DATA(AVOIDANCE, 0, spawnPos[3], 5));
	spawnData[1].push_back(new SPAWN_DATA(HOMING, 1, spawnPos[0], 5));
	spawnData[1].push_back(new SPAWN_DATA(AVOIDANCE, 1, spawnPos[1], 5));
	spawnData[1].push_back(new SPAWN_DATA(AVOIDANCE, 1, spawnPos[2], 5));
	spawnData[1].push_back(new SPAWN_DATA(AVOIDANCE, 1, spawnPos[3], 5));

	for (int i = 0; i < spawnData[1].size(); i++) {
		waveEnemyNum[1] += spawnData[1][i]->num;
	}


	spawnData[2].push_back(new SPAWN_DATA(AVOIDANCE, 0, spawnPos[0], 5));
	spawnData[2].push_back(new SPAWN_DATA(AVOIDANCE, 0, spawnPos[1], 5));
	spawnData[2].push_back(new SPAWN_DATA(AVOIDANCE, 0, spawnPos[2], 5));
	spawnData[2].push_back(new SPAWN_DATA(AVOIDANCE, 0, spawnPos[3], 5));
	spawnData[2].push_back(new SPAWN_DATA(AVOIDANCE, 1, spawnPos[0], 5));
	spawnData[2].push_back(new SPAWN_DATA(AVOIDANCE, 1, spawnPos[1], 5));
	spawnData[2].push_back(new SPAWN_DATA(AVOIDANCE, 1, spawnPos[2], 5));
	spawnData[2].push_back(new SPAWN_DATA(AVOIDANCE, 1, spawnPos[3], 5));

	for (int i = 0; i < spawnData[2].size(); i++) {
		waveEnemyNum[2] += spawnData[2][i]->num;
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

}

void EnemyManager::Update() {
	//ウェーブ進行
	if (enemys.size() == 0 && spawnData[nowWave].size() == 0) {
		if (nowWave < MAX_WAVE) {
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
				Vector3 spawnPos = spawnData[nowWave][0]->pos + Vector3((rand() % 50 - 25), 0, (rand() % 50 - 25));
				switch (spawnData[nowWave][0]->type) 				{
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
			defeatParticle1->AddDefeat(1, 40, enemys[i]->pos, ParticleEmitter::SHOCKWAVE);
			defeatParticle2->AddDefeat(3, 40, enemys[i]->pos, ParticleEmitter::STAR);
			delete enemys[i];
			enemys.erase(enemys.begin() + i);
		}
	}
	//更新
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Update();
	}
	//DebugText::GetInstance()->VariablePrint(0, 120, "EnemyCount", enemys.size(), 3);
	defeatParticle1->Update();
	defeatParticle2->Update();
}

void EnemyManager::FinalUpdate() {
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->LustUpdate();
	}
}

void EnemyManager::Adaptation() {
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Adaptation();
	}
}

void EnemyManager::Draw() {
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw();
		defeatParticle1->Draw(cmdList);
		defeatParticle2->Draw(cmdList);
	}
}

void EnemyManager::Finalize() {
	for (auto& a : spawnData) 	{
		a.clear();
	}
	for (auto& a : enemys) 	{
		delete a;
	}
	enemys.clear();
}
