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

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(2, L"Resources/Sprite/weve1.png")) {
		assert(0);
	}
	// テクスチャ読み込み
	if (!Sprite::LoadTexture(3, L"Resources/Sprite/weve2.png")) {
		assert(0);
	}
	// テクスチャ読み込み
	if (!Sprite::LoadTexture(4, L"Resources/Sprite/weve3.png")) {
		assert(0);
	}


	if (!Sprite::LoadTexture(5, L"Resources/Sprite/UI/wave.png")) {
		assert(0);
	}


	//UI周り
	//HP
	if (!Sprite::LoadTexture(6, L"Resources/Sprite/UI/playerHp.png")) {
		assert(0);
	}
	//体当たり
	if (!Sprite::LoadTexture(7, L"Resources/Sprite/UI/exGauge/expGaugeEmpty.png")) {
		assert(0);
	}
	//回収
	if (!Sprite::LoadTexture(8, L"Resources/Sprite/UI/recover/recovery.png")) {
		assert(0);
	}




}

SpriteFactory* SpriteFactory::GetInstance()
{
	static SpriteFactory instance;
	return &instance;
}