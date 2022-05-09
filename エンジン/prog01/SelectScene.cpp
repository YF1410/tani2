#include "SelectScene.h"
#include "SceneManager.h"
#include "Easing.h"

SelectScene::~SelectScene()
{
	Finalize();
}

void SelectScene::Initialize()
{
	// 背景スプライト生成
	sprite = Sprite::Create(33, { 0.0f,0.0f });
	sprite->SetSize({ 1280.0f,720.0f });

	tutorial = Sprite::Create(100, tutorialPos, color, anchorPoint);
	tutorial->SetSize(selectSize);
	stage1 = Sprite::Create(101, stage1Pos, color, anchorPoint);
	stage1->SetSize(defaultSize);
	stage2 = Sprite::Create(102, stage2Pos, color, anchorPoint);
	stage2->SetSize(defaultSize);
	stage3 = Sprite::Create(103, stage3Pos, color, anchorPoint);
	stage3->SetSize(defaultSize);
	stage4 = Sprite::Create(104, stage4Pos, color, anchorPoint);
	stage4->SetSize(defaultSize);
	stage5 = Sprite::Create(105, stage5Pos, color, anchorPoint);
	stage5->SetSize(defaultSize);

	tutorialBG = Sprite::Create(111, tutorialBGPos);
	tutorialBG->SetSize({ 1280.0f,720.0f });
	stage1BG = Sprite::Create(111, stage1BGPos);
	stage1BG->SetSize({ 1280.0f,720.0f });
	stage2BG = Sprite::Create(112, stage2BGPos);
	stage2BG->SetSize({ 1280.0f,720.0f });
	stage3BG = Sprite::Create(113, stage3BGPos);
	stage3BG->SetSize({ 1280.0f,720.0f });
	stage4BG = Sprite::Create(114, stage4BGPos);
	stage4BG->SetSize({ 1280.0f,720.0f });
	stage5BG = Sprite::Create(115, stage5BGPos);
	stage5BG->SetSize({ 1280.0f,720.0f });

	//Audio::GetInstance()->LoopPlayWave(1, 0.5f);
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
		if (selectCount == 0) {
			sceneChange.SceneChangeStart("GameScene", 0);
		}
		else if (selectCount == 1) {
			sceneChange.SceneChangeStart("GameScene", 1);
		}
		else if (selectCount == 2) {
			sceneChange.SceneChangeStart("GameScene", 2);
		}
		else if (selectCount == 3) {
			sceneChange.SceneChangeStart("GameScene", 3);
		}
		else if (selectCount == 4) {
			sceneChange.SceneChangeStart("GameScene", 4);
		}
		else if (selectCount == 5) {
			sceneChange.SceneChangeStart("GameScene", 5);
		}
	}

	//ステージアンロックデバッグ用
	if (input->TriggerKey(DIK_1))
	{
		StageUnlock(1);
	}
	if (input->TriggerKey(DIK_2))
	{
		StageUnlock(2);
	}
	if (input->TriggerKey(DIK_3))
	{
		StageUnlock(3);
	}
	if (input->TriggerKey(DIK_4))
	{
		StageUnlock(4);
	}
	if (input->TriggerKey(DIK_5))
	{
		StageUnlock(5);
	}

	if (input->TriggerUp() && !isSelectEase && selectCount < maxUnlockStage) {
		selectCount++;
		isNext = true;
		isSelectEase = true;
		SavePosition();
	}
	else if (input->TriggerDown() && !isSelectEase && selectCount > 0) {
		selectCount--;
		isBack = true;
		isSelectEase = true;
		SavePosition();
	}

	if (isSelectEase) {
		SelectEasing();
	}
}

void SelectScene::LastUpdate()
{
	sceneChange.Update();
}

void SelectScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//sprite->Draw();
	tutorialBG->Draw();
	stage1BG->Draw();
	stage2BG->Draw();
	stage3BG->Draw();
	stage4BG->Draw();
	stage5BG->Draw();
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
	tutorial->Draw();
	stage1->Draw();
	stage2->Draw();
	stage3->Draw();
	stage4->Draw();
	stage5->Draw();
	sceneChange.Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void SelectScene::SavePosition() {
	saveTutorialPos = tutorialPos;
	saveStage1Pos = stage1Pos;
	saveStage2Pos = stage2Pos;
	saveStage3Pos = stage3Pos;
	saveStage4Pos = stage4Pos;
	saveStage5Pos = stage5Pos;
	saveTutorialBGPos = tutorialBGPos;
	saveStage1BGPos = stage1BGPos;
	saveStage2BGPos = stage2BGPos;
	saveStage3BGPos = stage3BGPos;
	saveStage4BGPos = stage4BGPos;
	saveStage5BGPos = stage5BGPos;
}

void SelectScene::SelectEasing() {
	selectEaseTimer++;
	float eTime = (float)(selectEaseTimer / static_cast<double>(maxSelectEaseTimer));

	tutorialPos.x = Ease(In, Quint, eTime, saveTutorialPos.x, (WinApp::window_width / 2) + (offAmount * -selectCount));
	stage1Pos.x = Ease(In, Quint, eTime, saveStage1Pos.x, (WinApp::window_width / 2) + (offAmount * (-selectCount + 1)));
	stage2Pos.x = Ease(In, Quint, eTime, saveStage2Pos.x, (WinApp::window_width / 2) + (offAmount * (-selectCount + 2)));
	stage3Pos.x = Ease(In, Quint, eTime, saveStage3Pos.x, (WinApp::window_width / 2) + (offAmount * (-selectCount + 3)));
	stage4Pos.x = Ease(In, Quint, eTime, saveStage4Pos.x, (WinApp::window_width / 2) + (offAmount * (-selectCount + 4)));
	stage5Pos.x = Ease(In, Quint, eTime, saveStage5Pos.x, (WinApp::window_width / 2) + (offAmount * (-selectCount + 5)));

	tutorialBGPos.x = Ease(In, Quint, eTime, saveTutorialBGPos.x, bgSpriteWidth * -selectCount);
	stage1BGPos.x = Ease(In, Quint, eTime, saveStage1BGPos.x, bgSpriteWidth * (-selectCount + 1));
	stage2BGPos.x = Ease(In, Quint, eTime, saveStage2BGPos.x, bgSpriteWidth * (-selectCount + 2));
	stage3BGPos.x = Ease(In, Quint, eTime, saveStage3BGPos.x, bgSpriteWidth * (-selectCount + 3));
	stage4BGPos.x = Ease(In, Quint, eTime, saveStage4BGPos.x, bgSpriteWidth * (-selectCount + 4));
	stage5BGPos.x = Ease(In, Quint, eTime, saveStage5BGPos.x, bgSpriteWidth * (-selectCount + 5));

	if (selectCount == 0) {
		tutorialSize = Ease(In, Quint, eTime, defaultSize, selectSize);
		stage1Size = Ease(In, Quint, eTime, selectSize, defaultSize);
	}
	else if (selectCount == 1) {
		if (isNext) {
			tutorialSize = Ease(In, Quint, eTime, selectSize, defaultSize);
			stage1Size = Ease(In, Quint, eTime, defaultSize, selectSize);
		}
		else if (isBack) {
			stage1Size = Ease(In, Quint, eTime, defaultSize, selectSize);
			stage2Size = Ease(In, Quint, eTime, selectSize, defaultSize);
		}
	}
	else if (selectCount == 2) {
		if (isNext) {
			stage1Size = Ease(In, Quint, eTime, selectSize, defaultSize);
			stage2Size = Ease(In, Quint, eTime, defaultSize, selectSize);
		}
		else if (isBack) {
			stage2Size = Ease(In, Quint, eTime, defaultSize, selectSize);
			stage3Size = Ease(In, Quint, eTime, selectSize, defaultSize);
		}
	}
	else if (selectCount == 3) {
		if (isNext) {
			stage2Size = Ease(In, Quint, eTime, selectSize, defaultSize);
			stage3Size = Ease(In, Quint, eTime, defaultSize, selectSize);
		}
		else if (isBack) {
			stage3Size = Ease(In, Quint, eTime, defaultSize, selectSize);
			stage4Size = Ease(In, Quint, eTime, selectSize, defaultSize);
		}
	}
	else if (selectCount == 4) {
		if (isNext) {
			stage3Size = Ease(In, Quint, eTime, selectSize, defaultSize);
			stage4Size = Ease(In, Quint, eTime, defaultSize, selectSize);
		}
		else if (isBack) {
			stage4Size = Ease(In, Quint, eTime, defaultSize, selectSize);
			stage5Size = Ease(In, Quint, eTime, selectSize, defaultSize);
		}
	}
	else if (selectCount == 5) {
		stage4Size = Ease(In, Quint, eTime, selectSize, defaultSize);
		stage5Size = Ease(In, Quint, eTime, defaultSize, selectSize);
	}


	tutorial->SetPosition(tutorialPos);
	stage1->SetPosition(stage1Pos);
	stage2->SetPosition(stage2Pos);
	stage3->SetPosition(stage3Pos);
	stage4->SetPosition(stage4Pos);
	stage5->SetPosition(stage5Pos);

	tutorialBG->SetPosition(tutorialBGPos);
	stage1BG->SetPosition(stage1BGPos);
	stage2BG->SetPosition(stage2BGPos);
	stage3BG->SetPosition(stage3BGPos);
	stage4BG->SetPosition(stage4BGPos);
	stage5BG->SetPosition(stage5BGPos);

	tutorial->SetSize(tutorialSize);
	stage1->SetSize(stage1Size);
	stage2->SetSize(stage2Size);
	stage3->SetSize(stage3Size);
	stage4->SetSize(stage4Size);
	stage5->SetSize(stage5Size);

	if (selectEaseTimer >= maxSelectEaseTimer) {
		isSelectEase = false;
		isNext = false;
		isBack = false;
		selectEaseTimer = 0;
	}
}

void SelectScene::StageUnlock(int clearStageNum)
{
	if (clearStageNum > maxUnlockStage) {
		maxUnlockStage++;
	}
}
