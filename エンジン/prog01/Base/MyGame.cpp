#include "MyGame.h"
#include "SceneFactory.h"
#include "SpriteFactory.h"
#include "ObjFactory.h"

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
	SceneManager::GetInstance()->ChangeScene("SelectScene");

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