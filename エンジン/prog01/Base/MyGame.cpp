#include "MyGame.h"
#include "SceneFactory.h"
#include "SpriteFactory.h"
#include "ObjFactory.h"
#include "Audio.h"

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
	SceneManager::GetInstance()->ChangeScene("TitleScene");

	//BGM(0〜9)
	Audio::GetInstance()->LoadWave(0, "Resources/Audio/BGM/title_01.wav");    //タイトル
	Audio::GetInstance()->LoadWave(1, "Resources/Audio/BGM/select_01.wav");   //セレクトシーン
	Audio::GetInstance()->LoadWave(2, "Resources/Audio/BGM/tutorial_01.wav"); //ステージ0
	Audio::GetInstance()->LoadWave(7, "Resources/Audio/BGM/game_05.wav");     //ステージ5
	Audio::GetInstance()->LoadWave(8, "Resources/Audio/BGM/gameover_01.wav"); //ゲームオー`バー
	Audio::GetInstance()->LoadWave(9, "Resources/Audio/BGM/clear_01.wav");    //クリア
	//SE(10〜16)
	Audio::GetInstance()->LoadWave(10, "Resources/Audio/SE/boost.wav");     //ブースト状態
	Audio::GetInstance()->LoadWave(11, "Resources/Audio/SE/damage_01.wav");   //ダメージ1
	Audio::GetInstance()->LoadWave(12, "Resources/Audio/SE/BoostUnavailable.wav");   //出来ない時の音
	Audio::GetInstance()->LoadWave(13, "Resources/Audio/SE/heal_01.wav");     //回復
	Audio::GetInstance()->LoadWave(14, "Resources/Audio/SE/recovery_01.wav"); //回収
	Audio::GetInstance()->LoadWave(15, "Resources/Audio/SE/select_01.wav");   //選択時
	Audio::GetInstance()->LoadWave(16, "Resources/Audio/SE/Select_02.wav");   //決定
	Audio::GetInstance()->LoadWave(17, "Resources/Audio/SE/wall_01.wav");     //壁に当たった時
	Audio::GetInstance()->LoadWave(18, "Resources/Audio/SE/hitStop.wav");     //ヒットストップ

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