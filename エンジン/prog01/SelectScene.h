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
	void StageUnlock1(int clearStageNum);

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
	SceneChange sceneChange;

	XMFLOAT2 defaultSize = { 150.0f,150.0f };
	XMFLOAT2 selectSize = { 500.0f,500.0f };
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
	XMFLOAT2 tutorialSize = selectSize;
	XMFLOAT2 stage1Size = defaultSize;
	XMFLOAT2 stage2Size = defaultSize;
	XMFLOAT2 stage3Size = defaultSize;
	XMFLOAT2 stage4Size = defaultSize;
	XMFLOAT2 stage5Size = defaultSize;

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
	int maxSelectEaseTimer = 30;
	bool isSelectEase = false;

	bool isNext = false;
	bool isBack = false;

	int maxUnlockStage = 0;
};

