#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>

class UserInterface
{
private: //シングルトンパターン
	UserInterface() = default;
	~UserInterface() = default;
	UserInterface(const UserInterface & r) = default;
	UserInterface &operator= (const UserInterface & r) = default;
public:
	//インスタンス
	static UserInterface *GetIns();
	void Initialize(int *nowWave);
	void Update();
	void Draw() const;

private:
	//ウェーブ変更演出
	static std::unique_ptr<Sprite> wave[3];
	static std::unique_ptr<Sprite> moveWave[3];


	//現在のウェーブ数
	static int *nowWave;
	static int oldWave;
	static float moveWaveTimer;
	static float movePosX;
	static bool isChangeWave;
};

