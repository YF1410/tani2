#include "BaseGame.h"
#include "LightGroup.h"
#include "Sprite.h"
#include "Object3d.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "ModelManager.h"
#include "MapChip.h"

void BaseGame::Run()
{
	Initialize();	//この時点でロードを終わらせる

	while (true)  // ゲームループ
	{
		Update();

		if (endRequst)
		{
			break;
		}

		
		Draw();

	}

	Finalize();
}

void BaseGame::Initialize()
{
	// ゲームウィンドウの作成
	win = std::make_unique<WinApp>();
	win->CreateGameWindow();

	//DirectX初期化処理
	DirectXCommon::GetInstance()->Initialize(win.get());

	//入力の初期化
	//input = std::make_unique<Input>();
	Input::GetInstance()->Initialize(win->GetInstance(), win->GetHwnd());

	// オーディオの初期化
	if (!Audio::GetInstance()->Initialize())
	{
		assert(0);
		return;
	}
	// スプライト静的初期化
	if (!Sprite::StaticInitialize(DirectXCommon::GetInstance()->GetDevice(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
		return;
	}

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(DirectXCommon::GetInstance()->GetDevice());
	//ライト静的初期化
	LightGroup::StaticInitialize(DirectXCommon::GetInstance()->GetDevice());
	// FBXオブジェクト静的初期化
	FbxObject3d::StaticInitialize(DirectXCommon::GetInstance()->GetDevice());
	//FBXオブジェクト初期化
	FbxLoader::GetInstance()->Initialize(DirectXCommon::GetInstance()->GetDevice());

	//全モデル読み込み
	ModelManager::GetIns()->Initialize();

	//マップチップ読み込み
	MapChip::GetInstance()->Initialize();
}

void BaseGame::Finalize()
{
	delete sceneFactory;
	ModelManager::GetIns()->Finalize();
	Sprite::StaticFinalize();
	Object3d::StaticFinalize();
	FbxObject3d::StaticFinalize();
	FbxLoader::GetInstance()->Finalize();
	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
}

void BaseGame::Update()
{
	// メッセージ処理
	if (win->ProcessMessage())
	{
		endRequst = true;
		return;
	}

	// 入力関連の毎フレーム処理
	Input::GetInstance()->Update();

	// ゲームシーンの毎フレーム処理
	SceneManager::GetInstance()->Update();
}

void BaseGame::Draw()
{
	// 描画開始
	DirectXCommon::GetInstance()->PreDraw();

	// ゲームシーンの描画
	SceneManager::GetInstance()->Draw();

	// 描画終了
	DirectXCommon::GetInstance()->PostDraw();
}