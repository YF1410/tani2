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

	//チュートリアル
	int tutorialNum;
	bool stopFrag;
private:
	//フレーム
	std::unique_ptr<Sprite> frame;
	//ステージテキスト
	std::unique_ptr<Sprite> stageText;
	//ステージ番号
	std::unique_ptr<Sprite> stageNum[10];
	//ウェーブテキスト
	std::unique_ptr<Sprite> waveText;
	//HPフレーム
	std::unique_ptr<Sprite> hpFrame;
	//HPゲージ
	std::unique_ptr<Sprite> hpGauge;


	//ウェーブ変更演出
	std::unique_ptr<Sprite> moveWave;
	
	//左右のゲージ
	std::unique_ptr<Sprite> boostFrame;		//ブーストフレーム
	std::unique_ptr<Sprite> boostGauge;		//ブーストゲージ
	std::unique_ptr<Sprite> kaisyuFrame;	//回収フレーム
	std::unique_ptr<Sprite> kaisyuGauge;	//回収ゲージ

	//ウェーブ数
	std::unique_ptr<Sprite> waveNum[10];
	std::unique_ptr<Sprite> waveMaxNum[10];
	std::unique_ptr<Sprite> moveWaveNum[10];
	std::unique_ptr<Sprite> moveWaveMaxNum[10];

	const int maxRecoverColorTimer = 30;
	int recoverColorTimer;
	XMFLOAT2 shake;

	std::unique_ptr<Sprite> text[18];

	std::vector<std::unique_ptr<Sprite>> tutorialImag;


	//ストップ用フラグ

	//現在のウェーブ数
	static int *nowWave;
	static int oldWave;
	static float moveWaveTimer;
	static float movePosX;
	static bool isChangeWave;

	//プレイヤーのデータ
	PlayerObject *player;
	EnemyManager *enemys;

	//カウンター
	int *counter;

	bool isMinimapDraw;
	//ミニマップ
	std::unique_ptr<Sprite> mapImag[MapChip::MAP_NAME::MAX];
	std::unique_ptr<Sprite> playerPos;
	std::unique_ptr<Sprite> enemysPos[100];

};

