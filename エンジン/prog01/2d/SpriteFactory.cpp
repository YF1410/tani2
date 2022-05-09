#include "SpriteFactory.h"

void SpriteFactory::Initialize()
{
	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(0, L"Resources/debugfont.png"))
	{
		assert(0);
	}
	// �f�o�b�O�e�L�X�g������
	DebugText::GetInstance()->Initialize(0);

	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(1, L"Resources/Sprite/APEX_01.png"))
	{
		assert(0);
	}

	//UI����
	if (!Sprite::LoadTexture(5, L"Resources/Sprite/UI/wave.png")) {
		assert(0);
	}
	//HP
	if (!Sprite::LoadTexture(6, L"Resources/Sprite/UI/playerHp.png")) {
		assert(0);
	}
	//Enemy
	if (!Sprite::LoadTexture(7, L"Resources/Sprite/UI/enemyNum.png")) {
		assert(0);
	}
	//���
	if (!Sprite::LoadTexture(8, L"Resources/Sprite/UI/recover/recovery.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(9, L"Resources/Sprite/UI/recover/recoveryGauge.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(10, L"Resources/Sprite/UI/recover/recovery1.png")) {
		assert(0);
	}

	//�̓�����
	if (!Sprite::LoadTexture(20, L"Resources/Sprite/UI/exGauge/expGaugeEmpty.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(21, L"Resources/Sprite/UI/exGauge/expGauge1.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(22, L"Resources/Sprite/UI/exGauge/expGauge2.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(23, L"Resources/Sprite/UI/exGauge/expGauge3.png")) {
		assert(0);
	}


	//scene�̉摜
	if (!Sprite::LoadTexture(33, L"Resources/Sprite/stageSelect.png")) {
		assert(0);
	}

	if (!Sprite::LoadTexture(35, L"Resources/Sprite/stage.png")) {
		assert(0);
	}
	if (!Sprite::LoadTexture(36, L"Resources/Sprite/background.png")) {
		assert(0);
	}

	//�N���A�A�Q�[���I�[�o�[���̉摜
	if (!Sprite::LoadTexture(40, L"Resources/Sprite/back.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(41, L"Resources/Sprite/gameclear.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(42, L"Resources/Sprite/gameover.png"))
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

	//����
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

	//SelectScene�̔w�i
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


}

SpriteFactory* SpriteFactory::GetInstance()
{
	static SpriteFactory instance;
	return &instance;
}