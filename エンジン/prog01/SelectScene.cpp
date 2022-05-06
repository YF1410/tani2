#include "SelectScene.h"
#include "SceneManager.h"

SelectScene::~SelectScene()
{
	Finalize();
}

void SelectScene::Initialize()
{
	// �w�i�X�v���C�g����
	sprite = Sprite::Create(33, { 0.0f,0.0f });
	sprite->SetSize({ 1280.0f,720.0f });
}

void SelectScene::Finalize()
{
}

void SelectScene::Update()
{
	Input* input = Input::GetInstance();

	if (input->TriggerPadButton(BUTTON_A) ||
		input->TriggerKey(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("GameScene",1);
	}

	if (input->TriggerKey(DIK_1)) {
		SceneManager::GetInstance()->ChangeScene("GameScene", 1);
	}

	if (input->TriggerKey(DIK_2)) {
		SceneManager::GetInstance()->ChangeScene("GameScene", 2);
	}

	if (input->TriggerKey(DIK_3)) {
		SceneManager::GetInstance()->ChangeScene("GameScene", 3);
	}

	if (input->TriggerKey(DIK_4)) {
		SceneManager::GetInstance()->ChangeScene("GameScene", 4);
	}

	if (input->TriggerKey(DIK_5)) {
		SceneManager::GetInstance()->ChangeScene("GameScene", 5);
	}
}

void SelectScene::LastUpdate()
{
}

void SelectScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	sprite->Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion �w�i�X�v���C�g�`��
#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�N�W�F�N�g�̕`��
	Object3d::PreDraw(cmdList);

	Object3d::PostDraw();
#pragma endregion 3D�I�u�W�F�N�g�`��
#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��
}
