#include "TitleScene.h"
#include "SceneManager.h"
#include "ObjFactory.h"

TitleScene::~TitleScene()
{
	Finalize();
}

void TitleScene::Initialize()
{
	// カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera.get());

	// 背景スプライト生成
	sprite = Sprite::Create(36, { 0.0f,0.0f });
	sprite->SetSize({ 1280.0f,720.0f });

	//ライト生成
	light = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, false);

	titleObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("title"));
	titleObject3d->SetRotation({ -105,-35,0 });
	titleObject3d->SetScale({ 75, 1, 15 });
	titleObject3d->SetPosition({ 5,5,0 });

	startObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("start"));
	startObject3d->SetRotation({ -90,25,0 });
	startObject3d->SetScale({ 30, 1, 10 });
	startObject3d->SetPosition({ 30,-10,0 });
	startObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });

	endObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("end"));
	endObject3d->SetRotation({ -90,25,0 });
	endObject3d->SetScale({ 10, 1, 5 });
	endObject3d->SetPosition({ 30,-20,0 });

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,-10,-5 });
	camera->SetUp({ 0,1,0 });
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	if (input->TriggerPadButton(BUTTON_A))
	{
		SceneManager::GetInstance()->ChangeScene("SelectScene");
	}

	if (input->TriggerUp())
	{
		if (!flag)
		{
			flag = true;
		}
		else if (flag)
		{
			flag = false;
		}
	}
	else if (input->TriggerDown())
	{
		if (!flag)
		{
			flag = true;
		}
		else if (flag)
		{
			flag = false;
		}
	}

	if (!flag)
	{
		startObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		startObject3d->SetScale({ 30, 1, 10 });
		endObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		endObject3d->SetScale({ 10, 1, 5 });
	}
	else if (flag)
	{
		startObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		startObject3d->SetScale({ 10, 1, 5 });
		endObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		endObject3d->SetScale({ 30, 1, 10 });
	}

	titleObject3d->Update();
	startObject3d->Update();
	endObject3d->Update();
}

void TitleScene::LastUpdate()
{
}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	sprite->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 3Dオブジェクト描画
	// 3Dオブクジェクトの描画
	Object3d::PreDraw(cmdList);
	titleObject3d->Draw();
	startObject3d->Draw();
	endObject3d->Draw();
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}