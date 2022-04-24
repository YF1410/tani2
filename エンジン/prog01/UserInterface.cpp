#include "UserInterface.h"
#include "WinApp.h"
#include "Easing.h"
#include "DebugText.h"

int *UserInterface::nowWave;
int UserInterface::oldWave;
bool UserInterface::isChangeWave;
float UserInterface::moveWaveTimer;
float UserInterface::movePosX;

UserInterface::UserInterface(int *nowWave, PlayerObject *player, EnemyManager *enemys)
{
	this->nowWave = nowWave;
	this->player = player;
	this->enemys = enemys;
}

UserInterface::~UserInterface()
{
}

void UserInterface::Initialize()
{
	waveText = Sprite::Create(5, { 0,0 });

	wave[0] = Sprite::Create(2, {0,0});
	wave[1] = Sprite::Create(3, {0,0});
	wave[2] = Sprite::Create(4, {0,0});

	moveWave[0] = Sprite::Create(2, { 0,0 }, {1,1,1,1}, {0.5f, 0.5f});
	moveWave[1] = Sprite::Create(3, { 0,0 }, { 1,1,1,1 }, {0.5f,0.5f});
	moveWave[2] = Sprite::Create(4, { 0,0 }, { 1,1,1,1 }, {0.5f,0.5f});

	playerHp = Sprite::Create(6, { WinApp::window_width,-20}, { 1,1,1,1 }, {1.0f,0.0f});
	
	exGauge = Sprite::Create(7, { WinApp::window_width-20,200}, { 1,1,1,1 }, {1.0f,0.0f});

	 

	oldWave = *nowWave;
	isChangeWave = true;
}


void UserInterface::Update()
{
	//�E�F�[�u���X�V���ꂽ��
	if (oldWave != *nowWave) {
		//�C�[�W���O�p�Ƀ^�C�}�[���Z�b�g
		moveWaveTimer = 0.0f;
		isChangeWave = true;
	}

	oldWave = *nowWave;
	if (isChangeWave) {
		//�����܂�
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



	//�e�L�X�g�n
	//HP
	DebugText::GetInstance()->VariablePrint(WinApp::window_width - 300, 100, "", player->GetEnergy(), 2.5f);
	//wave
	DebugText::GetInstance()->VariablePrint(100, 50, "", *nowWave + 1, 2.5f);
	DebugText::GetInstance()->VariablePrint(120, 50, "", 3, 2.5f);
	//DebugText::GetInstance()->VariablePrint(WinApp::window_width - 300, 100, "", player->GetEnergy(), 2.5f);

}

void UserInterface::Draw() const
{
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	waveText.get()->Draw();
	moveWave[*nowWave].get()->Draw();
	playerHp.get()->Draw();
	exGauge.get()->Draw();
	Sprite::PostDraw();
}
