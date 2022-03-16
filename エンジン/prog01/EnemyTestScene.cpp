#include "EnemyTestScene.h"
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

using namespace DirectX;

EnemyTestScene::EnemyTestScene() {
	//モデルマネージャーに全モデルロード
	ModelManager::GetIns()->Initialize();
}

EnemyTestScene::~EnemyTestScene() {
	Finalize();
}

void EnemyTestScene::Initialize() {


	collisionManager = CollisionManager::GetInstance();

	// カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera.get());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera.get());

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) 	{
		assert(0);
	}
	// デバッグテキスト初期化
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png")) 	{
		assert(0);
	}

	// 背景スプライト生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetSize({ 100.0f,100.0f });
	sprite->SetPosition({ 100.0f,100.0f });

	// パーティクルマネージャ生成
	particleMan = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());

	//ライト生成
	light = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, true);
	light->SetDirLightActive(2, true);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, true);

	// モデル読み込み
	modelSphere = Model::CreateFromObject("sphere", true);

	// 3Dオブジェクト生成
	enemyObject = std::make_unique<Enemy>();

	//モデルテーブル
	modelPlane = Model::CreateFromObject("cube");
	Model* modeltable = modelPlane.get();

	//サウンド再生
	Audio::GetInstance()->LoadWave(0, "Resources/Alarm01.wav");

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,500,-500 });
	camera->SetUp({ 0,1,0 });

	//Debris::StaticInit();
	//playerObject->Init();
}

void EnemyTestScene::Finalize() {
}

void EnemyTestScene::Update() {
	Input* input = Input::GetInstance();
	light->Update();
	camera->Update();
	particleMan->Update();


	DebugText::GetInstance()->VariablePrint(0, 0, "angle", enemyObject->GetAngle(), 3);

	if (input->TriggerKey(DIK_C)) 	{
		SceneManager::GetInstance()->ChangeScene("ClearScene");
	}
	else if (input->TriggerKey(DIK_B)) 	{
		SceneManager::GetInstance()->ChangeScene("GameOverScene");
	}


	for (auto& object : objects) {
		object->Update();
	}
	//プレイヤー更新
	//playerObject->Update();
	//破片更新
	Debris::StaticUpdate();

	//fbxObject3d->Update();
	enemyObject->Update();
	// 全ての衝突をチェック
	collisionManager->CheckAllCollisions();
}

void EnemyTestScene::Draw() {
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
	/*for (auto& object : objects) {
		object->Draw();
	}*/
	enemyObject->Draw();
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画

#pragma region 3Dオブジェクト(FBX)描画
	//playerObject->Draw();
	Debris::StaticDraw();
#pragma endregion 3Dオブジェクト(FBX)描画
#pragma region パーティクル
	// パーティクルの描画
	particleMan->Draw(cmdList);
#pragma endregion パーティクル
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}