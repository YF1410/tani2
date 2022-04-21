#include "UserInterface.h"
#include "WinApp.h"
#include "Easing.h"

std::unique_ptr<Sprite> UserInterface::wave[3];
std::unique_ptr<Sprite> UserInterface::moveWave[3];
int *UserInterface::nowWave;
int UserInterface::oldWave;
bool UserInterface::isChangeWave;
float UserInterface::moveWaveTimer;
float UserInterface::movePosX;
UserInterface *UserInterface::GetIns()
{
	static UserInterface *instance;
	return instance;
}

void UserInterface::Initialize(int *nowWave)
{
	wave[0] = Sprite::Create(2, {0,0});
	wave[1] = Sprite::Create(3, {0,0});
	wave[2] = Sprite::Create(4, {0,0});

	moveWave[0] = Sprite::Create(2, { 0,0 });
	moveWave[1] = Sprite::Create(3, { 0,0 });
	moveWave[2] = Sprite::Create(4, { 0,0 });

	this->nowWave = nowWave;
	oldWave = *nowWave;
	isChangeWave = false;
}

void UserInterface::Update()
{
	//ウェーブが更新されたら
	if (oldWave != *nowWave) {
		//イージング用にタイマーをセット
		moveWaveTimer = 0.0f;
		isChangeWave = true;
	}

	oldWave = *nowWave;
	if (isChangeWave) {
		//中央まで
		if (moveWaveTimer <= 1.0f) {
			movePosX = Ease(Out, Quint, moveWaveTimer, WinApp::window_width, WinApp::window_width / 2);
		}
		else if (moveWaveTimer <= 2.0f) {
			movePosX = Ease(In, Quint, moveWaveTimer - 1.0f,  WinApp::window_width / 2,0);
		}
		else {
			isChangeWave = false;
		}
		moveWaveTimer += 0.02f;
	}
	for (int i = 0; i < 3; i++) {
		moveWave[i].get()->SetPosition({ movePosX,WinApp::window_height/2 });
	}
}

void UserInterface::Draw() const
{
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	wave[*nowWave].get()->Draw();
	moveWave[*nowWave].get()->Draw();
	Sprite::PostDraw();
}
