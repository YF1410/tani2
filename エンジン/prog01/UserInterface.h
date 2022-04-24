#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "PlayerObject.h"
#include "EnemyManager.h"

class UserInterface
{
public:
	UserInterface(int *nowWave, PlayerObject *player, EnemyManager *enemys);
	~UserInterface();
	void Initialize();
	void Update();
	void Draw() const;

private:
	//ウェーブ変更演出
	std::unique_ptr<Sprite> waveText;
	std::unique_ptr<Sprite> wave[3];
	std::unique_ptr<Sprite> moveWave[3];

	//HP
	std::unique_ptr<Sprite> playerHp;
	//体当たり回数
	std::unique_ptr<Sprite> exGauge;
	std::unique_ptr<Sprite> recover;

	//現在のウェーブ数
	static int *nowWave;
	static int oldWave;
	static float moveWaveTimer;
	static float movePosX;
	static bool isChangeWave;

	//プレイヤーのデータ
	PlayerObject *player;
	EnemyManager *enemys;
};

