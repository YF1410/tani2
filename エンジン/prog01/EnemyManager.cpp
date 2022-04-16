#include "EnemyManager.h"
#include "GameScene.h"
#include "DebugText.h"

EnemyManager *EnemyManager::GetIns()
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize(PlayerObject *player)
{
	this->player = player;
	//ここでエネミーを追加しておく
	spawnData.push_back(new SPAWN_DATA(5,Vector3(1,0,0),5));
	spawnData.push_back(new SPAWN_DATA(5,Vector3(-1,0,0),5));
	spawnData.push_back(new SPAWN_DATA(7,Vector3(1,0,0),5));
	spawnData.push_back(new SPAWN_DATA(7,Vector3(-1,0,0),5));
	spawnData.push_back(new SPAWN_DATA(10,Vector3(1,0,0),5));
	spawnData.push_back(new SPAWN_DATA(10,Vector3(-1,0,0),5));
	
	spawnData.push_back(new SPAWN_DATA(14,Vector3(0,0,1),10));
	spawnData.push_back(new SPAWN_DATA(14,Vector3(0,0,-1),10));
	
	for (int i = 15; i <= 100; i++) {
		spawnData.push_back(new SPAWN_DATA(i, Vector3(rand() % 10 - 5, 0, rand() % 10 - 5),10));
	}
}

void EnemyManager::Update()
{
	if (spawnData.size() != 0) {
		while ((int)GameScene::counter / 60 == spawnData[0]->time) {
			for (int i = 0; i <= spawnData[0]->num; i++) {
				Vector3 spawnPos = spawnData[0]->pos.Normal() * 2500 + Vector3((rand() % 100 - 50),0, (rand() % 100 - 50)) + player->GetPos();
				enemys.push_back(new Enemy(spawnPos, player));
			}

			delete spawnData[0];
			spawnData.erase(spawnData.begin());
			if (spawnData.size() == 0) {
				break;
			}
		}
	}
	//if (GameScene::counter == spawnData.begin()->time) {

		//float Rad;
		//for (int i = 0; i < 2; i++) {
		//	Rad = XMConvertToRadians(rand() % 360);
		//	Vector3 normal = {
		//		static_cast<float>(cos(Rad)),
		//		0,
		//		static_cast<float>(sin(Rad))
		//	};
		//	Vector3 spawnPos = normal.Normal() * 2000 + player->GetPos();
		//	//マップ外に生成しない
		//	if (spawnPos.x < 400) {
		//		spawnPos.x = 400;
		//	}
		//	if (MapChip::GetInstance()->GetMapData().wide * 200 - 400 < spawnPos.x) {
		//		spawnPos.x = MapChip::GetInstance()->GetMapData().wide * 200 - 400;
		//	}
		//	if (-(MapChip::GetInstance()->GetMapData().high * 200 - 400) > spawnPos.z) {
		//		spawnPos.z = -(MapChip::GetInstance()->GetMapData().high * 200 ) + 400;
		//	}
		//	if (spawnPos.z > -400) {
		//		spawnPos.z = -400;
		//	}
		//	enemys.push_back(new Enemy(spawnPos, player));
		//}
	//}


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
