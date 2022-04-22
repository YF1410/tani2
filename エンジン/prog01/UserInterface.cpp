#include "UserInterface.h"
#include "WinApp.h"
#include "Easing.h"

int *UserInterface::nowWave;
int UserInterface::oldWave;
bool UserInterface::isChangeWave;
float UserInterface::moveWaveTimer;
float UserInterface::movePosX;

UserInterface::UserInterface(int *nowWave)
{
	this->nowWave = nowWave;
}

UserInterface::~UserInterface()
{
}

void UserInterface::Initialize()
{
	wave[0] = Sprite::Create(2, {0,0});
	wave[1] = Sprite::Create(3, {0,0});
	wave[2] = Sprite::Create(4, {0,0});

	moveWave[0] = Sprite::Create(2, { 0,0 }, {1,1,1,1}, {0.5f, 0.5f});
	moveWave[1] = Sprite::Create(3, { 0,0 }, { 1,1,1,1 }, {0.5f,0.5f});
	moveWave[2] = Sprite::Create(4, { 0,0 }, { 1,1,1,1 }, {0.5f,0.5f});

	oldWave = *nowWave;
	isChangeWave = true;
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
			movePosX = Ease(Out, Quad, moveWaveTimer, WinApp::window_width + 200, WinApp::window_width / 2);
		}
		else if (moveWaveTimer <= 2.0f) {
			movePosX = Ease(In, Quad, moveWaveTimer - 1.0f,  WinApp::window_width / 2,-200);
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
