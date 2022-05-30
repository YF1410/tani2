#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "PlayerObject.h"
#include "EnemyManager.h"
#include "FbxObject3d.h"

class UserInterface
{
public:
	UserInterface(int *nowWave, PlayerObject *player, EnemyManager *enemys, int *counter);
	~UserInterface();
	void Initialize();
	void Update();
	void Draw() const;

	//�`���[�g���A��
	static int tutorialNum;
	bool checkFlag[4];
private:
	//�t���[��
	std::unique_ptr<Sprite> frame;
	//�X�e�[�W�e�L�X�g
	std::unique_ptr<Sprite> stageText;
	//�X�e�[�W�ԍ�
	std::unique_ptr<Sprite> stageNum[10];
	//�E�F�[�u�e�L�X�g
	std::unique_ptr<Sprite> waveText;
	//HP�t���[��
	std::unique_ptr<Sprite> hpFrame;
	//HP�Q�[�W
	std::unique_ptr<Sprite> hpGauge;
	std::unique_ptr<Sprite> hpGaugeDamage;


	//�E�F�[�u�ύX���o
	std::unique_ptr<Sprite> moveWave;
	
	//���E�̃Q�[�W
	std::unique_ptr<Sprite> boostFrame;		//�u�[�X�g�t���[��
	std::unique_ptr<Sprite> boostGauge;		//�u�[�X�g�Q�[�W
	std::unique_ptr<Sprite> kaisyuFrame;	//����t���[��
	std::unique_ptr<Sprite> kaisyuGauge;	//����Q�[�W

	//�{�^��
	std::unique_ptr<Sprite> AButton;		//����Q�[�W
	std::unique_ptr<Sprite> BButton;		//����Q�[�W
	std::unique_ptr<Sprite> RBButton;	//����Q�[�W


	//�E�F�[�u��
	std::unique_ptr<Sprite> waveNum[10];
	std::unique_ptr<Sprite> waveMaxNum[10];
	std::unique_ptr<Sprite> moveWaveNum[10];
	std::unique_ptr<Sprite> moveWaveMaxNum[10];

	const int maxRecoverColorTimer = 30;
	int recoverColorTimer;

	std::unique_ptr<Sprite> text[14];

	std::vector<std::unique_ptr<Sprite>> tutorialImag;

	//�^�X�N
	std::unique_ptr<Sprite> taskList;
	std::unique_ptr<Sprite> check[4];
	int oldEnemySize;

	//���݂̃E�F�[�u��
	static int *nowWave;
	static int oldWave;
	static float moveWaveTimer;
	static float movePosX;
	static bool isChangeWave;

	//�v���C���[�̃f�[�^
	PlayerObject *player;
	EnemyManager *enemys;

	//�J�E���^�[
	int *counter;

	bool isMinimapDraw;
	//�~�j�}�b�v
	std::unique_ptr<Sprite> mapImag[MapChip::MAP_NAME::MAX];
	std::unique_ptr<Sprite> xButton;
	std::unique_ptr<Sprite> nextYButton;
	std::unique_ptr<Sprite> endYButton;
	std::unique_ptr<Sprite> playerPos;
	std::unique_ptr<Sprite> enemysPos[50];
	int outCount;
	std::unique_ptr<Sprite> outEnemys[50];


	//Hpbar�̒���
	float hpGaugeLength;
	float hpGaugeDamageLength;
	float oldHp;
	float saveHp;
	int scaleResetCount;
	const int scaleResetTime = 60;
	bool isDamageReset;
	float resetEase = 0.0f;
};

