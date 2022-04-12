#include "EnemyManager.h"
#include "GameScene.h"
#include "DebugText.h"

EnemyManager *EnemyManager::GetIns()
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Update()
{
	//�\�b���ƂɓG�ǉ�
	if (GameScene::counter % 30 == 0 &&
		enemys.size() <= 256) {
		float Rad;
		for (int i = 0; i < 2; i++) {
			Rad = XMConvertToRadians(rand() % 360);
			Vector3 normal = {
				static_cast<float>(cos(Rad)),
				0,
				static_cast<float>(sin(Rad))
			};
			Vector3 spawnPos = normal.Normal() * 2000 + player->GetPos();
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


			enemys.push_back(new Enemy(spawnPos, player));
		}
	}


	//�폜
	for (int i = enemys.size() - 1; i >= 0; i--) {
		if (!enemys[i]->isAlive) {
			delete enemys[i];
			enemys.erase(enemys.begin() + i);
		}
	}
	//�X�V
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Update();
	}
	DebugText::GetInstance()->VariablePrint(0, 120, "EnemySize", enemys.size(), 3);

}

void EnemyManager::FinalUpdate()
{
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->FinalUpdate();
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
