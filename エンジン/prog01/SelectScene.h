#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
#include "Camera.h"

#include "Sprite.h"
#include "DebugText.h"
#include "Object3d.h"
#include "Model.h"

#include <memory>

#include "SceneChange.h"

class SelectScene : public BaseScene
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // メンバ関数
	SelectScene(int parameter);
	// デストラクタ
	~SelectScene();
	// 初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	// 毎フレーム処理
	void Update() override;
	// 毎フレームごとの最後の処理
	void LastUpdate() override;
	// 描画
	void Draw() override;

	void SavePosition();
	void SelectEasing();
	void InitializePosSize(int selectCount);
	void UnlockStage(int unlockStageNum);
	void selectSceneChange();

private: // メンバ変数
	//スプライト
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> tutorial;
	std::unique_ptr<Sprite> stage1;
	std::unique_ptr<Sprite> stage2;
	std::unique_ptr<Sprite> stage3;
	std::unique_ptr<Sprite> stage4;
	std::unique_ptr<Sprite> stage5;
	std::unique_ptr<Sprite> tutorialBG;
	std::unique_ptr<Sprite> stage1BG;
	std::unique_ptr<Sprite> stage2BG;
	std::unique_ptr<Sprite> stage3BG;
	std::unique_ptr<Sprite> stage4BG;
	std::unique_ptr<Sprite> stage5BG;

	std::unique_ptr<Sprite> stage1Lock;
	std::unique_ptr<Sprite> stage2Lock;
	std::unique_ptr<Sprite> stage3Lock;
	std::unique_ptr<Sprite> stage4Lock;
	std::unique_ptr<Sprite> stage5Lock;
	std::unique_ptr<Sprite> stage1Chain;
	std::unique_ptr<Sprite> stage2Chain;
	std::unique_ptr<Sprite> stage3Chain;
	std::unique_ptr<Sprite> stage4Chain;
	std::unique_ptr<Sprite> stage5Chain;

	SceneChange sceneChange;

	int maxUnlockStage = 0;

	XMFLOAT2 defaultNumSize = { 150.0f,150.0f };
	XMFLOAT2 selectNumSize = { 400.0f,400.0f };
	XMFLOAT2 defaultChainSize = {145.0f,145.0f};
	XMFLOAT2 defaultLockSize = {135.0f,135.0f};
	XMFLOAT2 selectChainSize = { 400.0f,400.0f };
	XMFLOAT2 selectLockSize = { 370.0f,370.0f };

	float defaultRotation = 0;

	float offAmount = 400.0f;
	int selectCount = 0;
	XMFLOAT2 tutorialPos = { (WinApp::window_width / 2) + (offAmount * selectCount),WinApp::window_height / 2 };
	XMFLOAT2 stage1Pos = { (WinApp::window_width / 2) + (offAmount * (selectCount + 1)),WinApp::window_height / 2 };
	XMFLOAT2 stage2Pos = { (WinApp::window_width / 2) + (offAmount * (selectCount + 2)),WinApp::window_height / 2 };
	XMFLOAT2 stage3Pos = { (WinApp::window_width / 2) + (offAmount * (selectCount + 3)),WinApp::window_height / 2 };
	XMFLOAT2 stage4Pos = { (WinApp::window_width / 2) + (offAmount * (selectCount + 4)),WinApp::window_height / 2 };
	XMFLOAT2 stage5Pos = { (WinApp::window_width / 2) + (offAmount * (selectCount + 5)),WinApp::window_height / 2 };
	XMFLOAT2 saveTutorialPos = { 0,0 };
	XMFLOAT2 saveStage1Pos = { 0,0 };
	XMFLOAT2 saveStage2Pos = { 0,0 };
	XMFLOAT2 saveStage3Pos = { 0,0 };
	XMFLOAT2 saveStage4Pos = { 0,0 };
	XMFLOAT2 saveStage5Pos = { 0,0 };
	XMFLOAT2 tutorialSize = selectNumSize;
	XMFLOAT2 stage1Size = defaultNumSize;
	XMFLOAT2 stage2Size = defaultNumSize;
	XMFLOAT2 stage3Size = defaultNumSize;
	XMFLOAT2 stage4Size = defaultNumSize;
	XMFLOAT2 stage5Size = defaultNumSize;
	XMFLOAT2 stage1LockPosition = stage1Pos;
	XMFLOAT2 stage2LockPosition = stage2Pos;
	XMFLOAT2 stage3LockPosition = stage3Pos;
	XMFLOAT2 stage4LockPosition = stage4Pos;
	XMFLOAT2 stage5LockPosition = stage5Pos;
	XMFLOAT2 stage1LockSize = defaultLockSize;
	XMFLOAT2 stage2LockSize = defaultLockSize;
	XMFLOAT2 stage3LockSize = defaultLockSize;
	XMFLOAT2 stage4LockSize = defaultLockSize;
	XMFLOAT2 stage5LockSize = defaultLockSize;
	XMFLOAT2 stage1ChainSize = defaultChainSize;
	XMFLOAT2 stage2ChainSize = defaultChainSize;
	XMFLOAT2 stage3ChainSize = defaultChainSize;
	XMFLOAT2 stage4ChainSize = defaultChainSize;
	XMFLOAT2 stage5ChainSize = defaultChainSize;

	float bgSpriteWidth = 1280.0f;
	XMFLOAT2 tutorialBGPos = { bgSpriteWidth * selectCount,0 };
	XMFLOAT2 stage1BGPos = { bgSpriteWidth * (selectCount + 1),0 };
	XMFLOAT2 stage2BGPos = { bgSpriteWidth * (selectCount + 2),0 };
	XMFLOAT2 stage3BGPos = { bgSpriteWidth * (selectCount + 3),0 };
	XMFLOAT2 stage4BGPos = { bgSpriteWidth * (selectCount + 4),0 };
	XMFLOAT2 stage5BGPos = { bgSpriteWidth * (selectCount + 5),0 };
	XMFLOAT2 saveTutorialBGPos = { 0,0 };
	XMFLOAT2 saveStage1BGPos = { 0,0 };
	XMFLOAT2 saveStage2BGPos = { 0,0 };
	XMFLOAT2 saveStage3BGPos = { 0,0 };
	XMFLOAT2 saveStage4BGPos = { 0,0 };
	XMFLOAT2 saveStage5BGPos = { 0,0 };

	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 anchorPoint = { 0.5f,0.5f };

	int selectEaseTimer = 0;
	int maxSelectEaseTimer = 25;
	bool isSelectEase = false;

	bool isNext = false;
	bool isBack = false;

	int droplockEaseTimer = 0;
	int maxDroplockEaseTimer = 120;
	int unlockEaseTimer = 0;
	int maxUnlockEaseTimer = 60;
	bool isUnlockStage = false;

	bool nowSceneChange = false;

	float selectLockRotation = 0;
	float selectLockRightRotation = 15.0f;
	float selectLockLeftRotation = -15.0f;
	float rotationVelocity = -15.0f;

	int pushSelectTimer = 0;
	int maxPushSelectTimer = 20;
};

