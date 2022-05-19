#include "UserInterface.h"
#include "WinApp.h"
#include "Easing.h"
#include "DebugText.h"
#include "Input.h"

int *UserInterface::nowWave;
int UserInterface::oldWave;
bool UserInterface::isChangeWave;
float UserInterface::moveWaveTimer;
float UserInterface::movePosX;

UserInterface::UserInterface(int *nowWave, PlayerObject *player, EnemyManager *enemys, int *counter)
{
	this->nowWave = nowWave;
	this->player = player;
	this->enemys = enemys;
	this->counter = counter;

}

UserInterface::~UserInterface()
{
}

void UserInterface::Initialize()
{
	if (MapChip::GetInstance()->nowMap == MapChip::TUTORIAL) {
		stopFrag = true;
	}
	else {
		stopFrag = false;
	}
	//�t���[��
	frame = Sprite::Create(2, { 0,0 });
	//�X�e�[�W�e�L�X�g
	stageText = Sprite::Create(4, { WinApp::window_width / 2,50 }, { 1,1,1,1 }, { 0.5f, 1.0f });
	//�X�e�[�W�ԍ�
	for (int i = 0; i < 10; i++) {
		stageNum[i] = Sprite::Create(100 + i,
			{ WinApp::window_width / 2 + 40 ,5    }, { 1,1,1,1 }, { 0,1.0f });
		stageNum[i].get()->SetSize({ 50,50 });
	}
	//�E�F�[�u�e�L�X�g
	waveText = Sprite::Create(5, { WinApp::window_width / 2,100 }, { 1,1,1,1 }, { 0.5f, 1.0f });
	//�E�F�[�u��
	for (int i = 0; i < 10; i++) {
		waveNum[i] = Sprite::Create(100 + i,
			{ WinApp::window_width / 2 -10 ,110 }, { 1,1,1,1 }, { 0,1.0f });
		waveNum[i].get()->SetSize({ 80,80 });
	}
	for (int i = 0; i < 10; i++) {
		waveMaxNum[i] = Sprite::Create(100 + i,
			{ WinApp::window_width / 2 + 60,110 }, { 1,1,1,1 }, { 0,1.0f });
		waveMaxNum[i].get()->SetSize({ 80,80 });
	}
	//HP�t���[��
	hpFrame = Sprite::Create(6, { WinApp::window_width / 2,WinApp::window_height-30 }, { 1,1,1,1 }, { 0.5f, 1.0f });
	//hpGauge
	hpGauge = Sprite::Create(7, { WinApp::window_width / 2 - 309.0f / 2,WinApp::window_height - 60 }, { 1,1,1,1 }, { 0.0f, 0.5f });

	//�u�[�X�g�t���[��
	boostFrame = Sprite::Create(8, { WinApp::window_width / 2 - 450,WinApp::window_height / 2 }, { 1,1,1,1 }, { 0.5f, 0.5f });
	//�u�[�X�g�Q�[�W
	boostGauge = Sprite::Create(10, { WinApp::window_width / 2 - 450,WinApp::window_height / 2 + 40 }, { 1,1,1,1 }, { 0.5f, 1.0f });
	//����t���[��
	kaisyuFrame = Sprite::Create(9, { WinApp::window_width / 2 + 450,WinApp::window_height / 2 }, { 1,1,1,1 }, { 0.5f, 0.5f });
	//����Q�[�W
	kaisyuGauge = Sprite::Create(10, { WinApp::window_width / 2 + 450,WinApp::window_height / 2 + 40 }, { 1,1,1,1 }, { 0.5f, 1.0f });


	//�����E�F�[�u�e�L�X�g
	moveWave = Sprite::Create(5, { 0,WinApp::window_height / 2 }, {1,1,1,1} ,{ 0.5f, 0.5f });
	moveWave.get()->SetScale(2.0f);
	//�E�F�[�u��
	for (int i = 0; i < 10; i++) {
		moveWaveNum[i] = Sprite::Create(100 + i, { 110,105 }, { 1,1,1,1 }, { 0.5f,0.5f });
		moveWaveNum[i].get()->SetSize({ 80 * 2,80 * 2 });
	}
	for (int i = 0; i < 10; i++) {
		moveWaveMaxNum[i] = Sprite::Create(100 + i, { 180,105 }, { 1,1,1,1 }, { 0.5f,0.5f });
		moveWaveMaxNum[i].get()->SetSize({ 80 * 2,80 * 2 });
	}
	

	//�`���[�g���A��
	if (MapChip::GetInstance()->nowMap == 0) {
		//����
		for (int i = 0; i < 18; i++) {
			text[i] = Sprite::Create(150 + i/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) }, { 1,1,1,1 }, { 0.5f,0.5f });
			tutorialImag.push_back(std::move(text[i]));
		}
	}
	//X�{�^���������̎���UI
	xButton = Sprite::Create(169, { WinApp::window_width / 2 - 60,WinApp::window_height / 2 + 200 });

	
	//�~�j�}�b�v
	isMinimapDraw = false;
	for (int i = 0; i < MapChip::MAP_NAME::MAX; i++) {
		mapImag[i] = Sprite::Create(20 + i/*+MapChip::GetInstance()->nowMap*/, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) }, { 1,1,1,1 }, { 0.5f,0.5f });;

	}
	playerPos = Sprite::Create(16, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) }, { 1,1,1,1 }, { 0.5f,0.6f });
	playerPos.get()->SetScale(0.5f);
	for (int i = 0; i < 100; i++) {
		enemysPos[i] = Sprite::Create(15, { (float)(WinApp::window_width / 2),(float)(WinApp::window_height / 2) },
			{ 1,1,1,1 }, { 0.5f,0.5f });
		enemysPos[i].get()->SetScale(0.5f);

	}
	

	//���
	recoverColorTimer = 0;
	 

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

	if (Input::GetInstance()->PushKey(DIK_M) ||
		Input::GetInstance()->LongPushPadButton(BUTTON_RIGHT_SHOULDER)) {
		isMinimapDraw = true;
	}
	else {
		isMinimapDraw = false;
	}
	if (isChangeWave) {
		//�ŏ��̕\���ɑҋ@���Ԃ�t����
		if (true) {

		}

		//�����܂�
		if (moveWaveTimer <= 1.0f) {
			movePosX = Ease(Out, Quad, moveWaveTimer, WinApp::window_width + 256*2, WinApp::window_width / 2);
		}
		else if (moveWaveTimer <= 2.0f) {
			movePosX = Ease(In, Quad, moveWaveTimer - 1.0f,  WinApp::window_width / 2,-256*2);
		}
		else {
			isChangeWave = false;
		}
		moveWaveTimer += 0.02f;
	}
	for (int i = 0; i < 3; i++) {
		moveWave.get()->SetPosition({ movePosX,WinApp::window_height/2 });
	}
	for (int i = 0; i < 10; i++) {
		moveWaveNum[i].get()->SetPosition({ movePosX + 50,WinApp::window_height / 2 });
		moveWaveMaxNum[i].get()->SetPosition({ movePosX + 200,WinApp::window_height / 2 });
	}
	
	//Hp�Q�[�W�̊g�k
	float hpGaugeLength = player->hp / player->maxHp *309.0f;
	hpGauge.get()->SetSize({
		hpGaugeLength, 38
		});


	//����Q�[�W�̊g�k
	float kaisyuGaugeLength = (float)((player->recovery.interval - player->recovery.timer) / (float)player->recovery.interval * 162.0f);
	kaisyuGauge.get()->SetSize({
	91,
	kaisyuGaugeLength
		});

	//�̓�����Q�[�W�̊g�k
	float boostGaugeLength = (float)((player->attackCount / 3.0f)  * 162.0f);
	boostGauge.get()->SetSize({
	91 ,
	boostGaugeLength
		});

	//�g���Ȃ��Ƃ�
	if (player->dontRecovery) {
		//recoverGauge.get()->SetColor({ 1,0,0,1 });
		recoverColorTimer++;
		if (recoverColorTimer >= maxRecoverColorTimer) {
			player->dontRecovery = false;
			recoverColorTimer = 0;
			shake = { 0,0 };
			//recoverGauge.get()->SetColor({ 1,1,1,1 });
		}
		else {
			shake = {
				(float)(rand() % (maxRecoverColorTimer - recoverColorTimer) - (maxRecoverColorTimer - recoverColorTimer)/2),
				(float)(rand() % (maxRecoverColorTimer - recoverColorTimer) - (maxRecoverColorTimer - recoverColorTimer) / 2)
			};
		}
		//recoverFrame.get()->SetPosition({ WinApp::window_width - 300+shake.x,290 +shake.y});
		//recoverGauge.get()->SetPosition({ WinApp::window_width - 220+shake.x,290 +shake.y});
	}
	else {
		//recoverGauge.get()->SetColor({ 1,1,1,1 });
		//recoverFrame.get()->SetColor({ 1,1,1,1 });

	}

	//���܂�����
	if (player->recovery.can) {
		//recoverFrame.get()->SetColor({ 1,1,0.5f,1 });
		//recoverGauge.get()->SetColor({ 1,1,0.5f,1 });
	}

	//�`�悪����Ȃ�~�j�}�b�v���X�V
	if (isMinimapDraw) {
		//
		float offsetW = (mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().x / (MapChip::GetInstance()->GetMapData().wide * MapChip::GetInstance()->chipSize));
		float offsetH = (mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().y / (MapChip::GetInstance()->GetMapData().high * MapChip::GetInstance()->chipSize));
		//�v���C���[�|�W�V�����X�V
		playerPos.get()->SetPosition(
			{
				player->pos.x	//�v���C���[�̃|�W�V����
				* offsetW
				+ WinApp::window_width / 2 - mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().x / 2.0f
			,

				-player->pos.z 	//�v���C���[�̃|�W�V����
				* offsetH
				+ WinApp::window_height / 2 - mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().y / 2.0f
			}
		);
		playerPos.get()->SetRotation(player->rotate.y);

		//�G�l�~�[�X�V
		for (int i = 0; i < enemys->enemys[MapChip::GetInstance()->nowMap].size(); i++) {
			enemysPos[i].get()->SetPosition(
				{
				enemys->enemys[MapChip::GetInstance()->nowMap][i]->pos.x
				* offsetW
				+ WinApp::window_width / 2 - mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().x/2.0f
				,

				-enemys->enemys[MapChip::GetInstance()->nowMap][i]->pos.z
				* offsetH
				+ WinApp::window_height / 2 - mapImag[MapChip::GetInstance()->nowMap].get()->GetSize().y / 2.0f
				}
			);
		}
	}
	//�`���[�g���A��
	if (stopFrag && MapChip::GetInstance()->nowMap == 0 && tutorialNum <= tutorialImag.size() - 1) {
		if (*counter % 200 == 0 && tutorialNum < tutorialImag.size() - 1) {
			if (tutorialNum == 5
				|| tutorialNum == 12
				) {
				//��~����
				stopFrag = false;
			}
			else {
				tutorialNum++;

			}

		}
		else if (*counter % 200 == 0) {
			tutorialImag[tutorialNum].get()->SetIsInvisible(true);
			stopFrag = false;

		}
	}

	
}

void UserInterface::Draw() const
{
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	
	frame.get()->Draw(); 
	stageText.get()->Draw();
	stageNum[MapChip::GetInstance()->nowMap].get()->Draw();
	waveText.get()->Draw();
	waveNum[*nowWave + 1].get()->Draw();
	waveMaxNum[enemys->MAX_WAVE[MapChip::GetInstance()->nowMap]].get()->Draw();
	hpFrame.get()->Draw();
	hpGauge.get()->Draw();

	boostFrame.get()->Draw();
	boostGauge.get()->Draw();
	kaisyuFrame.get()->Draw();
	kaisyuGauge.get()->Draw();
	
	moveWave.get()->Draw();
	moveWaveNum[*nowWave + 1].get()->Draw();
	moveWaveMaxNum[enemys->MAX_WAVE[MapChip::GetInstance()->nowMap]].get()->Draw();
	
	//recoverGauge.get()->Draw();
	//recoverFrame.get()->Draw();

	//�}�b�v
	if (isMinimapDraw) {
		mapImag[MapChip::GetInstance()->nowMap].get()->Draw();
		playerPos.get()->Draw();
		for (int i = 0; i < enemys->enemys[MapChip::GetInstance()->nowMap].size(); i++) {
			enemysPos[i].get()->Draw();
		}
	}


	//�`���[�g���A��
	if (MapChip::GetInstance()->nowMap == 0 && stopFrag) {
		tutorialImag[tutorialNum].get()->Draw();
	}
	if (MapChip::GetInstance()->nowMap == 0)
	{
		xButton->Draw();
	}


	Sprite::PostDraw();
}
