#include "SpriteFactory.h"

void SpriteFactory::Initialize()
{
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(0, L"Resources/debugfont.png"))
	{
		assert(0);
	}
	// デバッグテキスト初期化
	DebugText::GetInstance()->Initialize(0);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/Sprite/APEX_01.png"))
	{
		assert(0);
	}

	//UI周り
	//frame
	if (!Sprite::LoadTexture(2, L"Resources/Sprite/UI/frame.png")) {
		assert(0);
	}
	//ステージテキスト
	if (!Sprite::LoadTexture(4, L"Resources/Sprite/UI/stage.png")) {
		assert(0);
	}
	//ウェーブテキスト
	if (!Sprite::LoadTexture(5, L"Resources/Sprite/UI/wave.png")) {
		assert(0);
	}

	//HPフレーム
	if (!Sprite::LoadTexture(6, L"Resources/Sprite/UI/Hp.png")) {
		assert(0);
	}
	//HPゲージ
	if (!Sprite::LoadTexture(7, L"Resources/Sprite/UI/gauge_02.png")) {
		assert(0);
	}

	//ブーストフレーム
	if (!Sprite::LoadTexture(8, L"Resources/Sprite/UI/boost_01.png")) {
		assert(0);
	}
	//回収フレーム
	if (!Sprite::LoadTexture(9, L"Resources/Sprite/UI/kaisyu_01.png")) {
		assert(0);
	}
	//縦ゲージ
	if (!Sprite::LoadTexture(10, L"Resources/Sprite/UI/gauge_01.png")) {
		assert(0);
	}
	//Aボタン
	if (!Sprite::LoadTexture(11, L"Resources/Sprite/UI/A.png")) {
		assert(0);
	}
	//Bボタン
	if (!Sprite::LoadTexture(12, L"Resources/Sprite/UI/B.png")) {
		assert(0);
	}
	//RBボタン
	if (!Sprite::LoadTexture(13, L"Resources/Sprite/UI/RB.png")) {
		assert(0);
	}

	//ミニマップ
	//エネミーの位置
	if (!Sprite::LoadTexture(15, L"Resources/Sprite/UI/enemy_pos.png")) {
		assert(0);
	}
	//プレイヤーの位置
	if (!Sprite::LoadTexture(16, L"Resources/Sprite/UI/playerpos_01.png")) {
		assert(0);
	}

	//マップ0
	if (!Sprite::LoadTexture(20, L"Resources/Sprite/UI/00.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(21, L"Resources/Sprite/UI/01.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(22, L"Resources/Sprite/UI/02.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(23, L"Resources/Sprite/UI/03.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(24, L"Resources/Sprite/UI/04.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(25, L"Resources/Sprite/UI/05.png")) {
		assert(0);
	}

	//sceneの画像
	if (!Sprite::LoadTexture(33, L"Resources/Sprite/stageSelect.png")) {
		assert(0);
	}

	if (!Sprite::LoadTexture(35, L"Resources/Sprite/stageBackGround.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(36, L"Resources/Sprite/background.png")) {
		assert(0);
	}

	//クリア、ゲームオーバー時の画像
	if (!Sprite::LoadTexture(40, L"Resources/Sprite/back.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(43, L"Resources/Sprite/nextstage.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(44, L"Resources/Sprite/retry.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(45, L"Resources/Sprite/escape.png"))
	{
		assert(0);
	}

	//数字
	if (!Sprite::LoadTexture(100, L"Resources/Sprite/UI/font/0.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(101, L"Resources/Sprite/UI/font/1.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(102, L"Resources/Sprite/UI/font/2.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(103, L"Resources/Sprite/UI/font/3.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(104, L"Resources/Sprite/UI/font/4.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(105, L"Resources/Sprite/UI/font/5.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(106, L"Resources/Sprite/UI/font/6.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(107, L"Resources/Sprite/UI/font/7.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(108, L"Resources/Sprite/UI/font/8.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(109, L"Resources/Sprite/UI/font/9.png")) {
		assert(0);
	}

	
	//SceneChange
	if (!Sprite::LoadTexture(110, L"Resources/Sprite/SceneChange/SceneChangeBox.png")) {
		assert(0);
	}

	//SelectSceneの背景
	if (!Sprite::LoadTexture(111, L"Resources/Sprite/SelectSceneBG/stage1.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(112, L"Resources/Sprite/SelectSceneBG/stage2.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(113, L"Resources/Sprite/SelectSceneBG/stage3.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(114, L"Resources/Sprite/SelectSceneBG/stage4.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(115, L"Resources/Sprite/SelectSceneBG/stage5.png")) {
		assert(0);
	}
	//SelectSceneの数字
	if (!Sprite::LoadTexture(116, L"Resources/Sprite/SelectSceneNum/00.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(117, L"Resources/Sprite/SelectSceneNum/01.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(118, L"Resources/Sprite/SelectSceneNum/02.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(119, L"Resources/Sprite/SelectSceneNum/03.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(120, L"Resources/Sprite/SelectSceneNum/04.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(121, L"Resources/Sprite/SelectSceneNum/05.png")) {
		assert(0);
	}

	//チュートリアル用
	if (!Sprite::LoadTexture(150, L"Resources/Sprite/tutorial/01.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(151, L"Resources/Sprite/tutorial/02.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(152, L"Resources/Sprite/tutorial/03.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(153, L"Resources/Sprite/tutorial/04.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(154, L"Resources/Sprite/tutorial/05.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(155, L"Resources/Sprite/tutorial/06.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(156, L"Resources/Sprite/tutorial/07.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(157, L"Resources/Sprite/tutorial/08.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(158, L"Resources/Sprite/tutorial/09.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(159, L"Resources/Sprite/tutorial/10.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(160, L"Resources/Sprite/tutorial/11.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(161, L"Resources/Sprite/tutorial/12.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(162, L"Resources/Sprite/tutorial/13.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(163, L"Resources/Sprite/tutorial/14.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(164, L"Resources/Sprite/tutorial/15.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(165, L"Resources/Sprite/tutorial/16.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(166, L"Resources/Sprite/tutorial/17.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(167, L"Resources/Sprite/tutorial/18.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(168, L"Resources/Sprite/tutorial/skipText.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(169, L"Resources/Sprite/end_02.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(172, L"Resources/Sprite/tutorial/task.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(173, L"Resources/Sprite/tutorial/check.png")) {
		assert(0);
	}






	if (!Sprite::LoadTexture(170, L"Resources/Sprite/SelectSceneNum/chain.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(171, L"Resources/Sprite/SelectSceneNum/lock.png")) {
		assert(0);
	}
}


SpriteFactory* SpriteFactory::GetInstance()
{
	static SpriteFactory instance;
	return &instance;
}