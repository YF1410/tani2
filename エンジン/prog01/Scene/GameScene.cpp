#include "GameScene.h"
#include "Collision.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "CollisionManager.h"
#include "ContactableObject.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Debris.h"
#include "MapChip.h"
#include "Easing.h"
#include "EnemyManager.h"
#include "Ease.h"

using namespace DirectX;

int GameScene::counter;


GameScene::GameScene() {
	MapChip::GetInstance()->Initialize();
	//�}�b�v����
	MapChip::GetInstance()->SetMapName(MapChip::TEST_MAP);
	MapChip::GetInstance()->CreateStage();


	//�v���C���[����
	playerObject = std::make_unique<PlayerObject>(MapChip::GetInstance()->GetStartPos());
}

GameScene::~GameScene() {
	Finalize();
}

void GameScene::Initialize() {

	//�o�߃t���[����0��
	counter = 0;

	collisionManager = CollisionManager::GetInstance();

	// �J��������
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera.get());
	// FBX�I�u�W�F�N�g�ɃJ�������Z�b�g
	FbxObject3d::SetCamera(camera.get());

	// �w�i�X�v���C�g����
	for (int i = 2; i < 5; i++)
	{
		std::unique_ptr<Sprite> tempsprite = Sprite::Create(i, { 0.0f,0.0f });
		tempsprite->SetSize({ 600.0f,200.0f });
		tempsprite->SetPosition({ 1280.0f,250.0f });
		weveSprite.push_back(std::move(tempsprite));
	}

	// �p�[�e�B�N���}�l�[�W������
	//particleMan = 
	particleMan = particleMan->Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());
	particleMan2 = particleMan2->Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());

	particle = particle->Create();
	particle2 = particle2->Create();

	particle->SetParticleManager(particleMan.get(), L"APEX_01");
	particle2->SetParticleManager(particleMan2.get());

	//���C�g����
	light = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light.get());
	FbxObject3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, true);

	light->SetDirLightDir(0, Vector3(0,-1,-0.4).Normal());

	// �J���������_���Z�b�g
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,0,-10 });
	camera->SetUp({ 0,1,0 });
	
	//�v���C���[�̏�����
	playerObject->Initialize();

	//�G�l�~�[�Ƀv���C���[�̏���n��
	EnemyManager::GetIns()->SetPlayer(playerObject.get());


	//�f�u�����Z�b�g
	Debris::StaticInitialize(playerObject.get());
}

void GameScene::Finalize() {
}

void GameScene::Update() {
	//�J�����X�V
	//�v���C���[�̏�������œ_�ɂ���
	//�J�����X�V
	
	//�f�o�b�N�e�L�X�g
	particle->Add();
	particle2->Add();

	particle->Update();
	particle2->Update();
}

void GameScene::LastUpdate()
{
}

void GameScene::Draw() {
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	//sprite->Draw();
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


#pragma region 3D�I�u�W�F�N�g(FBX)�`��
	
#pragma endregion 3D�I�u�W�F�N�g(FBX)�`��


#pragma region �p�[�e�B�N��
	// �p�[�e�B�N���̕`��
	particle->Draw(cmdList);
	particle2->Draw(cmdList);
#pragma endregion �p�[�e�B�N��


#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);
	if (weveStartTimer > 0)
	{
		weveSprite[weveCount]->Draw();
	}
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��

	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_C)) {
		SceneManager::GetInstance()->ChangeScene("EnemyTestScene");
	}
	else if (input->TriggerKey(DIK_B)) {
		SceneManager::GetInstance()->ChangeScene("PlayerTestScene");
	}
}