#include "UserInterface.h"
#include "WinApp.h"
#include "Easing.h"
#include "DebugText.h"

int *UserInterface::nowWave;
int UserInterface::oldWave;
bool UserInterface::isChangeWave;
float UserInterface::moveWaveTimer;
float UserInterface::movePosX;

UserInterface::UserInterface(int *nowWave, PlayerObject *player, EnemyManager *enemys)
{
	this->nowWave = nowWave;
	this->player = player;
	this->enemys = enemys;
}

UserInterface::~UserInterface()
{
}

void UserInterface::Initialize()
{
	waveText = Sprite::Create(5, { 0,250 });

	wave[0] = Sprite::Create(2, {0,0});
	wave[1] = Sprite::Create(3, {0,0});
	wave[2] = Sprite::Create(4, {0,0});

	moveWave = Sprite::Create(5, { 0,WinApp::window_height / 2 }, {1,1,1,1} ,{ 0.5f, 0.5f });
	moveWave.get()->SetSize({ 256*2,128*2 });

	//ウェーブ数
	for (int i = 0; i < 10; i++) {
		waveNum[i] = Sprite::Create(100 + i, { 110,350 }, { 1,1,1,1 }, {0,1.0f});
		waveNum[i].get()->SetSize({80,80});
	}
	for (int i = 0; i < 10; i++) {
		waveMaxNum[i] = Sprite::Create(100 + i, { 180,350 }, { 1,1,1,1 }, { 0,1.0f });
		waveMaxNum[i].get()->SetSize({ 80,80 });
	}

	for (int i = 0; i < 10; i++) {
		moveWaveNum[i] = Sprite::Create(100 + i, { 110,105 }, { 1,1,1,1 }, { 0.5f,0.5f });
		moveWaveNum[i].get()->SetSize({ 80 * 2,80 * 2 });
	}
	for (int i = 0; i < 10; i++) {
		moveWaveMaxNum[i] = Sprite::Create(100 + i, { 180,105 }, { 1,1,1,1 }, { 0.5f,0.5f });
		moveWaveMaxNum[i].get()->SetSize({ 80 * 2,80 * 2 });
	}
	//エネミーの数
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 10; i++) {
			enemyResidue[j][i] = Sprite::Create(100 + i, { (float)(80 + j * 30),120 }, { 1,1,1,1 }, { 0.5f,0.5f });
			enemyResidue[j][i].get()->SetSize({ 80 * 0.7f,80 * 0.7f });
		}
	}
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 10; i++) {
			enemyWaveMax[j][i] = Sprite::Create(100 + i, { (float)(160 + j * 30),190 }, { 1,1,1,1 }, { 0.5f,0.5f });
			enemyWaveMax[j][i].get()->SetSize({ 80 * 0.7f,80 * 0.7f });
		}
	}

	//HP
	playerHp = Sprite::Create(6, { WinApp::window_width,-20 }, { 1,1,1,1 }, { 1.0f,0.0f });
	//エネルギー
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 10; i++) {
			energyNum[j][i] = Sprite::Create(100 + i, {(float)(WinApp::window_width - 250 + j * 40),170 }, { 1,1,1,1 }, { 0,1.0f });
			energyNum[j][i].get()->SetSize({ 80,80 });
		}
	}

	//Enemy
	enemy = Sprite::Create(7, { 0, 0 }, { 1,1,1,1 }, { 0,0 });
	enemy.get()->SetSize({ 300,300 });

	for (int i = 0; i < 4; i++) {
		expGauge[i] = Sprite::Create(i + 20, { WinApp::window_width - 20,220 }, { 1,1,1,1 }, { 1.0f,0.0f });
	}

	//回収
	recoverFrame = Sprite::Create(8, { WinApp::window_width - 300,290 });
	recoverGauge = Sprite::Create(9, { WinApp::window_width - 220,290 });
	recoverColorTimer = 0;
	 

	oldWave = *nowWave;
	isChangeWave = true;
}


void UserInterface::Update()
{
	//ウェーブが更新されたら
	if (oldWave != *nowWave) {
		//イージング用にタイマーをセット
		moveWaveTimer = 0.0f;
		isChangeWave = true;
	}

	oldWave = *nowWave;
	if (isChangeWave) {
		//中央まで
		if (moveWaveTimer <= 1.0f) {
			movePosX = Ease(Out, Quad, moveWaveTimer, WinApp::window_width + 256*2, WinApp::window_width / 2);
		}
		else if (moveWaveTimer <= 2.0f) {
			movePosX = Ease(In, Quad, moveWaveTimer - 1.0f,  WinApp::window_width / 2,-256*2);
		}
		else {
			isChangeWave = false;
		}
		moveWaveTimer += 0.02f;
	}
	for (int i = 0; i < 3; i++) {
		moveWave.get()->SetPosition({ movePosX,WinApp::window_height/2 });
	}
	for (int i = 0; i < 10; i++) {
		moveWaveNum[i].get()->SetPosition({ movePosX + 50,WinApp::window_height / 2 });
		moveWaveMaxNum[i].get()->SetPosition({ movePosX + 200,WinApp::window_height / 2 });
	}


	//エネルギー描画用
	energyCon.clear();
	int  b = 0;
	for (int temp = player->GetEnergy(); temp > 0;) {
		energyCon.push_back(temp % 10);
		temp /= 10;
		b++;
	}
	if (b < 4) {
		for (int i = 0; i < 4 - b; i++) {
			energyCon.push_back(0);
		}
	}
	std::reverse(energyCon.begin(), energyCon.end());

	//今のエネミーの量
	enemyWaveCon.clear();
	b = 0;
	for (int temp = enemys->enemys.size(); temp > 0;) {
		enemyWaveCon.push_back(temp % 10);
		temp /= 10;
		b++;
	}
	if (b < 3) {
		for (int i = 0; i < 3 - b; i++) {
			enemyWaveCon.push_back(0);
		}
	}
	std::reverse(enemyWaveCon.begin(), enemyWaveCon.end());
	//ウェーブのエネミー量
	enemyWaveMaxCon.clear();
	b = 0;
	for (int temp = enemys->waveEnemyNum[enemys->nowWave]; temp > 0;) {
		enemyWaveMaxCon.push_back(temp % 10);
		temp /= 10;
		b++;
	}
	if (b < 3) {
		for (int i = 0; i < 3 - b; i++) {
			enemyWaveMaxCon.push_back(0);
		}
	}
	std::reverse(enemyWaveMaxCon.begin(), enemyWaveMaxCon.end());


	//回収ゲージの拡縮
	float recoveryGaugeLength = (float)((player->recovery.interval - player->recovery.timer) / (float)player->recovery.interval * 256.0f);
	recoverGauge.get()->SetSize({
	recoveryGaugeLength,
	128 });

	//使えないとき
	if (player->dontRecovery) {
		recoverGauge.get()->SetColor({ 1,0,0,1 });
		recoverColorTimer++;
		if (recoverColorTimer >= maxRecoverColorTimer) {
			player->dontRecovery = false;
			recoverColorTimer = 0;
			shake = { 0,0 };
			recoverGauge.get()->SetColor({ 1,1,1,1 });
		}
		else {
			shake = {
				(float)(rand() % (maxRecoverColorTimer - recoverColorTimer) - (maxRecoverColorTimer - recoverColorTimer)/2),
				(float)(rand() % (maxRecoverColorTimer - recoverColorTimer) - (maxRecoverColorTimer - recoverColorTimer) / 2)
			};
		}
		recoverFrame.get()->SetPosition({ WinApp::window_width - 300+shake.x,290 +shake.y});
		recoverGauge.get()->SetPosition({ WinApp::window_width - 220+shake.x,290 +shake.y});

	}
	else {
		recoverGauge.get()->SetColor({ 1,1,1,1 });
		recoverFrame.get()->SetColor({ 1,1,1,1 });

	}

	//たまった時
	if (player->recovery.can) {
		recoverFrame.get()->SetColor({ 1,1,0.5f,1 });
		recoverGauge.get()->SetColor({ 1,1,0.5f,1 });

	}


	//テキスト系
	//HP
	//DebugText::GetInstance()->VariablePrint(WinApp::window_width - 300, 120, "", player->GetEnergy(), 2.5f);
	//wave
	//DebugText::GetInstance()->VariablePrint(100, 50, "", *nowWave + 1, 2.5f);
	//DebugText::GetInstance()->VariablePrint(120, 50, "", 3, 2.5f);
	//DebugText::GetInstance()->VariablePrint(WinApp::window_width - 300, 100, "", player->GetEnergy(), 2.5f);

}

void UserInterface::Draw() const
{
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	waveText.get()->Draw();
	waveNum[*nowWave + 1].get()->Draw();
	waveMaxNum[enemys->MAX_WAVE+1].get()->Draw();

	moveWave.get()->Draw();
	moveWaveNum[*nowWave + 1].get()->Draw();
	moveWaveMaxNum[enemys->MAX_WAVE+1].get()->Draw();
	playerHp.get()->Draw();
	enemy.get()->Draw();
	//敵の残数
	for (int i = 0; i < 3; i++) {
		enemyResidue[i][enemyWaveCon[i]].get()->Draw();
	}
	//waveの敵の量
	for (int i = 0; i < 3; i++) {
		enemyWaveMax[i][enemyWaveMaxCon[i]].get()->Draw();
	}
	//エネルギーの残量
	for (int i = 0; i < 4; i++) {
		energyNum[i][energyCon[i]].get()->Draw();
	}
	expGauge[player->attackCount].get()->Draw();
	recoverGauge.get()->Draw();
	recoverFrame.get()->Draw();
	Sprite::PostDraw();
}
