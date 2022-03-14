#include "GameScene.h"
#include "Collision.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "ContactableObject.h"
#include "SceneManager.h"

using namespace DirectX;

GameScene::~GameScene()
{
	Finalize();
}

void GameScene::Initialize()
{
	collisionManager = CollisionManager::GetInstance();

	// カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera.get());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera.get());

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png"))
	{
		assert(0);
	}
	// デバッグテキスト初期化
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png"))
	{
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
	modelFighter = Model::CreateFromObject("chr_sword");
	modelSphere = Model::CreateFromObject("sphere", true);

	// 3Dオブジェクト生成
	playerObject = std::make_unique<PlayerObject>(modelFighter.get(), modelSphere.get());

	//モデルテーブル
	modelPlane = Model::CreateFromObject("cube");
	Model* modeltable = modelPlane.get();

	MapChip::GetInstance()->CsvLoad(26, 20, "Resources/map.csv");

	const float LAND_SCALE = 2.0f;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 26; j++) {

			if (MapChip::GetInstance()->GetChipNum(j, i, 1, 0))
			{
				ContactableObject* object = ContactableObject::Create(modeltable);
				object->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
				object->SetPosition({ j * LAND_SCALE, (i * -LAND_SCALE) + 30, 0 });
				objects.push_back(std::unique_ptr<Object3d>(object));
			}
		}
	}

	//.fbxの名前を指定してモデルを読み込む
	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("prin");
	// FBXオブジェクト生成
	fbxObject3d = FbxObject3d::Create(fbxModel.get(), true);
	//アニメーション
	fbxObject3d->PlayAnimation(0);

	fbxObject3d->SetScale({ 0.1f, 0.1f, 0.1f });

	//サウンド再生
	Audio::GetInstance()->LoadWave(0, "Resources/Alarm01.wav");

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,1,-150 });
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{
	Input* input = Input::GetInstance();
	light->Update();
	camera->Update();
	particleMan->Update();

	// オブジェクト移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			fighterPos[1] += 0.1f;
		}
		else if (input->PushKey(DIK_S))
		{
			fighterPos[1] -= 0.1f;
		}

		if (input->PushKey(DIK_D))
		{
			fighterPos[0] += 0.1f;
		}
		else if (input->PushKey(DIK_A))
		{
			fighterPos[0] -= 0.1f;
		}
	}

	if (input->TriggerKey(DIK_SPACE))
	{
		if (flag)
		{
			fbxObject3d->PlayAnimation(0);
			flag = false;
		}
		else if (!flag)
		{
			fbxObject3d->PlayAnimation(1);
			flag = true;
		}
	}

	DebugText::GetInstance()->VariablePrint(0, 0, "angle", input->PadStickAngle(), 3);

	/*XMFLOAT3 rot = fbxObject3d->GetRotation();
	rot.y += 1.0f;
	fbxObject3d->SetRotation(rot);*/

	if (input->TriggerKey(DIK_C))
	{
		SceneManager::GetInstance()->ChangeScene("ClearScene");
	}
	else if (input->TriggerKey(DIK_B))
	{
		SceneManager::GetInstance()->ChangeScene("GameOverScene");
	}


	for (auto& object : objects) {
		object->Update();
	}
	playerObject->Update();
	fbxObject3d->Update();
	// 全ての衝突をチェック
	collisionManager->CheckAllCollisions();
}

void GameScene::Draw()
{
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
	}
	playerObject->Draw();*/
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 3Dオブジェクト(FBX)描画
	fbxObject3d->Draw(cmdList);
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