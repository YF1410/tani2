#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>

class UserInterface
{
public:
	UserInterface(int *nowWave);
	~UserInterface();
	void Initialize();
	void Update();
	void Draw() const;

private:
	//ウェーブ変更演出
	std::unique_ptr<Sprite> wave[3];
	std::unique_ptr<Sprite> moveWave[3];


	//現在のウェーブ数
	static int *nowWave;
	static int oldWave;
	static float moveWaveTimer;
	static float movePosX;
	static bool isChangeWave;
};

