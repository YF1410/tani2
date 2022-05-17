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
	int tutorialNum;
	bool stopFrag;
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


	//�E�F�[�u�ύX���o
	std::unique_ptr<Sprite> moveWave;
	
	//���E�̃Q�[�W
	std::unique_ptr<Sprite> boostFrame;		//�u�[�X�g�t���[��
	std::unique_ptr<Sprite> boostGauge;		//�u�[�X�g�Q�[�W
	std::unique_ptr<Sprite> kaisyuFrame;	//����t���[��
	std::unique_ptr<Sprite> kaisyuGauge;	//����Q�[�W

	//�E�F�[�u��
	std::unique_ptr<Sprite> waveNum[10];
	std::unique_ptr<Sprite> waveMaxNum[10];
	std::unique_ptr<Sprite> moveWaveNum[10];
	std::unique_ptr<Sprite> moveWaveMaxNum[10];

	const int maxRecoverColorTimer = 30;
	int recoverColorTimer;
	XMFLOAT2 shake;

	std::unique_ptr<Sprite> text[18];

	std::vector<std::unique_ptr<Sprite>> tutorialImag;


	//�X�g�b�v�p�t���O

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
	std::unique_ptr<Sprite> playerPos;
	std::unique_ptr<Sprite> enemysPos[100];

};

