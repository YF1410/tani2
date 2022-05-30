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
	static int tutorialNum;
	bool checkFlag[4];
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
	std::unique_ptr<Sprite> hpGaugeDamage;


	//ウェーブ変更演出
	std::unique_ptr<Sprite> moveWave;
	
	//左右のゲージ
	std::unique_ptr<Sprite> boostFrame;		//ブーストフレーム
	std::unique_ptr<Sprite> boostGauge;		//ブーストゲージ
	std::unique_ptr<Sprite> kaisyuFrame;	//回収フレーム
	std::unique_ptr<Sprite> kaisyuGauge;	//回収ゲージ

	//ボタン
	std::unique_ptr<Sprite> AButton;		//回収ゲージ
	std::unique_ptr<Sprite> BButton;		//回収ゲージ
	std::unique_ptr<Sprite> RBButton;	//回収ゲージ


	//ウェーブ数
	std::unique_ptr<Sprite> waveNum[10];
	std::unique_ptr<Sprite> waveMaxNum[10];
	std::unique_ptr<Sprite> moveWaveNum[10];
	std::unique_ptr<Sprite> moveWaveMaxNum[10];

	const int maxRecoverColorTimer = 30;
	int recoverColorTimer;

	std::unique_ptr<Sprite> text[14];

	std::vector<std::unique_ptr<Sprite>> tutorialImag;

	//タスク
	std::unique_ptr<Sprite> taskList;
	std::unique_ptr<Sprite> check[4];
	int oldEnemySize;

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
	std::unique_ptr<Sprite> xButton;
	std::unique_ptr<Sprite> nextYButton;
	std::unique_ptr<Sprite> endYButton;
	std::unique_ptr<Sprite> playerPos;
	std::unique_ptr<Sprite> enemysPos[50];
	int outCount;
	std::unique_ptr<Sprite> outEnemys[50];


	//Hpbarの長さ
	float hpGaugeLength;
	float hpGaugeDamageLength;
	float oldHp;
	float saveHp;
	int scaleResetCount;
	const int scaleResetTime = 60;
	bool isDamageReset;
	float resetEase = 0.0f;
};

