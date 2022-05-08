#include "MyGame.h"
#include "SceneFactory.h"
#include "SpriteFactory.h"
#include "ObjFactory.h"
#include "Audio.h"

void MyGame::Initialize()
{
	//���N���X�̏�����
	BaseGame::Initialize();

	ObjFactory::GetInstance()->Initialize();

	// �V�[���̏�����
	//�V�[���t�@�N�g���𐶐����A�}�l�[�W���[�ɃZ�b�g
	sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);
	//�V�[���}�l�[�W���[�ɍŏ��̃V�[�����Z�b�g
	SceneManager::GetInstance()->ChangeScene("TitleScene");

	//BGM(0�`9)
	Audio::GetInstance()->LoadWave(0, "Resources/Audio/BGM/title_01.wav");    //�^�C�g��
	Audio::GetInstance()->LoadWave(1, "Resources/Audio/BGM/select_01.wav");   //�Z���N�g�V�[��
	Audio::GetInstance()->LoadWave(2, "Resources/Audio/BGM/tutorial_01.wav"); //�X�e�[�W0
	Audio::GetInstance()->LoadWave(3, "Resources/Audio/BGM/game_01.wav");     //�X�e�[�W1
	Audio::GetInstance()->LoadWave(4, "Resources/Audio/BGM/game_02.wav");     //�X�e�[�W2
	Audio::GetInstance()->LoadWave(5, "Resources/Audio/BGM/game_03.wav");     //�X�e�[�W3
	Audio::GetInstance()->LoadWave(6, "Resources/Audio/BGM/game_04.wav");     //�X�e�[�W4
	Audio::GetInstance()->LoadWave(7, "Resources/Audio/BGM/game_05.wav");     //�X�e�[�W5
	Audio::GetInstance()->LoadWave(8, "Resources/Audio/BGM/gameover_01.wav"); //�Q�[���I�[`�o�[
	Audio::GetInstance()->LoadWave(9, "Resources/Audio/BGM/clear_01.wav");    //�N���A
	//SE(10�`16)
	Audio::GetInstance()->LoadWave(10, "Resources/Audio/SE/select_01.wav");     //�u�[�X�g���
	Audio::GetInstance()->LoadWave(11, "Resources/Audio/SE/damage_01.wav");   //�_���[�W1
	Audio::GetInstance()->LoadWave(12, "Resources/Audio/SE/damage_02.wav");   //�_���[�W2
	Audio::GetInstance()->LoadWave(13, "Resources/Audio/SE/heal_01.wav");     //��
	Audio::GetInstance()->LoadWave(14, "Resources/Audio/SE/recovery_01.wav"); //���
	Audio::GetInstance()->LoadWave(15, "Resources/Audio/SE/select_01.wav");   //�I����
	Audio::GetInstance()->LoadWave(16, "Resources/Audio/SE/Select_02.wav");   //����
	Audio::GetInstance()->LoadWave(17, "Resources/Audio/SE/wall_01.wav");     //�ǂɓ���������


	SpriteFactory::GetInstance()->Initialize();
}

void MyGame::Finalize()
{
	// �V�[���̏I��
	SceneManager::GetInstance()->Finalize();
	ObjFactory::GetInstance()->Finalize();
	//���N���X�̉��
	BaseGame::Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V
	BaseGame::Update();
}

void MyGame::Draw()
{
	BaseGame::Draw();
}