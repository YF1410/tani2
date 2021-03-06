#include "UserInterface.h"
#include "WinApp.h"
#include "Easing.h"
#include "DebugText.h"
#include "Input.h"
#include "Debris.h"

int *UserInterface::nowWave;
int UserInterface::oldWave;
int UserInterface::tutorialNum;
bool UserInterface::isChangeWave;
float UserInterface::moveWaveTimer;
float UserInterface::movePosX;

UserInterface::UserInterface(int *nowWave, PlayerObject *player, EnemyManager *enemys, int *counter)
{
	this->nowWave = nowWave;
	this->player = player;
	this->enemys = enemys;
	this->counter = counter;

}

UserInterface::~UserInterface()
{
}

void UserInterface::Initialize()
{
	hpGaugeLength = 309.0f;
	hpGaugeDamageLength = 309.0f;
	saveHp = hpGaugeDamageLength;
	tutorialNum = 0;
	oldEnemySize = 0;
	//フレーム
	frame = Sprite::Create(2, { 0,0 });
	//ステージテキスト
	stageText = Sprite::Create(4, { WinApp::window_width / 2,50 }, { 1,1,1,1 }, { 0.5f, 1.0f });
	//ステージ番号
	for (int i = 0; i < 10; i++) {
		stageNum[i] = Sprite::Create(100 + i,
			{ WinApp::window_width / 2 + 40 ,55    }, { 1,1,1,1 }, { 0,1.0f });
		stageNum[i].get()->SetSize({ 50,50 });
	}
	//ウェーブテキスト
	waveText = Sprite::Create(5, { WinApp::window_width / 2,100 }, { 1,1,1,1 }, { 0.5f, 1.0f });
	//ウェーブ数
	for (int i = 0; i < 10; i++) {
		waveNum[i] = Sprite::Create(100 + i,
			{ WinApp::window_width / 2 + 10 ,110 }, { 1,1,1,1 }, { 0,1.0f });
		waveNum[i].get()->SetSize({ 80,80 });
	}
	for (int i = 0; i < 10; i++) {
		waveMaxNum[i] = Sprite::Create(100 + i,
			{ WinApp::window_width / 2 + 80,110 }, { 1,1,1,1 }, { 0,1.0f });
		waveMaxNum[i].get()->SetSize({ 80,80 });
	}
	//HPフレーム
	hpFrame = Sprite::Create(6, { WinApp::window_width / 2,WinApp::window_height-30 }, { 1,1,1,1 }, { 0.5f, 1.0f });
	//hpGauge
	hpGauge = Sprite::Create(7, { WinApp::window_width / 2 - 309.0f / 2,WinApp::window_height - 60 }, { 1,1,1,1 }, { 0.0f, 0.5f });
	hpGaugeDamage = Sprite::Create(7, { WinApp::window_width / 2 - 309.0f / 2,WinApp::window_height - 60 }, { 2,1,0,1 }, { 0.0f, 0.5f });

	//ブーストフレーム
	boostFrame = Sprite::Create(8, { WinApp::window_width / 2 - 450,WinApp::window_height / 2 }, { 1,1,1,1 }, { 0.5f, 0.5f });
	//ブーストゲージ
	boostGauge = Sprite::Create(10, { WinApp::window_width / 2 - 450,WinApp::window_height / 2 + 40 }, { 1,1,1,1 }, { 0.5f, 1.0f });
	//回収フレーム
	kaisyuFrame = Sprite::Create(9, { WinApp::window_width / 2 + 450,WinApp::window_height / 2 }, { 1,1,1,1 }, { 0.5f, 0.5f });
	//回収ゲージ
	kaisyuGauge = Sprite::Create(10, { WinApp::window_width / 2 + 450,WinApp::window_height / 2 + 40 }, { 1,1,1,1 }, { 0.5f, 1.0f });


	//動くウェーブテキスト
	moveWave = Sprite::Create(5, { 0,WinApp::window_height / 2 }, {1,1,1,1} ,{ 0.5f, 0.5f });
	moveWave.get()->SetScale(2.0f);
	//ウェーブ数
	for (int i = 0; i < 10; i++) {
		moveWaveNum[i] = Sprite::Create(100 + i, { 110,105 }, { 1,1,1,1 }, { 0.5f,0.5f });
		moveWaveNum[i].get()->SetSize({ 80 * 2,80 * 2 });
	}
	for (int i = 0; i < 10; i++) {
		moveWaveMaxNum[i] = Sprite::Create(100 + i, { 180,105 }, { 1,1,1,1 }, { 0.5f,0.5f });
		moveWaveMaxNum[i].get()->SetSize({ 80 * 2,80 * 2 });
	}
	

	//チュートリアル
	if (MapChip::GetInstance()->nowMap == 0) {
		std::unique_ptr<Sprite> text[14];
		//説明
		text[0] = Sprite::Create(150/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) }, { 1,1,1,1 }, { 0.5f,0.5f });
		text[1] = Sprite::Create(151/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2 - 310),(float)(WinApp::window_height / 2 + 50) });
		text[2] = Sprite::Create(152/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2 - 325),(float)(WinApp::window_height / 2 + 50) });
		text[3] = Sprite::Create(153/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2 - 330),(float)(WinApp::window_height / 2 + 50) });
		text[4] = Sprite::Create(154/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2 - 390),(float)(WinApp::window_height / 2 + 100) });
		text[5] = Sprite::Create(155/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2 - 300),(float)(WinApp::window_height / 2 + 50) });
		text[6] = Sprite::Create(156/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2 - 300),(float)(WinApp::window_height / 2 + 50) });
		text[7] = Sprite::Create(157/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2 - 320),(float)(WinApp::window_height / 2 + 110) });
		text[8] = Sprite::Create(158/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2 - 260),(float)(WinApp::window_height / 2 + 50) });
		text[9] = Sprite::Create(159/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2 - 280),(float)(WinApp::window_height / 2 + 50) });
		text[10] = Sprite::Create(160/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) }, { 1,1,1,1 }, { 0.5f,0.5f });
		text[11] = Sprite::Create(161/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) }, { 1,1,1,1 }, { 0.5f,0.5f });
		text[12] = Sprite::Create(162/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) }, { 1,1,1,1 }, { 0.5f,0.5f });
		for (int i = 0; i < 13; i++) {
			tutorialImag.push_back(std::move(text[i]));
		}

		background = Sprite::Create(40, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) }, { 1,1,1,1 }, { 0.5f,0.5f });
		background->SetColor({ 1,1,1,0.5f });

		//タスクリスト
		taskList = Sprite::Create(172, { (float)(WinApp::window_width / 2) - 375,100 }, { 1,1,1,1 });
		for (int i = 0; i < 4; i++) {
			check[i] = Sprite::Create(173, { (float)(WinApp::window_width / 2) - 375,100 + (i * 32.0f) }, { 1,1,1,1 });
		}
	}
	//Xボタン長押しの時のUI
	xButton = Sprite::Create(168, { WinApp::window_width / 2 - 80,WinApp::window_height / 2 + 280 });
	xButton->SetColor({ 1,1,1,0.5f });
	nextYButton = Sprite::Create(174, { WinApp::window_width  - 300,WinApp::window_height / 2 + 220 });
	endYButton = Sprite::Create(175, { WinApp::window_width - 300,WinApp::window_height / 2 + 220 });

	//ボタン
	AButton = Sprite::Create(11, { (float)(WinApp::window_width / 2 -510),(float)(WinApp::window_height / 2+230) }, { 1,1,1,1 }, { 0.5f,0.5f });
	BButton = Sprite::Create(12, { (float)(WinApp::window_width / 2 +510),(float)(WinApp::window_height / 2+230) }, { 1,1,1,1 }, { 0.5f,0.5f });
	RBButton = Sprite::Create(13, { (float)(WinApp::window_width / 2 + 350),(float)(WinApp::window_height / 2 - 250) });
	RBButton->SetSize({100,70});

	//ミニマップ
	isMinimapDraw = false;
	for (int i = 0; i < MapChip::MAP_NAME::MAX; i++) {
		mapImag[i] = Sprite::Create(20 + i/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) }, { 1,1,1,1 }, { 0.5f,0.5f });;

	}
	playerPos = Sprite::Create(16, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) }, { 1,1,1,1 }, { 0.5f,0.6f });
	playerPos.get()->SetScale(0.5f);
	//敵
	for (int i = 0; i < 50; i++) {
		enemysPos[i] = Sprite::Create(15, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) },
			{ 1,1,1,1 }, { 0.5f,0.5f });
		enemysPos[i].get()->SetScale(0.5f);

	}
	//敵
	for (int i = 0; i < 50; i++) {
		enemysPos[i] = Sprite::Create(15, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) },
			{ 1,1,1,1 }, { 0.5f,0.5f });
		enemysPos[i].get()->SetScale(0.5f);

	}

	//外側の敵
	for (int i = 0; i < 50; i++) {
		outEnemys[i] = Sprite::Create(17, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) },
			{ 1,1,1,1 }, { 0.5f,0.5f });
		outEnemys[i].get()->SetScale(0.5f);
		outEnemys[i].get()->SetColor({1,1,1,0.5f});

	}

	//回収
	recoverColorTimer = 0;
	 

	oldWave = *nowWave;
	isChangeWave = true;
	oldHp = player->hp;
}


void UserInterface::Update()
{
	
	//ウェーブが更新されたら
	if (enemys->MAX_WAVE[MapChip::GetInstance()->nowMap] > *nowWave)
	{
		if (oldWave != *nowWave) {
			//イージング用にタイマーをセット
			moveWaveTimer = 0.0f;
			isChangeWave = true;
		}

		oldWave = *nowWave;
	}

	if (Input::GetInstance()->PushKey(DIK_M) ||
		Input::GetInstance()->LongPushPadButton(BUTTON_RIGHT_SHOULDER)) {
		isMinimapDraw = true;
	}
	else {
		isMinimapDraw = false;
	}
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
		moveWaveNum[i].get()->SetPosition({ movePosX + 100,WinApp::window_height / 2 });
		moveWaveMaxNum[i].get()->SetPosition({ movePosX + 250,WinApp::window_height / 2 });
	}

	//Hpゲージの拡縮
	hpGaugeLength =
		Ease(In, Linear, 0.5f, hpGaugeLength,
		player->hp / player->maxHp *309.0f);

	//HPの変動検出
	//変動なし
	if (oldHp == player->hp) {
		if (isDamageReset) {
			saveHp = hpGaugeLength;
			isDamageReset = false;
		}
		scaleResetCount++;
	}
	//HP変動があるので長さを調整
	else {
		scaleResetCount = 0;
			resetEase = 0.0f;
			if (hpGaugeDamageLength > hpGaugeLength) {
				saveHp = hpGaugeDamageLength;
			}
			else {
				saveHp = hpGaugeLength;
				hpGaugeDamageLength = hpGaugeLength;

			}
	}

	//一定時間変動がなければ長さを0にする
	if (scaleResetCount >= scaleResetTime ) {
		if (resetEase >= 1.0f) {
			resetEase = 1.0f;
			isDamageReset = true;
		}
		else {
			float time = (saveHp - hpGaugeLength) / 1000.0f;
			resetEase += time;
		}

		hpGaugeDamageLength =
			Ease(Out, Circ, resetEase, saveHp, hpGaugeLength);
	}



	hpGauge.get()->SetSize({
		hpGaugeLength, 38
		});
	hpGaugeDamage.get()->SetSize(
		{ hpGaugeDamageLength
		,
		38
		});

	
	//色変更
	if (hpGaugeLength < 309.0f *0.4f) {
		//赤
		hpGauge.get()->SetColor({ 1,0,0,1 });
	}
	else if (hpGaugeLength < 309.0f * 0.6f) {
		//黄色
		hpGauge.get()->SetColor({ 1,1,0,1 });
	}
	else {
		hpGauge.get()->SetColor({ 0,1,1,1 });
	}

	//回収ゲージの拡縮
	float kaisyuGaugeLength = (float)((player->recovery.interval - player->recovery.timer) / (float)player->recovery.interval * 162.0f);
	kaisyuGauge.get()->SetSize({
	91,
	kaisyuGaugeLength
		});

	//体当たりゲージの拡縮
	float boostGaugeLength = (float)((player->attackCount / 3.0f)  * 162.0f);
	boostGauge.get()->SetSize({
	91 ,
	boostGaugeLength
		});

	//回収が使えないとき
	XMFLOAT2 kaisyuShake = { 0,0 };
	if (player->dontRecovery) {
		//recoverGauge.get()->SetColor({ 1,0,0,1 });
		recoverColorTimer++;
		if (recoverColorTimer >= maxRecoverColorTimer) {
			player->dontRecovery = false;
			recoverColorTimer = 0;
		}
		else {
			kaisyuShake = {
				(float)(rand() % (maxRecoverColorTimer - recoverColorTimer) - (maxRecoverColorTimer - recoverColorTimer)/2),
				(float)(rand() % (maxRecoverColorTimer - recoverColorTimer) - (maxRecoverColorTimer - recoverColorTimer) / 2)
			};
		}
	}
	kaisyuFrame.get()->SetPosition(
		{ WinApp::window_width / 2 + 450 + kaisyuShake.x,
		WinApp::window_height / 2 + kaisyuShake.y });
	kaisyuGauge.get()->SetPosition(
		{ WinApp::window_width / 2 + 450 + kaisyuShake.x,
		WinApp::window_height / 2 + 40 + kaisyuShake.y });

	//ブーストが使えないとき
	XMFLOAT2 boostShake = { 0,0 };
	if (player->dontBoost) {
		//recoverGauge.get()->SetColor({ 1,0,0,1 });
		recoverColorTimer++;
		if (recoverColorTimer >= maxRecoverColorTimer) {
			player->dontBoost = false;
			recoverColorTimer = 0;
		}
		else {
			boostShake = {
				(float)(rand() % (maxRecoverColorTimer - recoverColorTimer) - (maxRecoverColorTimer - recoverColorTimer) / 2),
				(float)(rand() % (maxRecoverColorTimer - recoverColorTimer) - (maxRecoverColorTimer - recoverColorTimer) / 2)
			};
		}
	}
	boostFrame.get()->SetPosition(
		{ WinApp::window_width / 2 - 450 + boostShake.x,
		WinApp::window_height / 2 + boostShake.y });
	boostGauge.get()->SetPosition(
		{ WinApp::window_width / 2 - 450 + boostShake.x,
		WinApp::window_height / 2 + 40 + boostShake.y });


	//回収ゲージがたまった時
	if (player->recovery.can) {
		kaisyuFrame.get()->SetColor({ 1,1,1,1 });
		kaisyuGauge.get()->SetColor({ 1,1,1,1 });
	}
	//回収ゲージがない時
	else
	{
		kaisyuFrame.get()->SetColor({ 1,0,0,1 });
		kaisyuGauge.get()->SetColor({ 1,0,0,1 });
	}
	//ブーストゲージが3の時
	if (player->attackCount == 3)
	{
		boostFrame.get()->SetColor({ 1,1,1,1 });
		boostGauge.get()->SetColor({ 1,1,1,1 });
	}
	//ブーストゲージが2の時
	else if (player->attackCount == 2)
	{
		boostFrame.get()->SetColor({ 1,1,0,1 });
		boostGauge.get()->SetColor({ 1,1,0,1 });
	}
	//ブーストゲージが1の時
	else if (player->attackCount == 1)
	{
		boostFrame.get()->SetColor({ 1,0.6,0,1 });
		boostGauge.get()->SetColor({ 1,0.6,0,1 });
	}
	//ブーストゲージがない時
	else if (player->attackCount == 0)
	{
		boostFrame.get()->SetColor({ 1,0,0,1 });
		boostGauge.get()->SetColor({ 1,0,0,1 });
	}

	//外側の敵
	outCount = 0;
	
	for (int i = 0; i < enemys->enemys[MapChip::GetInstance()->nowMap].size(); i++) {
		Vector3 enemyToPlayerLenght = Vector3(enemys->enemys[MapChip::GetInstance()->nowMap][i]->pos - player->pos);
		if (enemyToPlayerLenght.Length() >= 2000) {
			Vector3 angle = enemyToPlayerLenght;
			angle.Normalize();
			outEnemys[outCount].get()->SetRotation(ConvertNormalToDeg(angle, Vector3(0, 0, -1)).y);

			float outScale = (enemyToPlayerLenght.Length() - 2000) / 2000;
			if (outScale >= 0.5f) {
				outScale =  0.5f;
			}
			outScale = 1.0f - outScale;
			outEnemys[outCount].get()->SetScale(outScale);
			
			XMFLOAT2 outPos = {
				angle.x * 300+(float)(WinApp::window_width / 2),
				-angle.z * 200 +(float)(WinApp::window_height / 2) };
			
			outEnemys[outCount].get()->SetPosition(outPos);

			outCount++;
		}
	}

	//描画があるならミニマップを更新
	if (isMinimapDraw) {
		//
		float offsetW = (mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().x / (MapChip::GetInstance()->GetMapData().wide * MapChip::GetInstance()->chipSize));
		float offsetH = (mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().y / (MapChip::GetInstance()->GetMapData().high * MapChip::GetInstance()->chipSize));
		//プレイヤーポジション更新
		playerPos.get()->SetPosition(
			{
				player->pos.x	//プレイヤーのポジション
				* offsetW
				+ WinApp::window_width / 2 - mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().x / 2.0f
			,

				-player->pos.z 	//プレイヤーのポジション
				* offsetH
				+ WinApp::window_height / 2 - mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().y / 2.0f
			}
		);
		playerPos.get()->SetRotation(player->rotate.y);

		//エネミー更新
		for (int i = 0; i < enemys->enemys[MapChip::GetInstance()->nowMap].size(); i++) {
			enemysPos[i].get()->SetPosition(
				{
				enemys->enemys[MapChip::GetInstance()->nowMap][i]->pos.x
				* offsetW
				+ WinApp::window_width / 2 - mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().x/2.0f
				,

				-enemys->enemys[MapChip::GetInstance()->nowMap][i]->pos.z
				* offsetH
				+ WinApp::window_height / 2 - mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().y / 2.0f
				}
			);
		}
	}
	//チュートリアル
	if (MapChip::GetInstance()->nowMap == 0)
	{
		if (tutorialNum <= tutorialImag.size() - 1 &&  Input::GetInstance()->TriggerPadButton(BUTTON_Y)) {
			tutorialNum++;
		}

		//Aボタンでブースト
		if (player->animationType == PlayerObject::BOOST) {
			checkFlag[0] = true;
		}
		//Bボタンで回収
		if (player->animationType == PlayerObject::RETRIEVE) {
			checkFlag[1] = true;
		}
		//RBボタンでマップ確認
		if (isMinimapDraw) {
			checkFlag[2] = true;
		}
		//敵を倒す
		if (oldEnemySize > enemys->enemys[MapChip::GetInstance()->nowMap].size()) {
			checkFlag[3] = true;
		}

		if (tutorialNum >= tutorialImag.size())
		{
			if (isXButtonFlag && xButtonFlagTimer >= 60)
			{
				xButtonFlagTimer = 0;
				isXButtonFlag = false;
			}
			else if (!isXButtonFlag && xButtonFlagTimer >= 30)
			{
				xButtonFlagTimer = 0;
				isXButtonFlag = true;
			}

			xButtonFlagTimer++;
		}
	}

	oldEnemySize = enemys->enemys[MapChip::GetInstance()->nowMap].size();
	oldHp = player->hp;
}

void UserInterface::Draw() const
{
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	
	frame.get()->Draw(); 
	stageText.get()->Draw();
	stageNum[MapChip::GetInstance()->nowMap].get()->Draw();
	waveText.get()->Draw();
	waveNum[oldWave + 1].get()->Draw();
	waveMaxNum[enemys->MAX_WAVE[MapChip::GetInstance()->nowMap]].get()->Draw();
	hpFrame.get()->Draw();
	hpGaugeDamage.get()->Draw();
	hpGauge.get()->Draw();

	boostFrame.get()->Draw();
	boostGauge.get()->Draw();
	AButton.get()->Draw();
	kaisyuFrame.get()->Draw();
	kaisyuGauge.get()->Draw();
	BButton.get()->Draw();
	RBButton->Draw();
	
	moveWave.get()->Draw();
	moveWaveNum[*nowWave + 1].get()->Draw();
	moveWaveMaxNum[enemys->MAX_WAVE[MapChip::GetInstance()->nowMap]].get()->Draw();
	
	//recoverGauge.get()->Draw();
	//recoverFrame.get()->Draw();

	//範囲外描画
	if (outCount != 0) {
		for (int i = 0; i < outCount; i++) {
			outEnemys[i].get()->Draw();
		}
	}

	//マップ
	if (isMinimapDraw) {
		mapImag[MapChip::GetInstance()->nowMap].get()->Draw();
		playerPos.get()->Draw();
		for (int i = 0; i < enemys->enemys[MapChip::GetInstance()->nowMap].size(); i++) {
			enemysPos[i].get()->Draw();
		}
	}


	//チュートリアルでのみ描画
	if (MapChip::GetInstance()->nowMap == 0) {
		taskList.get()->Draw();
		if (tutorialNum != tutorialImag.size()) {
			background->Draw();
			tutorialImag[tutorialNum].get()->Draw();
		}

		for (int i = 0; i < 4; i++) {
			if (checkFlag[i]) {
				check[i].get()->Draw();
			}
		}
	}
	if (MapChip::GetInstance()->nowMap == 0)
	{
		if (isXButtonFlag)
		{
			xButton->Draw();
		}

		if (tutorialNum < 12) {
			nextYButton->Draw();
		}
		else if (tutorialNum == 12) {
			endYButton->Draw();
		}
	}


	Sprite::PostDraw();
}
