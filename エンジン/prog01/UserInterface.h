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
	//�E�F�[�u�ύX���o
	std::unique_ptr<Sprite> waveText;
	std::unique_ptr<Sprite> wave[3];
	std::unique_ptr<Sprite> moveWave;
	//�G�l�~�[�̐�
	std::unique_ptr<Sprite> enemyCount;
	//�E�F�[�u��
	std::unique_ptr<Sprite> waveNum[10];
	std::unique_ptr<Sprite> waveMaxNum[10];
	std::unique_ptr<Sprite> moveWaveNum[10];
	std::unique_ptr<Sprite> moveWaveMaxNum[10];

	std::unique_ptr<Sprite> energyNum[4][10];
	std::vector<int> energyCon;

	//HP
	std::unique_ptr<Sprite> playerHp;
	//�̓������
	std::unique_ptr<Sprite> expGauge[4];

	//���
	std::unique_ptr<Sprite> recoverFrame;
	std::unique_ptr<Sprite> recoverGauge;
	const int maxRecoverColorTimer = 30;
	int recoverColorTimer;
	XMFLOAT2 shake;


	//���݂̃E�F�[�u��
	static int *nowWave;
	static int oldWave;
	static float moveWaveTimer;
	static float movePosX;
	static bool isChangeWave;

	//�v���C���[�̃f�[�^
	PlayerObject *player;
	EnemyManager *enemys;
};

