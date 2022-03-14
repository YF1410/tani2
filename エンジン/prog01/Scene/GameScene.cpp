#include "GameScene.h"
#include "Collision.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "ContactableObject.h"
#include "SceneManager.h"

using namespace DirectX;

GameScene::~GameScene()
{
	Finalize();
}

void GameScene::Initialize()
{
	collisionManager = CollisionManager::GetInstance();

	// �J��������
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera.get());
	// FBX�I�u�W�F�N�g�ɃJ�������Z�b�g
	FbxObject3d::SetCamera(camera.get());

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png"))
	{
		assert(0);
	}
	// �f�o�b�O�e�L�X�g������
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png"))
	{
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
	modelFighter = Model::CreateFromObject("chr_sword");
	modelSphere = Model::CreateFromObject("sphere", true);

	// 3D�I�u�W�F�N�g����
	playerObject = std::make_unique<PlayerObject>(modelFighter.get(), modelSphere.get());

	//���f���e�[�u��
	modelPlane = Model::CreateFromObject("cube");
	Model* modeltable = modelPlane.get();

	MapChip::GetInstance()->CsvLoad(26, 20, "Resources/map.csv");

	const float LAND_SCALE = 2.0f;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 26; j++) {

			if (MapChip::GetInstance()->GetChipNum(j, i, 1, 0))
			{
				ContactableObject* object = ContactableObject::Create(modeltable);
				object->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
				object->SetPosition({ j * LAND_SCALE, (i * -LAND_SCALE) + 30, 0 });
				objects.push_back(std::unique_ptr<Object3d>(object));
			}
		}
	}

	//.fbx�̖��O���w�肵�ă��f����ǂݍ���
	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("prin");
	// FBX�I�u�W�F�N�g����
	fbxObject3d = FbxObject3d::Create(fbxModel.get(), true);
	//�A�j���[�V����
	fbxObject3d->PlayAnimation(0);

	fbxObject3d->SetScale({ 0.1f, 0.1f, 0.1f });

	//�T�E���h�Đ�
	Audio::GetInstance()->LoadWave(0, "Resources/Alarm01.wav");

	// �J���������_���Z�b�g
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,1,-150 });
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{
	Input* input = Input::GetInstance();
	light->Update();
	camera->Update();
	particleMan->Update();

	// �I�u�W�F�N�g�ړ�
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		// �ړ���̍��W���v�Z
		if (input->PushKey(DIK_W))
		{
			fighterPos[1] += 0.1f;
		}
		else if (input->PushKey(DIK_S))
		{
			fighterPos[1] -= 0.1f;
		}

		if (input->PushKey(DIK_D))
		{
			fighterPos[0] += 0.1f;
		}
		else if (input->PushKey(DIK_A))
		{
			fighterPos[0] -= 0.1f;
		}
	}

	if (input->TriggerKey(DIK_SPACE))
	{
		if (flag)
		{
			fbxObject3d->PlayAnimation(0);
			flag = false;
		}
		else if (!flag)
		{
			fbxObject3d->PlayAnimation(1);
			flag = true;
		}
	}

	DebugText::GetInstance()->VariablePrint(0, 0, "angle", input->PadStickAngle(), 3);

	/*XMFLOAT3 rot = fbxObject3d->GetRotation();
	rot.y += 1.0f;
	fbxObject3d->SetRotation(rot);*/

	if (input->TriggerKey(DIK_C))
	{
		SceneManager::GetInstance()->ChangeScene("ClearScene");
	}
	else if (input->TriggerKey(DIK_B))
	{
		SceneManager::GetInstance()->ChangeScene("GameOverScene");
	}


	for (auto& object : objects) {
		object->Update();
	}
	playerObject->Update();
	fbxObject3d->Update();
	// �S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();
}

void GameScene::Draw()
{
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
	}
	playerObject->Draw();*/
	Object3d::PostDraw();
#pragma endregion 3D�I�u�W�F�N�g�`��
#pragma region 3D�I�u�W�F�N�g(FBX)�`��
	fbxObject3d->Draw(cmdList);
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