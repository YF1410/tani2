#include "TitleScene.h"
#include "SceneManager.h"

TitleScene::~TitleScene()
{
	Finalize();
}

void TitleScene::Initialize()
{
	// 背景スプライト生成
	sprite = Sprite::Create(30, { 0.0f,0.0f });
	sprite->SetSize({ 1280.0f,720.0f });
	sceneChange.type = SceneChange::NOT;
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	if (input->TriggerPadButton(BUTTON_A) ||
		input->TriggerKey(DIK_SPACE))
	{
		sceneChangeFlag = true;

	}
}

void TitleScene::LastUpdate()
{
	sceneChange.Update();
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

	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	//シーン遷移
	sceneChange.Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
	if (sceneChangeFlag) {
		sceneChange.SceneChangeStart("SelectScene");
	}
}