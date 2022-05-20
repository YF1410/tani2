#include "EnemyManager.h"
#include "GameScene.h"
#include "DebugText.h"

std::vector<Enemy *> EnemyManager::enemys[MapChip::MAP_NAME::MAX];

EnemyManager::EnemyManager(PlayerObject *player)
{
	this->player = player;
	CsvLoad(MapChip::MAP_NAME::TUTORIAL, "testSpawn");
	CsvLoad(MapChip::MAP_NAME::Stage1, "01");
	CsvLoad(MapChip::MAP_NAME::Stage2, "02");
	CsvLoad(MapChip::MAP_NAME::Stage3, "03");
	CsvLoad(MapChip::MAP_NAME::Stage4, "04");
	CsvLoad(MapChip::MAP_NAME::Stage5, "05");

}

EnemyManager::~EnemyManager()
{
	Finalize();
}

void EnemyManager::Initialize()
{
	nowWave = 0;
	waveStartTime = 0;
	//�����ŃG�l�~�[��ǉ�

	//int spawnDataSize = spawnData[MapChip::GetInstance()->nowMap]->size() / spawnData[MapChip::GetInstance()->nowMap][0].size();
	for (int size = 0; size <= spawnData[MapChip::GetInstance()->nowMap]->size(); size++) {
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
	//�E�F�[�u�i�s
	if (enemys[MapChip::GetInstance()->nowMap].size() == 0 && spawnData[MapChip::GetInstance()->nowMap][nowWave].size() == 0) {
		if (nowWave < MAX_WAVE[MapChip::GetInstance()->nowMap]) {
			nowWave++;
			waveStartTime = GameScene::counter;
		}
		else {
			//�I���t���O
			endFlag = true;
		}
	}

	//�ǉ�
	if (spawnData[MapChip::GetInstance()->nowMap][nowWave].size() != 0) {
		//�E�F�[�u�J�n����w��̕b�����o�߂�����
		while (((int)GameScene::counter - waveStartTime) / 60 == spawnData[MapChip::GetInstance()->nowMap][nowWave][0]->time) {
			//�w�肳�ꂽ�������G�l�~�[���X�|�[��������
			for (int i = 0; i < spawnData[MapChip::GetInstance()->nowMap][nowWave][0]->num; i++) {
				//�d�Ȃ�h�~
				Vector3 spawnPos = spawnData[MapChip::GetInstance()->nowMap][nowWave][0]->pos + 
					Vector3((rand() % 30 - 15),0, (rand() % 30 - 15));
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
				case Boss:
					enemys[MapChip::GetInstance()->nowMap].push_back(new ShotBoss(spawnPos, player));
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

	//�폜
	for (int i = enemys[MapChip::GetInstance()->nowMap].size() - 1; i >= 0; i--) {
		if (!enemys[MapChip::GetInstance()->nowMap][i]->isAlive) {
			defeatParticle1->AddDefeat(1, 40, enemys[MapChip::GetInstance()->nowMap][i]->pos, ParticleEmitter::SHOCKWAVE);
			defeatParticle2->AddDefeat(3, 40, enemys[MapChip::GetInstance()->nowMap][i]->pos, ParticleEmitter::STAR);
			delete enemys[MapChip::GetInstance()->nowMap][i];
			enemys[MapChip::GetInstance()->nowMap].erase(enemys[MapChip::GetInstance()->nowMap].begin() + i);
		}
	}
	//�X�V
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
	int waveNum = -1;
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string field;
		std::vector<int> result;
		while (std::getline(stream, field, ','))
		{
			result.push_back(stoi(field));
		}
		spawnData[mapName][result[0]].push_back(new SPAWN_DATA((ENEMY_TYPE)result[1], result[2], 
			//���W
			Vector3((result[3] - 1) * 200, 0, (result[4] - 1) * -200 ),
			result[5]));

		if (waveNum != result[0]) {
			waveNum = result[0];
			MAX_WAVE[mapName]++;
		}
		/*for (auto i : result)
		{
			loadData.mapChip.push_back(i);
		}*/
	}
	////�t�@�C������ۑ�
	//loadData.mapName = "Resources/EnemySpawnData/" + fName + ".csv";
	////X�̍ő�l��ۑ�
	//loadData.wide = x / y;
	////Y�̍ő�l��ۑ�
	//loadData.high = y;

	////�}�b�v�l�[���ɑΉ������ʒu�Ɋi�[
	//mapData[nowMap] = loadData;
}
