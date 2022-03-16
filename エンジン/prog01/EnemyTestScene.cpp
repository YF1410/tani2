#include "EnemyTestScene.h"
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

using namespace DirectX;

EnemyTestScene::EnemyTestScene() {
	//���f���}�l�[�W���[�ɑS���f�����[�h
	ModelManager::GetIns()->Initialize();
}

EnemyTestScene::~EnemyTestScene() {
	Finalize();
}

void EnemyTestScene::Initialize() {


	collisionManager = CollisionManager::GetInstance();

	// �J��������
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera.get());
	// FBX�I�u�W�F�N�g�ɃJ�������Z�b�g
	FbxObject3d::SetCamera(camera.get());

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) 	{
		assert(0);
	}
	// �f�o�b�O�e�L�X�g������
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png")) 	{
		assert(0);
	}

	// �w�i�X�v���C�g����
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetSize({ 100.0f,100.0f });
	sprite->SetPosition({ 100.0f,100.0f });

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());

	//���C�g����
	light = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, true);
	light->SetDirLightActive(2, true);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, true);

	// ���f���ǂݍ���
	modelSphere = Model::CreateFromObject("sphere", true);

	// 3D�I�u�W�F�N�g����
	enemyObject = std::make_unique<Enemy>();

	//���f���e�[�u��
	modelPlane = Model::CreateFromObject("cube");
	Model* modeltable = modelPlane.get();

	//�T�E���h�Đ�
	Audio::GetInstance()->LoadWave(0, "Resources/Alarm01.wav");

	// �J���������_���Z�b�g
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,500,-500 });
	camera->SetUp({ 0,1,0 });

	//Debris::StaticInit();
	//playerObject->Init();
}

void EnemyTestScene::Finalize() {
}

void EnemyTestScene::Update() {
	Input* input = Input::GetInstance();
	light->Update();
	camera->Update();
	particleMan->Update();


	DebugText::GetInstance()->VariablePrint(0, 0, "angle", enemyObject->GetAngle(), 3);

	if (input->TriggerKey(DIK_C)) 	{
		SceneManager::GetInstance()->ChangeScene("ClearScene");
	}
	else if (input->TriggerKey(DIK_B)) 	{
		SceneManager::GetInstance()->ChangeScene("GameOverScene");
	}


	for (auto& object : objects) {
		object->Update();
	}
	//�v���C���[�X�V
	//playerObject->Update();
	//�j�ЍX�V
	Debris::StaticUpdate();

	//fbxObject3d->Update();
	enemyObject->Update();
	// �S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();
}

void EnemyTestScene::Draw() {
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
	/*for (auto& object : objects) {
		object->Draw();
	}*/
	enemyObject->Draw();
	Object3d::PostDraw();
#pragma endregion 3D�I�u�W�F�N�g�`��

#pragma region 3D�I�u�W�F�N�g(FBX)�`��
	//playerObject->Draw();
	Debris::StaticDraw();
#pragma endregion 3D�I�u�W�F�N�g(FBX)�`��
#pragma region �p�[�e�B�N��
	// �p�[�e�B�N���̕`��
	particleMan->Draw(cmdList);
#pragma endregion �p�[�e�B�N��
#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��
}