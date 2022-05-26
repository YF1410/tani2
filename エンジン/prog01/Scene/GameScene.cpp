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
#include "ClearConfirmation.h"

using namespace DirectX;

int GameScene::counter;

GameScene::GameScene(int parameter) {
	//ステージセット
	nowStageNum = parameter;
	//マップ生成
	MapChip::GetInstance()->SetMapName((MapChip::MAP_NAME)nowStageNum);
	MapChip::GetInstance()->CreateStage((MapChip::MAP_NAME)nowStageNum);


	//プレイヤー生成
	playerObject = std::make_unique<PlayerObject>(MapChip::GetInstance()->GetStartPos());
	enemyManager = std::make_unique<EnemyManager>(playerObject.get());
	//UI生成
	ui = std::make_unique<UserInterface>(&enemyManager->nowWave, playerObject.get(), enemyManager.get(), &counter);
	//背景セット

	//カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	enemyManager.get()->SetCam(camera.get());

	// カメラ注視点をセット
	camera->SetTarget(Vector3(playerObject.get()->GetPos() + targetDistanceDef));
	camera->SetEye(Vector3(playerObject.get()->GetPos() + eyeDistanceDef));
	camera->SetUp({ 0,1,0 });
	//フラグリセット
	isChangeBGM = false;

	if (parameter == 0) {
		tutorialFlag = true;
		Audio::GetInstance()->LoopPlayWave(2, 0.3f);
	}
	else {
		tutorialFlag = false;
		Audio::GetInstance()->LoopPlayWave(7, 0.3f);
	}
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

	light->SetDirLightDir(0, Vector3(0, -1, +0.4).Normal());

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
	stageclearObject3d->SetPosition({ 0,0,0 });

	nextStageObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("nextStage"));
	nextStageObject3d->SetRotation({ -40,0,15 });
	nextStageObject3d->SetScale({ 0, 0, 0 });
	//nextStageObject3d->SetPosition({ 3000,0,-300 });
	nextStageObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });

	clearEscapeObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("clearEscape"));
	clearEscapeObject3d->SetRotation({ -40,0,15 });
	clearEscapeObject3d->SetScale({ 0, 0, 0 });
	//clearEscapeObject3d->SetPosition({ 3000,0,-1700 });

	gameoverObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("gameover"));
	gameoverObject3d->SetRotation({ -50,0,-20 });
	gameoverObject3d->SetScale({ 5500, 5000, 2000 });
	gameoverObject3d->SetPosition({ 0,0,0 });

	retryObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("retry"));
	retryObject3d->SetRotation({ -40,0,15 });
	retryObject3d->SetScale({ 0, 0, 0 });
	//nextStageObject3d->SetPosition({ 3000,0,-300 });
	retryObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });

	gameoverEscapeObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("gameoverEscape"));
	gameoverEscapeObject3d->SetRotation({ -40,0,15 });
	gameoverEscapeObject3d->SetScale({ 0, 0, 0 });
	//clearEscapeObject3d->SetPosition({ 3000,0,-1700 });

	recoveryEffectObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("recoveryEffect"));
	recoveryEffect2Object3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("recoveryEffect2"));

	if (nowStageNum <= 1) {
		stageBGObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("stage1"));
	}
	else if (nowStageNum == 2) {
		stageBGObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("stage2"));
	}
	if (nowStageNum == 3) {
		stageBGObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("stage3"));
	}
	if (nowStageNum == 4) {
		stageBGObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("stage4"));
	}
	if (nowStageNum == 5) {
		stageBGObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("stage5"));
	}

	stageBGObject3d->SetScale({ 100, 1, 100 });
	stageBGObject3d->SetPosition({
		(MapChip::GetInstance()->GetMapData().wide * 200.0f) / 2 - 100.0f,
		-100.0f,
		-(MapChip::GetInstance()->GetMapData().high * 200.0f) / 2 + 100.0f
		});
	//デブリリセット
	Debris::StaticInitialize(playerObject.get());

	//UIの初期化
	ui->Initialize();
}

void GameScene::Finalize() {
	Debris::Finalize();
	ParticleManager::GetInstance()->Finalize();
}

void GameScene::Update() {
	//カメラ更新
	//プレイヤーの少し上を焦点にする
	//カメラ更新
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
	if (!enemyManager.get()->isCameraEasing) {
		Vector3 eyeOffset = Vector3(playerObject.get()->GetPos() +
			eyeDistanceDef +
			Vector3(0, debrisLengthMax * 0.7f, 0) +
			//xz軸へのオフセット
			playerObject.get()->velocity * velocityOffset
		);

		eyeDistance = Ease(Out, Quad, 0.02f,
			camera.get()->GetEye(),
			eyeOffset);
		camera->CameraMoveEyeVector(Vector3(eyeDistance - Vector3(camera.get()->GetEye())));
		//プレイヤーの少し上を焦点にする
		targetDistance = Ease(Out, Quad, 0.02f,
			camera.get()->GetTarget(),
			Vector3(playerObject.get()->GetPos() +
				targetDistanceDef +
				playerObject.get()->velocity * velocityOffset));

		camera->CameraMoveTargetVector(Vector3(targetDistance - Vector3(camera.get()->GetTarget())));

		camera->Update();
	}

	if (enemyManager.get()->isEndFlag()) {
		if (ui->checkFlag[0] == true && ui->checkFlag[1] == true && ui->checkFlag[2] == true && ui->checkFlag[3] == true &&
			tutorialFlag)
		{
			clearFlag = true;
			if (!isChangeBGM) {
				Audio::GetInstance()->LoopStopWave(1);
				Audio::GetInstance()->LoopPlayWave(9, 0.3f);
				isChangeBGM = true;
			}
		}
		else if (!tutorialFlag)
		{
			clearFlag = true;
			if (!isChangeBGM) {
				Audio::GetInstance()->LoopStopWave(1);
				Audio::GetInstance()->LoopPlayWave(9, 0.3f);
				isChangeBGM = true;
			}
		}
	}
	else if (playerObject.get()->GetEnergy() <= 0) {
		gameOverFlag = true;
		if (!isChangeBGM) {
			Audio::GetInstance()->LoopStopWave(1);
			Audio::GetInstance()->LoopPlayWave(8, 0.3f);
			isChangeBGM = true;
		}
	}

	if (playerObject.get()->recovery.can) {
		recoveryEffectObject3d->SetScale({ 3000.0f,1.0f,3000.0f });
		recoveryEffect2Object3d->SetScale({ 3000.0f,1.0f,3000.0f });
	}
	else if (playerObject.get()->recovery.is && (playerObject.get()->recovery.interval / 2) <= playerObject.get()->recovery.timer) {
		float eTime = (float)((playerObject.get()->recovery.interval - playerObject.get()->recovery.timer) / static_cast<double>(playerObject.get()->recovery.interval / 2));
		recoveryEffectObject3d->SetScale(Ease(Out, Quint, eTime, { 3000.0f,1.0f,3000.0f }, { 1.0f,1.0f,1.0f }));
		recoveryEffect2Object3d->SetScale(Ease(Out, Quint, eTime, { 3000.0f,1.0f,3000.0f }, { 1.0f,1.0f,1.0f }));
		recoveryEffectObject3d->SetPosition({ playerObject.get()->GetPos().x,playerObject.get()->GetPos().y + 50.0f,playerObject.get()->GetPos().z });
		recoveryEffect2Object3d->SetPosition({ playerObject.get()->GetPos().x,playerObject.get()->GetPos().y + 60.0f,playerObject.get()->GetPos().z });
	}

	recoveryEffectObject3d->SetRotation({ 0, static_cast<float>(12 * playerObject.get()->recovery.timer),0 });
	recoveryEffect2Object3d->SetRotation({ 0, static_cast<float>(4 * playerObject.get()->recovery.timer),0 });

	playerObject->SetEndFlag(clearFlag, gameOverFlag);
	if (clearFlag) {
		Clear();
	}
	else if (gameOverFlag) {
		Gameover();
	}

	//Select();

	//ライト更新
	light->Update();

	EnemyHelperManager::GetIns()->Update();
	EnemyHelperManager::GetIns()->LustUpdate();
	//ステージ更新
	//testStage->Update();

	//チュートリアルストップ
	if (MapChip::GetInstance()->nowMap == MapChip::TUTORIAL && tutorialFlag) {
		//ストップ解除処理
		if (ui.get()->tutorialNum == 5
			&& EnemyManager::enemys->size() == 0
			&& enemyManager.get()->spawnData[MapChip::GetInstance()->nowMap][0].size() == 0)
		{
			ui.get()->tutorialNum++;
		}
		if (ui.get()->tutorialNum == 12) {
			static bool push = false;
			if (Input::GetInstance()->PushKey(DIK_Q)
				|| Input::GetInstance()->TriggerPadButton(BUTTON_B)) {
				push = true;
			}
			if (playerObject.get()->healChack
				&& push
				)
			{
				ui.get()->tutorialNum++;
			}
		}

		if (!enemyManager.get()->isCameraEasing) {
			//プレイヤー更新
			playerObject->Update();
			//エネミー更新
			if (!clearFlag && !gameOverFlag) {
				enemyManager.get()->Update();
			}
		}

		//破片更新
		Debris::StaticUpdate();
		stageclearObject3d->Update();
		nextStageObject3d->Update();
		clearEscapeObject3d->Update();
		gameoverObject3d->Update();
		retryObject3d->Update();
		gameoverEscapeObject3d->Update();
		recoveryEffectObject3d->Update();
		recoveryEffect2Object3d->Update();
		stageBGObject3d->Update();
		//stageBG2Object3d->Update();

		//パーティクル全てのアップデート
		ParticleManager::GetInstance()->Update();

	}
	else if (!tutorialFlag) {

		if (!enemyManager.get()->isCameraEasing) {
			//プレイヤー更新
			playerObject->Update();
			//エネミー更新
			if (!clearFlag && !gameOverFlag) {
				enemyManager.get()->Update();
			}
		}

		//破片更新
		Debris::StaticUpdate();
		stageclearObject3d->Update();
		nextStageObject3d->Update();
		clearEscapeObject3d->Update();
		gameoverObject3d->Update();
		retryObject3d->Update();
		gameoverEscapeObject3d->Update();
		recoveryEffectObject3d->Update();
		recoveryEffect2Object3d->Update();
		stageBGObject3d->Update();
		//stageBG2Object3d->Update();

		//パーティクル全てのアップデート
		ParticleManager::GetInstance()->Update();
	}

	//カウンターを加算
	counter++;

	camera->CameraShake();
}


void GameScene::LastUpdate() {
	//ここから

	MapChip::GetInstance()->Update();
	{
		//全ての移動最終適応処理
		playerObject.get()->Adaptation();
		Debris::StaticAdaptation();
		enemyManager.get()->Adaptation();
		MapChip::GetInstance()->Adaptation();
		// 全ての衝突をチェック
		collisionManager->CheckBroadCollisions();

		enemyManager.get()->FinalUpdate();
		playerObject.get()->LustUpdate();
		Debris::StaticLustUpdate();
		//全ての移動最終適応処理
		playerObject.get()->Adaptation();
		Debris::StaticAdaptation();
		enemyManager.get()->Adaptation();
		MapChip::GetInstance()->Adaptation();
	}
	//最終更新

	ui.get()->Update();
	//ここまで
	//パーティクル全てのアップデート
	ParticleManager::GetInstance()->Update();
	sceneChange.Update();

	//チュートリアルスキップ
	if ((Input::GetInstance()->PushPadButton(BUTTON_X) || Input::GetInstance()->TriggerKey(DIK_RETURN)) && !gameOverFlag && !clearFlag)// && tutorialFlag)
	{
		longPushTimer++;
		if (longPushTimer >= 30)
		{
			clearFlag = true;
			if (!isChangeBGM)
			{
				Audio::GetInstance()->LoopStopWave(1);
				Audio::GetInstance()->LoopPlayWave(9, 0.3f);
				isChangeBGM = true;
			}
		}
	}

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
	stageBGObject3d->Draw();
	//stageBG2Object3d->Draw();
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画


#pragma region 3Dオブジェクト(FBX)描画
	//testStage->Draw(DirectXCommon::GetInstance()->GetCommandList());
	EnemyHelperManager::GetIns()->Draw();
	MapChip::GetInstance()->Draw();
	Debris::StaticDraw();
	playerObject->Draw();
	enemyManager.get()->Draw();

	Object3d::PreDraw(cmdList);
	if (playerObject.get()->recovery.is) {
		recoveryEffectObject3d->Draw();
		recoveryEffect2Object3d->Draw();
	}
	Object3d::PostDraw();

#pragma endregion 3Dオブジェクト(FBX)描画
	ParticleManager::GetInstance()->Draw(cmdList);

#pragma region パーティクル

#pragma endregion パーティクル


#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	if (!clearFlag && !gameOverFlag) {
		ui.get()->Draw();
	}

	Sprite::PreDraw(cmdList);
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);

	if (clearFlag || gameOverFlag) {
		backSprite->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 前景スプライト描画


	Object3d::PreDraw(cmdList);
	//ここから下に書く
	if (clearFlag) {
		stageclearObject3d->Draw();
		nextStageObject3d->Draw();
		clearEscapeObject3d->Draw();
	}
	else if (gameOverFlag) {
		gameoverObject3d->Draw();
		retryObject3d->Draw();
		gameoverEscapeObject3d->Draw();
	}
	Object3d::PostDraw();

	Sprite::PreDraw(cmdList);
	//シーン遷移用
	sceneChange.Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	DirectXCommon::GetInstance()->ClearDepthBuffer();

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

void GameScene::Select() {
	//入力更新
	Input* input = Input::GetInstance();
	if (saveCount == 0) {
		saveStageclearPos = stageclearPos;
		saveGameoverRot = gameoverObject3d->GetRotation();
		saveNextStagePos = nextStagePos;
		saveRetryPos = retryPos;
		saveClearEscapePos = clearEscapePos;
		saveGameoverEscapePos = gameoverEscapePos;
		saveCount++;
	}
	if ((input->TriggerUp() || input->TriggerPadStickUp() || input->TriggerKey(DIK_W) || input->TriggerKey(DIK_UP)
		|| input->TriggerDown() || input->TriggerPadStickDown() || input->TriggerKey(DIK_S) || input->TriggerKey(DIK_DOWN))
		&& !isShake && !isSceneChange) {
		if (!selectFlag) {
			selectFlag = true;
			saveNextStagePos = nextStagePos;
			saveRetryPos = retryPos;
		}
		else if (selectFlag) {
			selectFlag = false;
			saveClearEscapePos = clearEscapePos;
			saveGameoverEscapePos = gameoverEscapePos;
		}
		isShake = true;
		Audio::GetInstance()->PlayWave(15);
	}

	if ((input->TriggerPadButton(BUTTON_A) || input->TriggerKey(DIK_SPACE)) && !isSceneChange) {
		if (selectFlag) {
			Audio::GetInstance()->PlayWave(16);
			if (clearFlag) {
				ClearConfirmation::GetInstance()->SetMaxUnlockStageNum(nowStageNum + 1);
				isSceneChange = true;
				sceneChange.SceneChangeStart("SelectScene", nowStageNum + 1);
			}
			else if (gameOverFlag) {
				isSceneChange = true;
				sceneChange.SceneChangeStart("GameScene", nowStageNum);
			}
		}

		if (!selectFlag) {
			isSceneChange = true;
			Audio::GetInstance()->PlayWave(16);
			sceneChange.SceneChangeStart("TitleScene");
			//exit(1);
		}
	}

	if (clearFlag) {
		sceneBouncePosDown = saveStageclearPos;
		sceneBouncePosUp = saveStageclearPos;
		sceneBouncePosUp += bounceAmount;
		if (!selectFlag) {
			nextStageObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			nextStageObject3d->SetScale(maxNextStageScale);
			clearEscapeObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			clearEscapeObject3d->SetScale(selectScale);

			selectBouncePosDown = saveClearEscapePos;
			selectBouncePosUp = saveClearEscapePos;
			selectBouncePosUp += bounceAmount;
			if (maxBounceTimer >= bounceTimer) {
				bounceTimer++;
				if (maxBounceTimer <= bounceTimer) {
					bounceTimer = 0;
					if (!isUp) {
						isUp = true;
					}
					else if (isUp) {
						isUp = false;
					}
				}
			}
			float eTime = (float)(bounceTimer / static_cast<double>(maxBounceTimer));
			if (isUp) {
				sceneBouncePos = Ease(Out, ease::Quint, eTime, sceneBouncePosDown, sceneBouncePosUp);
				selectBouncePos = Ease(Out, ease::Quint, eTime, selectBouncePosDown, selectBouncePosUp);
			}
			else if (!isUp) {
				sceneBouncePos = Ease(In, ease::Quint, eTime, sceneBouncePosUp, sceneBouncePosDown);
				selectBouncePos = Ease(In, ease::Quint, eTime, selectBouncePosUp, selectBouncePosDown);
			}
			clearEscapeObject3d->SetPosition(selectBouncePos);
		}
		else if (selectFlag) {
			nextStageObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			nextStageObject3d->SetScale(selectScale);
			clearEscapeObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			clearEscapeObject3d->SetScale(maxClearEscapeScale);

			selectBouncePosDown = saveNextStagePos;
			selectBouncePosUp = saveNextStagePos;
			selectBouncePosUp += bounceAmount;
			if (maxBounceTimer >= bounceTimer) {
				bounceTimer++;
				if (maxBounceTimer <= bounceTimer) {
					bounceTimer = 0;
					if (!isUp) {
						isUp = true;
					}
					else if (isUp) {
						isUp = false;
					}
				}
			}
			float eTime = (float)(bounceTimer / static_cast<double>(maxBounceTimer));
			if (isUp) {
				sceneBouncePos = Ease(Out, ease::Quint, eTime, sceneBouncePosDown, sceneBouncePosUp);
				selectBouncePos = Ease(Out, ease::Quint, eTime, selectBouncePosDown, selectBouncePosUp);
			}
			else if (!isUp) {
				sceneBouncePos = Ease(In, ease::Quint, eTime, sceneBouncePosUp, sceneBouncePosDown);
				selectBouncePos = Ease(In, ease::Quint, eTime, selectBouncePosUp, selectBouncePosDown);
			}
			nextStageObject3d->SetPosition(selectBouncePos);
		}
		stageclearObject3d->SetPosition(sceneBouncePos);
	}
	if (gameOverFlag) {
		sceneRotateLeft = saveGameoverRot;
		sceneRotateRight = saveGameoverRot;
		sceneRotateRight += rotateAmount;
		if (!selectFlag) {
			retryObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			retryObject3d->SetScale(maxRetryScale);
			gameoverEscapeObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			gameoverEscapeObject3d->SetScale(selectScale);

			selectBouncePosDown = saveGameoverEscapePos;
			selectBouncePosUp = saveGameoverEscapePos;
			selectBouncePosUp += bounceAmount;
			if (maxBounceTimer >= bounceTimer) {
				bounceTimer++;
				if (maxBounceTimer <= bounceTimer) {
					bounceTimer = 0;
					if (!isUp) {
						isUp = true;
					}
					else if (isUp) {
						isUp = false;
					}
				}
			}

			if (maxRotateTimer >= rotateTimer) {
				rotateTimer++;
				if (maxRotateTimer <= rotateTimer) {
					rotateTimer = 0;
					if (!isRight) {
						isRight = true;
					}
					else if (isRight) {
						isRight = false;
					}
				}
			}

			float  bounceETime = (float)(bounceTimer / static_cast<double>(maxBounceTimer));
			float rotateETime = (float)(rotateTimer / static_cast<double>(maxRotateTimer));
			if (isUp) {
				selectBouncePos = Ease(Out, ease::Quint, bounceETime, selectBouncePosDown, selectBouncePosUp);
			}
			else if (!isUp) {
				selectBouncePos = Ease(In, ease::Quint, bounceETime, selectBouncePosUp, selectBouncePosDown);
			}

			if (isRight) {
				sceneRotate = Ease(In, ease::Quint, rotateETime, sceneRotateLeft, sceneRotateRight);
			}
			else if (!isRight) {
				sceneRotate = Ease(In, ease::Quint, rotateETime, sceneRotateRight, sceneRotateLeft);
			}

			gameoverEscapeObject3d->SetPosition(selectBouncePos);
		}
		else if (selectFlag) {
			retryObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			retryObject3d->SetScale(selectScale);
			gameoverEscapeObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			gameoverEscapeObject3d->SetScale(maxGameoverEscapeScale);

			selectBouncePosDown = saveRetryPos;
			selectBouncePosUp = saveRetryPos;
			selectBouncePosUp += bounceAmount;
			if (maxBounceTimer >= bounceTimer) {
				bounceTimer++;
				if (maxBounceTimer <= bounceTimer) {
					bounceTimer = 0;
					if (!isUp) {
						isUp = true;
					}
					else if (isUp) {
						isUp = false;
					}
				}
			}

			if (maxRotateTimer >= rotateTimer) {
				rotateTimer++;
				if (maxRotateTimer <= rotateTimer) {
					rotateTimer = 0;
					if (!isRight) {
						isRight = true;
					}
					else if (isRight) {
						isRight = false;
					}
				}
			}

			float bounceETime = (float)(bounceTimer / static_cast<double>(maxBounceTimer));
			float rotateETime = (float)(rotateTimer / static_cast<double>(maxRotateTimer));
			if (isUp) {
				selectBouncePos = Ease(Out, ease::Quint, bounceETime, selectBouncePosDown, selectBouncePosUp);
			}
			else if (!isUp) {
				selectBouncePos = Ease(In, ease::Quint, bounceETime, selectBouncePosUp, selectBouncePosDown);
			}

			if (isRight) {
				sceneRotate = Ease(In, ease::Quint, rotateETime, sceneRotateLeft, sceneRotateRight);
			}
			else if (!isRight) {
				sceneRotate = Ease(In, ease::Quint, rotateETime, sceneRotateRight, sceneRotateLeft);
			}
			retryObject3d->SetPosition(selectBouncePos);
		}
		gameoverObject3d->SetRotation(sceneRotate);
	}

	Shake(input);
}

void GameScene::Clear() {
	if (maxClearTimer >= clearTimer) {
		clearTimer++;

		stageclearPos = stageclearObject3d->GetPosition();

		float eTime = (float)(clearTimer / static_cast<double>(maxClearTimer));

		Vector3 startPos = playerObject->GetPos();
		startPos += {-700, 100, 5000};

		Vector3 endPos = playerObject->GetPos();
		endPos += {-700, 100, 1200};

		stageclearPos = Ease(Out, ease::Bounce, eTime, startPos, endPos);

		stageclearObject3d->SetPosition(stageclearPos);
	}
	else if (maxClearTimer <= clearTimer) {
		OutBack();
	}
}

void GameScene::Gameover() {
	if (maxGameoverTimer >= gameoverTimer) {
		gameoverTimer++;
	}
	else if (maxGameoverTimer <= gameoverTimer) {
		OutBack();
	}

	gameoverPos = gameoverObject3d->GetPosition();

	float eTime = (float)(gameoverTimer / static_cast<double>(maxGameoverTimer));

	Vector3 startPos = playerObject->GetPos();
	startPos += {-700, 100, 5000};

	Vector3 endPos = playerObject->GetPos();
	endPos += {-700, 100, 1200};

	gameoverPos = Ease(Out, ease::Quint, eTime, startPos, endPos);

	gameoverObject3d->SetPosition(gameoverPos);
}

void GameScene::OutBack() {
	if (clearFlag) {
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
	else if (gameOverFlag) {
		if (maxRetryScaleTimer >= retryScaleTimer) {
			retryScaleTimer++;
		}

		if (retryScaleTimer >= maxRetryScaleTimer / 2) {
			if (maxGameoverEscapeTimer >= gameoverEscapeTimer) {
				gameoverEscapeTimer++;
			}
		}

		XMFLOAT3 retryScale = retryObject3d->GetScale();
		XMFLOAT3 gameoverEscapeScale = gameoverEscapeObject3d->GetScale();

		float retryETime = (float)(retryScaleTimer / static_cast<double>(maxRetryScaleTimer));
		float gameoverEscapeETime = (float)(gameoverEscapeTimer / static_cast<double>(maxGameoverEscapeTimer));

		retryScale = Ease(Out, ease::Back, retryETime, { 0,0,0 }, maxRetryScale);
		gameoverEscapeScale = Ease(Out, ease::Back, gameoverEscapeETime, { 0,0,0 }, maxGameoverEscapeScale);

		retryPos = playerObject->GetPos();
		gameoverEscapePos = playerObject->GetPos();
		retryPos += {1400, 100, -300};
		gameoverEscapePos += {1200, 100, -1000};


		retryObject3d->SetPosition(retryPos);
		gameoverEscapeObject3d->SetPosition(gameoverEscapePos);
		retryObject3d->SetScale(retryScale);
		gameoverEscapeObject3d->SetScale(gameoverEscapeScale);

		if (maxGameoverEscapeTimer <= gameoverEscapeTimer) {
			Select();
		}
	}
}

void GameScene::Shake(Input* input) {
	//input->SetVibrationPower(5000);

	if (!selectFlag && isShake) {
		if (clearFlag) {
			XMFLOAT3 shake = {};
			shakeTimer++;

			input->SetVibration(true);

			if (shakeTimer > 0) {
				shake.x = (rand() % (100 - attenuation) - 50) + saveClearEscapePos.x;
				shake.y = saveClearEscapePos.y;
				shake.z = (rand() % (100 - attenuation) - 50) + saveClearEscapePos.z;
			}

			if (shakeTimer >= attenuation * 2) {
				attenuation += 1;
				clearEscapeObject3d->SetPosition(shake);
			}
			else if (attenuation >= 6) {
				shakeTimer = 0;
				attenuation = 0;
				isShake = false;
				input->SetVibration(false);
				clearEscapeObject3d->SetPosition(saveClearEscapePos);
			}
		}
		else if (gameOverFlag) {
			XMFLOAT3 shake = {};
			shakeTimer++;

			input->SetVibration(true);

			if (shakeTimer > 0) {
				shake.x = (rand() % (100 - attenuation) - 50) + saveGameoverEscapePos.x;
				shake.y = saveGameoverEscapePos.y;
				shake.z = (rand() % (100 - attenuation) - 50) + saveGameoverEscapePos.z;
			}

			if (shakeTimer >= attenuation * 2) {
				attenuation += 1;
				gameoverEscapeObject3d->SetPosition(shake);
			}
			else if (attenuation >= 6) {
				shakeTimer = 0;
				attenuation = 0;
				isShake = false;
				input->SetVibration(false);
				gameoverEscapeObject3d->SetPosition(saveGameoverEscapePos);
			}
		}
	}
	else if (selectFlag && isShake) {
		if (clearFlag) {
			XMFLOAT3 shake = {};
			shakeTimer++;
			input->SetVibration(true);

			if (shakeTimer > 0) {
				shake.x = (rand() % (100 - attenuation) - 50) + saveNextStagePos.x;
				shake.y = saveNextStagePos.y;
				shake.z = (rand() % (100 - attenuation) - 50) + saveNextStagePos.z;
			}

			if (shakeTimer >= attenuation * 2) {
				attenuation += 1;
				nextStageObject3d->SetPosition(shake);
			}
			else if (attenuation >= 6) {
				shakeTimer = 0;
				attenuation = 0;
				isShake = false;
				input->SetVibration(false);
				nextStageObject3d->SetPosition(saveNextStagePos);
			}
		}
		if (gameOverFlag) {
			XMFLOAT3 shake = {};
			shakeTimer++;
			input->SetVibration(true);

			if (shakeTimer > 0) {
				shake.x = (rand() % (100 - attenuation) - 50) + saveRetryPos.x;
				shake.y = saveRetryPos.y;
				shake.z = (rand() % (100 - attenuation) - 50) + saveRetryPos.z;
			}

			if (shakeTimer >= attenuation * 2) {
				attenuation += 1;
				retryObject3d->SetPosition(shake);
			}
			else if (attenuation >= 6) {
				shakeTimer = 0;
				attenuation = 0;
				isShake = false;
				input->SetVibration(false);
				retryObject3d->SetPosition(saveRetryPos);
			}
		}
	}
}