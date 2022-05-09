#include "SelectScene.h"
#include "SceneManager.h"
#include "Easing.h"

SelectScene::SelectScene(int parameter) {
	if (maxUnlockStage < parameter) {
		maxUnlockStage = parameter;
		isUnlockStage = true;
	}
	selectCount = parameter;
}

SelectScene::~SelectScene()
{
	Finalize();
}

void SelectScene::Initialize()
{
	InitializePosSize(selectCount);
	// 背景スプライト生成
	sprite = Sprite::Create(33, { 0.0f,0.0f });
	sprite->SetSize({ 1280.0f,720.0f });

	tutorial = Sprite::Create(116, tutorialPos, color, anchorPoint);
	tutorial->SetSize(tutorialSize);
	stage1 = Sprite::Create(117, stage1Pos, color, anchorPoint);
	stage1->SetSize(stage1Size);
	stage2 = Sprite::Create(118, stage2Pos, color, anchorPoint);
	stage2->SetSize(stage2Size);
	stage3 = Sprite::Create(119, stage3Pos, color, anchorPoint);
	stage3->SetSize(stage3Size);
	stage4 = Sprite::Create(120, stage4Pos, color, anchorPoint);
	stage4->SetSize(stage4Size);
	stage5 = Sprite::Create(121, stage5Pos, color, anchorPoint);
	stage5->SetSize(stage5Size);

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

	stage1Lock = Sprite::Create(110, tutorialPos, color, anchorPoint);
	stage1Lock->SetSize(stage1LockSize);
	stage2Lock = Sprite::Create(110, stage2Pos, color, anchorPoint);
	stage2Lock->SetSize(stage2LockSize);
	stage3Lock = Sprite::Create(110, stage3Pos, color, anchorPoint);
	stage3Lock->SetSize(stage3LockSize);
	stage4Lock = Sprite::Create(110, stage4Pos, color, anchorPoint);
	stage4Lock->SetSize(stage4LockSize);
	stage5Lock = Sprite::Create(110, stage5Pos, color, anchorPoint);
	stage5Lock->SetSize(stage5LockSize);

	Audio::GetInstance()->LoopPlayWave(1, 0.5f);
}

void SelectScene::Finalize()
{
}

void SelectScene::Update()
{
	Input* input = Input::GetInstance();

	if (isUnlockStage) {
		UnlockStage(maxUnlockStage);
	}

	if (input->TriggerPadButton(BUTTON_A) ||
		input->TriggerKey(DIK_SPACE))
	{
		Audio::GetInstance()->PlayWave(16);
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

		/*if (selectCount <= maxUnlockStage) {
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
		}*/
	}

	if (input->TriggerRight() && !isSelectEase && selectCount < maxUnlockStage && !isUnlockStage) {
		selectCount++;
		isNext = true;
		isSelectEase = true;
		SavePosition();
	}
	else if (input->TriggerLeft() && !isSelectEase && selectCount > 0 && !isUnlockStage) {
		selectCount--;
		isBack = true;
		isSelectEase = true;
		SavePosition();
	}

	/*if (input->TriggerKey(DIK_1)) {
		maxUnlockStage = 1;
	}
	if (input->TriggerKey(DIK_2)) {
		isUnlockStage = true;
	}*/

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
	stage1Lock->Draw();
	stage2Lock->Draw();
	stage3Lock->Draw();
	stage4Lock->Draw();
	stage5Lock->Draw();
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
		tutorialSize = Ease(In, Quint, eTime, defaultNumSize, selectNumSize);
		stage1Size = Ease(In, Quint, eTime, selectNumSize, defaultNumSize);
	}
	else if (selectCount == 1) {
		if (isNext) {
			tutorialSize = Ease(In, Quint, eTime, selectNumSize, defaultNumSize);
			stage1Size = Ease(In, Quint, eTime, defaultNumSize, selectNumSize);
		}
		else if (isBack) {
			stage1Size = Ease(In, Quint, eTime, defaultNumSize, selectNumSize);
			stage2Size = Ease(In, Quint, eTime, selectNumSize, defaultNumSize);
		}
	}
	else if (selectCount == 2) {
		if (isNext) {
			stage1Size = Ease(In, Quint, eTime, selectNumSize, defaultNumSize);
			stage2Size = Ease(In, Quint, eTime, defaultNumSize, selectNumSize);
		}
		else if (isBack) {
			stage2Size = Ease(In, Quint, eTime, defaultNumSize, selectNumSize);
			stage3Size = Ease(In, Quint, eTime, selectNumSize, defaultNumSize);
		}
	}
	else if (selectCount == 3) {
		if (isNext) {
			stage2Size = Ease(In, Quint, eTime, selectNumSize, defaultNumSize);
			stage3Size = Ease(In, Quint, eTime, defaultNumSize, selectNumSize);
		}
		else if (isBack) {
			stage3Size = Ease(In, Quint, eTime, defaultNumSize, selectNumSize);
			stage4Size = Ease(In, Quint, eTime, selectNumSize, defaultNumSize);
		}
	}
	else if (selectCount == 4) {
		if (isNext) {
			stage3Size = Ease(In, Quint, eTime, selectNumSize, defaultNumSize);
			stage4Size = Ease(In, Quint, eTime, defaultNumSize, selectNumSize);
		}
		else if (isBack) {
			stage4Size = Ease(In, Quint, eTime, defaultNumSize, selectNumSize);
			stage5Size = Ease(In, Quint, eTime, selectNumSize, defaultNumSize);
		}
	}
	else if (selectCount == 5) {
		stage4Size = Ease(In, Quint, eTime, selectNumSize, defaultNumSize);
		stage5Size = Ease(In, Quint, eTime, defaultNumSize, selectNumSize);
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

void SelectScene::InitializePosSize(int selectCount) {
	tutorialPos = { (WinApp::window_width / 2) + (offAmount * -selectCount),WinApp::window_height / 2 };
	stage1Pos = { (WinApp::window_width / 2) + (offAmount * (-selectCount + 1)),WinApp::window_height / 2 };
	stage2Pos = { (WinApp::window_width / 2) + (offAmount * (-selectCount + 2)),WinApp::window_height / 2 };
	stage3Pos = { (WinApp::window_width / 2) + (offAmount * (-selectCount + 3)),WinApp::window_height / 2 };
	stage4Pos = { (WinApp::window_width / 2) + (offAmount * (-selectCount + 4)),WinApp::window_height / 2 };
	stage5Pos = { (WinApp::window_width / 2) + (offAmount * (-selectCount + 5)),WinApp::window_height / 2 };

	tutorialBGPos = { bgSpriteWidth * -selectCount,0 };
	stage1BGPos = { bgSpriteWidth * (-selectCount + 1),0 };
	stage2BGPos = { bgSpriteWidth * (-selectCount + 2),0 };
	stage3BGPos = { bgSpriteWidth * (-selectCount + 3),0 };
	stage4BGPos = { bgSpriteWidth * (-selectCount + 4),0 };
	stage5BGPos = { bgSpriteWidth * (-selectCount + 5),0 };

	if (selectCount == 0) {
		tutorialSize = selectNumSize;
		stage1Size = defaultNumSize;
		stage2Size = defaultNumSize;
		stage3Size = defaultNumSize;
		stage4Size = defaultNumSize;
		stage5Size = defaultNumSize;
		stage1LockSize = defaultLockSize;
		stage2LockSize = defaultLockSize;
		stage3LockSize = defaultLockSize;
		stage4LockSize = defaultLockSize;
		stage5LockSize = defaultLockSize;
	}
	else if (selectCount == 1) {
		tutorialSize = defaultNumSize;
		stage1Size = selectNumSize;
		stage2Size = defaultNumSize;
		stage3Size = defaultNumSize;
		stage4Size = defaultNumSize;
		stage5Size = defaultNumSize;
		stage1LockSize = selectLockSize;
		stage2LockSize = defaultLockSize;
		stage3LockSize = defaultLockSize;
		stage4LockSize = defaultLockSize;
		stage5LockSize = defaultLockSize;
	}
	else if (selectCount == 2) {
		tutorialSize = defaultNumSize;
		stage1Size = defaultNumSize;
		stage2Size = selectNumSize;
		stage3Size = defaultNumSize;
		stage4Size = defaultNumSize;
		stage5Size = defaultNumSize;
		stage1LockSize = defaultLockSize;
		stage2LockSize = selectLockSize;
		stage3LockSize = defaultLockSize;
		stage4LockSize = defaultLockSize;
		stage5LockSize = defaultLockSize;
	}
	else if (selectCount == 3) {
		tutorialSize = defaultNumSize;
		stage1Size = defaultNumSize;
		stage2Size = defaultNumSize;
		stage3Size = selectNumSize;
		stage4Size = defaultNumSize;
		stage5Size = defaultNumSize;
		stage1LockSize = defaultLockSize;
		stage2LockSize = defaultLockSize;
		stage3LockSize = selectLockSize;
		stage4LockSize = defaultLockSize;
		stage5LockSize = defaultLockSize;
	}
	else if (selectCount == 4) {
		tutorialSize = defaultNumSize;
		stage1Size = defaultNumSize;
		stage2Size = defaultNumSize;
		stage3Size = defaultNumSize;
		stage4Size = selectNumSize;
		stage5Size = defaultNumSize;
		stage1LockSize = defaultLockSize;
		stage2LockSize = defaultLockSize;
		stage3LockSize = defaultLockSize;
		stage4LockSize = selectLockSize;
		stage5LockSize = defaultLockSize;
	}
	else if (selectCount == 5) {
		tutorialSize = defaultNumSize;
		stage1Size = defaultNumSize;
		stage2Size = defaultNumSize;
		stage3Size = defaultNumSize;
		stage4Size = defaultNumSize;
		stage5Size = selectNumSize;
		stage1LockSize = defaultLockSize;
		stage2LockSize = defaultLockSize;
		stage3LockSize = defaultLockSize;
		stage4LockSize = defaultLockSize;
		stage5LockSize = selectLockSize;
	}
}

void SelectScene::UnlockStage(int unlockStageNum)
{
	unlockEaseTimer++;
	float eTime = (float)(unlockEaseTimer / static_cast<double>(maxUnlockEaseTimer));
	if (unlockStageNum == 1) {
		stage1LockSize = Ease(In, Back, eTime, selectLockSize, {0,0});
		stage1Lock->SetSize(stage1LockSize);
	}
	else if (unlockStageNum == 2) {
		stage2LockSize = Ease(In, Back, eTime, selectLockSize, { 0,0 });
		stage2Lock->SetSize(stage2LockSize);
	}
	else if (unlockStageNum == 3) {
		stage3LockSize = Ease(In, Back, eTime, selectLockSize, { 0,0 });
		stage3Lock->SetSize(stage3LockSize);
	}
	else if (unlockStageNum == 4) {
		stage4LockSize = Ease(In, Back, eTime, selectLockSize, { 0,0 });
		stage4Lock->SetSize(stage4LockSize);
	}
	else if (unlockStageNum == 5) {
		stage5LockSize = Ease(In, Back, eTime, selectLockSize, { 0,0 });
		stage5Lock->SetSize(stage5LockSize);
	}

	if (unlockEaseTimer >= maxUnlockEaseTimer) {
		isUnlockStage = false;
		unlockEaseTimer = 0;
	}
}
