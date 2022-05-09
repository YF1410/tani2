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
	ui = std::make_unique<UserInterface>(&enemyManager->nowWave,playerObject.get(),enemyManager.get());
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


	//if (Input::GetInstance()->TriggerKey(DIK_1)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new Enemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_2)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new CushionEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));

	//	}
	//}
	//if (Input::GetInstance()->TriggerKey(DIK_3)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new BoundEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_4)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new AvoidanceEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_5)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new RandomMoveEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_6)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new EscapeEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_7)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new DefenseEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_8)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new KiteEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_9)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new SuctionEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_0)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new GetawayEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_P)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new RouteMoveEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//マップチップ更新
	//MapChip::GetInstance()->Update(MapChip::TEST_MAP);


	//入力更新
	Input* input = Input::GetInstance();

	//ライト更新
	light->Update();

	EnemyHelperManager::GetIns()->Update();
	EnemyHelperManager::GetIns()->LustUpdate();
	//ステージ更新
	//testStage->Update();

	//デバックテキスト

	
	//プレイヤー更新
	playerObject->Update();
	//破片更新
	Debris::StaticUpdate();
	//エネミー更新
	enemyManager.get()->Update();


	float contact_pos = 0.0f;

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

	if (playerObject.get()->GetEnergy() <= 0) {
		DebugText::GetInstance()->Print("Game Over", 0, 240, 5);
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)||
			Input::GetInstance()->TriggerPadButton(BUTTON_A)){

		}
	}
	//パーティクル全てのアップデート
	ParticleManager::GetInstance()->Update();

	//カウンターを加算
	counter++;

	camera->CameraShake();

}

void GameScene::LastUpdate() {

	if (enemyManager.get()->isEndFlag())
	{
		sceneChange.SceneChangeStart("ClearScene");
	}

	if (playerObject.get()->GetEnergy() == 0) {
		SceneManager::GetInstance()->ChangeScene("GameOverScene");
	}
	if (playerObject.get()->GetEnergy() <= 0) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) ||
			Input::GetInstance()->TriggerPadButton(BUTTON_A)) {
			sceneChange.SceneChangeStart("TitleScene");
		}
	}

	sceneChange.Update();

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
	playerObject->Draw();
	enemyManager.get()->Draw();
	
#pragma endregion 3Dオブジェクト(FBX)描画
	ParticleManager::GetInstance()->Draw(cmdList);

#pragma region パーティクル

#pragma endregion パーティクル


#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	ui.get()->Draw();
	Sprite::PreDraw(cmdList);
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);
	

	//シーン遷移用
	sceneChange.Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画


}