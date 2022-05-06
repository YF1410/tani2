#include "GameScene.h"
#include "Collision.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "CollisionManager.h"
#include "ContactableObject.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Debris.h"
#include "MapChip.h"
#include "Easing.h"
#include "EnemyHelperManager.h"
#include "Ease.h"
#include "ObjFactory.h"


using namespace DirectX;

int GameScene::counter;


GameScene::GameScene(int parameter) {
	//ステージセット
	nowStageNum = parameter;
	//マップ生成
	MapChip::GetInstance()->SetMapName((MapChip::MAP_NAME)nowStageNum);
	MapChip::GetInstance()->CreateStage();


	//プレイヤー生成
	playerObject = std::make_unique<PlayerObject>(MapChip::GetInstance()->GetStartPos());
	enemyManager = std::make_unique<EnemyManager>(playerObject.get());
	//UI生成
	ui = std::make_unique<UserInterface>(&enemyManager->nowWave, playerObject.get(), enemyManager.get());
	//背景セット


	//カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// カメラ注視点をセット
	camera->SetTarget(Vector3(playerObject.get()->GetPos() + targetDistanceDef));
	camera->SetEye(Vector3(playerObject.get()->GetPos() + eyeDistanceDef));
	camera->SetUp({ 0,1,0 });
}

GameScene::~GameScene() {
	Finalize();
}

void GameScene::Initialize() {

	//経過フレームを0に
	counter = 0;

	collisionManager = CollisionManager::GetInstance();

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera.get());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera.get());
	//
	ParticleEmitter::SetCamera(camera.get());


	//ライト生成
	light = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(light.get());
	FbxObject3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, true);

	light->SetDirLightDir(0, Vector3(0, -1, -0.4).Normal());

	//プレイヤーの初期化
	playerObject->Initialize();

	//エネミーにプレイヤーの情報を渡す
	enemyManager.get()->Initialize();


	sprite = Sprite::Create(35, { 0,0 });
	sprite->SetSize({ 1280.0f,720.0f });

	clearScreen.stateSprite = Sprite::Create(41, { 0,0 });
	clearScreen.selectSprite = Sprite::Create(43, { 0,250 });
	clearScreen.selectSprite->SetSize({ 750, 180 });
	clearScreen.endSprite = Sprite::Create(45, { -80,400 });
	clearScreen.endSprite->SetSize({ 750, 180 });

	gameOverScreen.stateSprite = Sprite::Create(42, { 0,0 });
	gameOverScreen.selectSprite = Sprite::Create(44, { 0,250 });
	gameOverScreen.selectSprite->SetSize({ 750, 180 });
	gameOverScreen.endSprite = Sprite::Create(45, { 0,400 });
	gameOverScreen.endSprite->SetSize({ 750, 180 });

	backSprite = Sprite::Create(40, { 0,0 });
	backSprite->SetSize({ 1280.0f,720.0f });
	backSprite->SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });

	stageclearObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("stageclear"));
	stageclearObject3d->SetRotation({ -50,0,-20 });
	stageclearObject3d->SetScale({ 5000, 5000, 2000 });
	stageclearObject3d->SetPosition({ 0,0,500 });

	nextStageObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("nextStage"));
	nextStageObject3d->SetRotation({ -40,0,15 });
	nextStageObject3d->SetScale({ 0, 0, 0 });
	//nextStageObject3d->SetPosition({ 3000,0,-300 });
	nextStageObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });

	clearEscapeObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("clearEscape"));
	clearEscapeObject3d->SetRotation({ -40,0,15 });
	clearEscapeObject3d->SetScale({ 0, 0, 0 });
	//clearEscapeObject3d->SetPosition({ 3000,0,-1700 });

	//デブリリセット
	Debris::StaticInitialize(playerObject.get());

	//UIの初期化
	ui->Initialize();
}

void GameScene::Finalize() {
	Debris::Finalize();
	MapChip::GetInstance()->Filnalize();
	ParticleManager::GetInstance()->Finalize();
}

void GameScene::Update() {
	//カメラ更新
		//プレイヤーの少し上を焦点にする
		//カメラ更新
	Vector3 camEye = camera.get()->GetEye();
	float debrisLengthMax = 0.0f;
	for (int i = 0; i < Debris::debris.size(); i++) {
		if (Debris::debris[i]->isFirstAttack &&
			debrisLengthMax <= Vector3(Debris::debris[i]->pos - playerObject.get()->GetPos()).Length()) {
			debrisLengthMax = Vector3(Debris::debris[i]->pos - playerObject.get()->GetPos()).Length();
		}

		if (Debris::debris[i]->state == Debris::RETURN &&
			debrisLengthMax <= Vector3(Debris::debris[i]->pos - playerObject.get()->GetPos()).Length()) {
			debrisLengthMax = Vector3(Debris::debris[i]->pos - playerObject.get()->GetPos()).Length();
		}
	}
	if (debrisLengthMax > 5000) {
		debrisLengthMax = 5000;
	}

	const float velocityOffset = 17.0f;
	//カメラのイージング制御
	eyeDistance = Ease(Out, Quad, 0.05f,
		camera.get()->GetEye(),
		Vector3(playerObject.get()->GetPos() +
			eyeDistanceDef +
			Vector3(0, debrisLengthMax * 0.7f, 0) +
			playerObject.get()->velocity * velocityOffset
		));
	camera->CameraMoveEyeVector(Vector3(eyeDistance - Vector3(camera.get()->GetEye())));
	//プレイヤーの少し上を焦点にする
	targetDistance = Ease(Out, Quad, 0.05f,
		camera.get()->GetTarget(),
		Vector3(playerObject.get()->GetPos() +
			targetDistanceDef +
			playerObject.get()->velocity * velocityOffset));

	camera->CameraMoveTargetVector(Vector3(targetDistance - Vector3(camera.get()->GetTarget())));

	camera->Update();

	if (enemyManager.get()->isEndFlag())
	{
		clearFlag = true;
	}
	else if (playerObject.get()->GetEnergy() <= 0)
	{
		gameOverFlag = true;
	}

	playerObject->SetEndFlag(clearFlag, gameOverFlag);

	if (clearFlag) {
		Clear();
	}

	//Select();

	//ライト更新
	light->Update();

	EnemyHelperManager::GetIns()->Update();

	for (auto& object : objects) {
		object->Update();
	}

	//プレイヤー更新
	playerObject->Update();
	//破片更新
	Debris::StaticUpdate();
	//エネミー更新
	enemyManager.get()->Update();

	stageclearObject3d->Update();
	nextStageObject3d->Update();
	clearEscapeObject3d->Update();

	camera->CameraShake();
}

void GameScene::LastUpdate()
{
	// 全ての衝突をチェック
	collisionManager->CheckBroadCollisions();
	//最終更新
	ui.get()->Update();
	enemyManager.get()->FinalUpdate();
	playerObject.get()->LustUpdate();
	Debris::StaticLustUpdate();
	//全ての移動最終適応処理
	playerObject.get()->Adaptation();
	Debris::StaticAdaptation();
	enemyManager.get()->Adaptation();
	MapChip::GetInstance()->Adaptation();
	//パーティクル全てのアップデート
	ParticleManager::GetInstance()->Update();

	//カウンターを加算
	counter++;
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	sprite->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景スプライト描画

#pragma region 3Dオブジェクト描画
	// 3Dオブクジェクトの描画
	Object3d::PreDraw(cmdList);
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画


#pragma region 3Dオブジェクト(FBX)描画
	//testStage->Draw(DirectXCommon::GetInstance()->GetCommandList());
	EnemyHelperManager::GetIns()->Draw();
	MapChip::GetInstance()->Draw();
	Debris::StaticDraw();
	enemyManager.get()->Draw();
	playerObject->Draw();

#pragma endregion 3Dオブジェクト(FBX)描画
	ParticleManager::GetInstance()->Draw(cmdList);

#pragma region パーティクル

#pragma endregion パーティクル


#pragma region 前景スプライト描画
	// 前景スプライト描画前処理

	if (!clearFlag && !gameOverFlag)
	{
		ui.get()->Draw();
	}

	Sprite::PreDraw(cmdList);
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);

	if (clearFlag || gameOverFlag)
	{
		backSprite->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 前景スプライト描画


	Object3d::PreDraw(cmdList);

	//ここから下に書く
	if (clearFlag)
	{
		stageclearObject3d->Draw();
		nextStageObject3d->Draw();
		clearEscapeObject3d->Draw();
	}
	else if (gameOverFlag)
	{

	}
	Object3d::PostDraw();


	/*if (enemyManager.get()->isEndFlag())
	{
		SceneManager::GetInstance()->ChangeScene("ClearScene");
	}

	if (playerObject.get()->GetEnergy() == 0) {
		SceneManager::GetInstance()->ChangeScene("GameOverScene");
	}
	if (playerObject.get()->GetEnergy() <= 0) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) ||
			Input::GetInstance()->TriggerPadButton(BUTTON_A)) {
			SceneManager::GetInstance()->ChangeScene("TitleScene");
		}
	}*/
}

void GameScene::Select()
{
	//入力更新
	Input* input = Input::GetInstance();
	if ((input->TriggerUp() && !shakeTimerFlag) || (input->TriggerDown() && !shakeTimerFlag))
	{
		if (!selectFlag)
		{
			selectFlag = true;
			saveNextStagePos = nextStagePos;
		}
		else if (selectFlag)
		{
			selectFlag = false;
			saveClearEscapePos = clearEscapePos;
		}
		shakeTimerFlag = true;
	}

	if (input->TriggerPadButton(BUTTON_A))
	{
		if (selectFlag)
		{
			exit(1);
		}
	}

	if (clearFlag)
	{
		if (!selectFlag)
		{
			nextStageObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			nextStageObject3d->SetScale(maxNextStageScale);
			clearEscapeObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			clearEscapeObject3d->SetScale(selectScale);
		}
		else if (selectFlag)
		{
			nextStageObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			nextStageObject3d->SetScale(selectScale);
			clearEscapeObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			clearEscapeObject3d->SetScale(maxClearEscapeScale);
		}
	}
	if (gameOverFlag)
	{
		if (!selectFlag)
		{
			gameOverScreen.selectSprite->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			gameOverScreen.selectSprite->SetPosition({ 0,250 });
			gameOverScreen.selectSprite->SetSize({ 750, 180 });
			gameOverScreen.endSprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			gameOverScreen.endSprite->SetPosition({ 90,400 });
			gameOverScreen.endSprite->SetSize({ 450, 100 });
		}
		else if (selectFlag)
		{
			gameOverScreen.selectSprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			gameOverScreen.selectSprite->SetPosition({ 80,280 });
			gameOverScreen.selectSprite->SetSize({ 450, 100 });
			gameOverScreen.endSprite->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			gameOverScreen.endSprite->SetPosition({ 0,350 });
			gameOverScreen.endSprite->SetSize({ 750, 180 });
		}
	}

	Shake(input);
}

void GameScene::Clear() {
	if (maxBounceTimer >= bounceTimer) {
		bounceTimer++;
	}

	if (maxBounceTimer <= bounceTimer) {
		OutBack();
	}

	XMFLOAT3 pos = stageclearObject3d->GetPosition();

	float eTime = (float)(bounceTimer / static_cast<double>(maxBounceTimer));

	Vector3 startPos = playerObject->GetPos();
	startPos += {-700,100,5000};

	Vector3 endPos = playerObject->GetPos();
	endPos += {-700,100,1200};

	pos = Ease(Out, ease::Bounce, eTime, startPos, endPos);

	stageclearObject3d->SetPosition(pos);
}

void GameScene::OutBack() {
	if (maxNextStageScaleTimer >= nextStageScaleTimer) {
		nextStageScaleTimer++;
	}

	if (nextStageScaleTimer >= maxNextStageScaleTimer / 2) {
		if (maxClearEscapeTimer >= clearEscapeTimer) {
			clearEscapeTimer++;
		}
	}

	XMFLOAT3 nextStageScale = nextStageObject3d->GetScale();
	XMFLOAT3 clearEscapeScale = clearEscapeObject3d->GetScale();

	float nextStageETime = (float)(nextStageScaleTimer / static_cast<double>(maxNextStageScaleTimer));
	float clearEscapeETime = (float)(clearEscapeTimer / static_cast<double>(maxClearEscapeTimer));

	nextStageScale = Ease(Out, ease::Back, nextStageETime, { 0,0,0 }, maxNextStageScale);
	clearEscapeScale = Ease(Out, ease::Back, clearEscapeETime, { 0,0,0 }, maxClearEscapeScale);
	
	nextStagePos = playerObject->GetPos();
	clearEscapePos = playerObject->GetPos();
	nextStagePos += {1400, 100, -300};
	clearEscapePos += {1200, 100, -1000};


	nextStageObject3d->SetPosition(nextStagePos);
	clearEscapeObject3d->SetPosition(clearEscapePos);
	nextStageObject3d->SetScale(nextStageScale);
	clearEscapeObject3d->SetScale(clearEscapeScale);

	if (maxClearEscapeTimer <= clearEscapeTimer) {
		Select();
	}
}

void GameScene::Shake(Input *input)
{
	//input->SetVibrationPower(5000);

	if (!selectFlag && shakeTimerFlag)
	{
		XMFLOAT3 shake = {};
		shakeTimer++;

		input->SetVibration(true);

		if (shakeTimer > 0)
		{
			shake.x = (rand() % (100 - attenuation) - 50) + saveClearEscapePos.x;
			shake.y = saveClearEscapePos.y;
			shake.z = (rand() % (100 - attenuation) - 50) + saveClearEscapePos.z;
		}

		if (shakeTimer >= attenuation * 2)
		{
			attenuation += 1;
			clearEscapeObject3d->SetPosition(shake);
		}
		else if (attenuation >= 6)
		{
			shakeTimer = 0;
			attenuation = 0;
			shakeTimerFlag = 0;
			input->SetVibration(false);
			clearEscapeObject3d->SetPosition(saveClearEscapePos);
		}
	}
	else if (selectFlag && shakeTimerFlag)
	{
		XMFLOAT3 shake = {};
		shakeTimer++;
		input->SetVibration(true);

		if (shakeTimer > 0)
		{
			shake.x = (rand() % (100 - attenuation) - 50) + saveNextStagePos.x;
			shake.y = saveNextStagePos.y;
			shake.z = (rand() % (100 - attenuation) - 50) + saveNextStagePos.z;
		}

		if (shakeTimer >= attenuation * 2)
		{
			attenuation += 1;
			nextStageObject3d->SetPosition(shake);
		}
		else if (attenuation >= 6)
		{
			shakeTimer = 0;
			attenuation = 0;
			shakeTimerFlag = 0;
			input->SetVibration(false);
			nextStageObject3d->SetPosition(saveNextStagePos);
		}
	}
}