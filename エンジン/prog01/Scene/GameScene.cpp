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

using namespace DirectX;

GameScene::GameScene() {
	//モデルマネージャーに全モデルロード
	ModelManager::GetIns()->Initialize();
}

GameScene::~GameScene() {
	Finalize();
}

void GameScene::Initialize() {


	collisionManager = CollisionManager::GetInstance();

	// カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera.get());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera.get());

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
	}
	// デバッグテキスト初期化
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png")) {
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


	// 3Dオブジェクト生成
	playerObject = std::make_unique<PlayerObject>();
	enemyObject = std::make_unique<Enemy>();

	//サウンド再生
	Audio::GetInstance()->LoadWave(0, "Resources/Alarm01.wav");

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,1600,-500 });
	camera->SetUp({ 0,1,0 });

	//Debris::StaticInit();
	playerObject->Init();
}

void GameScene::Finalize() {
}

void GameScene::Update() {
	light->Update();
	camera->Update();
	particleMan->Update();

	for (auto& object : objects) {
		object->Update();
	}
	//プレイヤー更新
	playerObject->Update();
	//破片更新
	Debris::StaticUpdate();
	//エネミー更新
	enemyObject->Update();

	float playerEnemyLen;	//プレイヤーとエネミーの距離
	float debrisEnemyLen;	//破片とエネミーの距離
	float lenTmp;			//破片とエネミーの距離最小値保存用
	Vector3 posTmp;			//lenTmpでの最小距離の破片の位置保存用
	playerEnemyLen = Vector3(playerObject->GetPos() - enemyObject->GetPos()).Length();

	//破片の数とエネミーの位置とで近い位置総当たりチェック
	for (int i = 0; i < Debris::debris.size(); i++) {
		debrisEnemyLen = Vector3(Debris::debris[i]->GetPos() - enemyObject->GetPos()).Length();
		if (i == 0) {
			lenTmp = playerEnemyLen;
		}
		if (lenTmp >= debrisEnemyLen) {
			lenTmp = debrisEnemyLen;
			posTmp = Debris::debris[i]->GetPos();
		}
	}

	if (Debris::debris.size() ==0) {
		//プレイヤーとの距離より索敵範囲が短ければうろうろする為にangleに乱数代入
		if (searchPlayerLen < playerEnemyLen) {
			//moveTime = 0;
			searchPlayerLen = 500.0f;
			enemyObject->SetPlayerContact(false);
		}
		//索敵範囲よりプレイヤーとの距離が短ければそちらに移動するようにangleに値を代入
		if (searchPlayerLen >= playerEnemyLen) {
			enemyObject->SetAngle(atan2(playerObject->GetPos().z - enemyObject->GetPos().z, playerObject->GetPos().x - enemyObject->GetPos().x));
			searchPlayerLen = 800.0f;
			enemyObject->SetPlayerContact(true);
		}
	}else if (Debris::debris.size() >= 1) {
		//破片との距離より索敵範囲が短ければうろうろする為にangleに乱数代入
		if (searchPlayerLen < lenTmp) {
			//moveTime = 0;
			searchPlayerLen = 500.0f;
			enemyObject->SetPlayerContact(false);
		}
		//索敵範囲より破片との距離が短ければそちらに移動するようにangleに値を代入
		if (searchPlayerLen >= lenTmp) {
			enemyObject->SetAngle(atan2(posTmp.z - enemyObject->GetPos().z, posTmp.x - enemyObject->GetPos().x));
			searchPlayerLen = 800.0f;
			enemyObject->SetPlayerContact(true);
		}

		if (lenTmp <= 100.0f) {
			//攻撃範囲内に入った時の処理
		}
		DebugText::GetInstance()->VariablePrint(0, 0, "angle", debrisEnemyLen, 3);
	}


	//fbxObject3d->Update();
	// 全ての衝突をチェック
	collisionManager->CheckAllCollisions();
	//全ての移動最終適応処理
	playerObject.get()->Reflection();
	Debris::StaticReflection();
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
	enemyObject->Draw();
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画


#pragma region 3Dオブジェクト(FBX)描画
	playerObject->Draw();
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

	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_C)) {
		SceneManager::GetInstance()->ChangeScene("EnemyTestScene");
	}
	else if (input->TriggerKey(DIK_B)) {
		SceneManager::GetInstance()->ChangeScene("PlayerTestScene");
	}
}