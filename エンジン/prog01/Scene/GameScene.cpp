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
#include "EnemyManager.h"
#include "Ease.h"

using namespace DirectX;

int GameScene::counter;


GameScene::GameScene() {
	MapChip::GetInstance()->Initialize();
	//マップ生成
	MapChip::GetInstance()->SetMapName(MapChip::TEST_MAP);
	MapChip::GetInstance()->CreateStage();


	//プレイヤー生成
	playerObject = std::make_unique<PlayerObject>(MapChip::GetInstance()->GetStartPos());
}

GameScene::~GameScene() {
	Finalize();
}

void GameScene::Initialize() {

	//経過フレームを0に
	counter = 0;

	collisionManager = CollisionManager::GetInstance();

	// カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera.get());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera.get());

	// 背景スプライト生成
	for (int i = 2; i < 5; i++)
	{
		std::unique_ptr<Sprite> tempsprite = Sprite::Create(i, { 0.0f,0.0f });
		tempsprite->SetSize({ 600.0f,200.0f });
		tempsprite->SetPosition({ 1280.0f,250.0f });
		weveSprite.push_back(std::move(tempsprite));
	}

	// パーティクルマネージャ生成
	//particleMan = 
	particleMan = particleMan->Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());
	particleMan2 = particleMan2->Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());

	particle = particle->Create();
	particle2 = particle2->Create();

	particle->SetParticleManager(particleMan.get(), L"APEX_01");
	particle2->SetParticleManager(particleMan2.get());

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

	light->SetDirLightDir(0, Vector3(0,-1,-0.4).Normal());

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,0,-10 });
	camera->SetUp({ 0,1,0 });
	
	//プレイヤーの初期化
	playerObject->Initialize();

	//エネミーにプレイヤーの情報を渡す
	EnemyManager::GetIns()->SetPlayer(playerObject.get());


	//デブリリセット
	Debris::StaticInitialize(playerObject.get());
}

void GameScene::Finalize() {
}

void GameScene::Update() {
	//カメラ更新
	//プレイヤーの少し上を焦点にする
	//カメラ更新
	
	//デバックテキスト
	particle->Add();
	particle2->Add();

	particle->Update();
	particle2->Update();
}

void GameScene::LastUpdate()
{
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//sprite->Draw();
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
	
#pragma endregion 3Dオブジェクト(FBX)描画


#pragma region パーティクル
	// パーティクルの描画
	particle->Draw(cmdList);
	particle2->Draw(cmdList);
#pragma endregion パーティクル


#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);
	if (weveStartTimer > 0)
	{
		weveSprite[weveCount]->Draw();
	}
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画

	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_C)) {
		SceneManager::GetInstance()->ChangeScene("EnemyTestScene");
	}
	else if (input->TriggerKey(DIK_B)) {
		SceneManager::GetInstance()->ChangeScene("PlayerTestScene");
	}
}