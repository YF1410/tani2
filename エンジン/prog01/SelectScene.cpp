#include "SelectScene.h"
#include "SceneManager.h"

SelectScene::~SelectScene()
{
	Finalize();
}

void SelectScene::Initialize()
{
	// 背景スプライト生成
	sprite = Sprite::Create(33, { 0.0f,0.0f });
	sprite->SetSize({ 1280.0f,720.0f });
}

void SelectScene::Finalize()
{
}

void SelectScene::Update()
{
	Input* input = Input::GetInstance();

	if (input->TriggerPadButton(BUTTON_A) ||
		input->TriggerKey(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("GameScene",1);
	}

	if (input->TriggerKey(DIK_1)) {
		SceneManager::GetInstance()->ChangeScene("GameScene", 1);
	}

	if (input->TriggerKey(DIK_2)) {
		SceneManager::GetInstance()->ChangeScene("GameScene", 2);
	}

	if (input->TriggerKey(DIK_3)) {
		SceneManager::GetInstance()->ChangeScene("GameScene", 3);
	}

	if (input->TriggerKey(DIK_4)) {
		SceneManager::GetInstance()->ChangeScene("GameScene", 4);
	}

	if (input->TriggerKey(DIK_5)) {
		SceneManager::GetInstance()->ChangeScene("GameScene", 5);
	}
}

void SelectScene::LastUpdate()
{
}

void SelectScene::Draw()
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

	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}
