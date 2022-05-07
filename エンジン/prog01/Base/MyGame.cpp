#include "MyGame.h"
#include "SceneFactory.h"
#include "SpriteFactory.h"
#include "ObjFactory.h"

void MyGame::Initialize()
{
	//基底クラスの初期化
	BaseGame::Initialize();

	ObjFactory::GetInstance()->Initialize();

	// シーンの初期化
	//シーンファクトリを生成し、マネージャーにセット
	sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);
	//シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("GameScene");

	SpriteFactory::GetInstance()->Initialize();
}

void MyGame::Finalize()
{
	// シーンの終了
	SceneManager::GetInstance()->Finalize();
	ObjFactory::GetInstance()->Finalize();
	//基底クラスの解放
	BaseGame::Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新
	BaseGame::Update();
}

void MyGame::Draw()
{
	BaseGame::Draw();
}