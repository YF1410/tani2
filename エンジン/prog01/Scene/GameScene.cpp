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

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
	}
	// �f�o�b�O�e�L�X�g������
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png")) {
		assert(0);
	}

	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(1, L"Resources/Sprite/weve1.png")) {
		assert(0);
	}
	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(2, L"Resources/Sprite/weve2.png")) {
		assert(0);
	}
	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(3, L"Resources/Sprite/weve3.png")) {
		assert(0);
	}

	// �w�i�X�v���C�g����
	for (int i = 1; i < 4; i++)
	{
		std::unique_ptr<Sprite> tempsprite = Sprite::Create(i, { 0.0f,0.0f });
		tempsprite->SetSize({ 600.0f,200.0f });
		tempsprite->SetPosition({ 1280.0f,250.0f });
		weveSprite.push_back(std::move(tempsprite));
	}

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());

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
	camera->SetEye({ 0,1600,-500 });
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
	Vector3 camEye = camera.get()->GetEye();
	float debrisLengthMax = 0.0f;
	for (int i = 0; i < Debris::debris.size(); i++) {
		if (Debris::debris[i]->isFirstAttack &&
			debrisLengthMax <= Vector3(Debris::debris[i]->pos - playerObject.get()->GetPos()).Length()) {
			debrisLengthMax = Vector3(Debris::debris[i]->pos - playerObject.get()->GetPos()).Length();
		}

		if (Debris::debris[i]->state == Debris::RETURN &&
			debrisLengthMax <= Vector3(Debris::debris[i]->pos - playerObject.get()->GetPos()).Length()) {
			debrisLengthMax = Vector3(Debris::debris[i]->pos - playerObject.get()->GetPos()).Length();
		}
	}

	const float velocityOffset = 17.0f;
	//�J�����̃C�[�W���O����
	eyeDistance = Ease(Out, Quad, 0.05f,
		camera.get()->GetEye(),
		Vector3(playerObject.get()->GetPos() +
			eyeDistanceDef +
			Vector3(0, debrisLengthMax * 0.7f, 0)
		));
	camera->SetEye(eyeDistance);
	//�v���C���[�̏�������œ_�ɂ���
	targetDistance = Ease(Out, Quad, 0.05f,
		camera.get()->GetTarget(),
		Vector3(playerObject.get()->GetPos() + 
			targetDistanceDef));
	camera->SetTarget(targetDistance);

	camera->Update();

	//�}�b�v�`�b�v�X�V
	//MapChip::GetInstance()->Update(MapChip::TEST_MAP);


	//���͍X�V
	Input *input = Input::GetInstance();
	
	//���C�g�X�V
	light->Update();
	//�X�e�[�W�X�V
	//testStage->Update();

	if (!EnemyManager::GetIns()->isEnemyAddFlag() && counter % 1200 == 0 && flag && weveCount < 3)
	{
		showingFlag = true;
		EnemyManager::GetIns()->SetEnemyAddFlag(true);
		weveCount++;
	}
	else if (!EnemyManager::GetIns()->isEnemyAddFlag() && counter <= 150 && weveCount <= -1)
	{
		showingFlag = true;
		weveCount++;
	}
	else if (!EnemyManager::GetIns()->isEnemyAddFlag() && counter >= 150 && weveCount <= 0 && !flag)
	{
		EnemyManager::GetIns()->SetEnemyAddFlag(true);
		flag = true;
	}

	if (weveCount < 3 && showingFlag)
	{
		weveStartTimer++;
		float timeRate = 0.0f;
		int countNum = 125;
		upTimer++;

		timeRate = upTimer / countNum;
		if (!changeFlag && !endFlag)
		{
			weveSprite[weveCount]->SetPosition(Ease::easeOut({ 1280.0f,250.0f }, { 350.0f,250.0f }, timeRate));
			if (upTimer > countNum)
			{
				upTimer = 0;
				changeFlag = true;
			}
		}
		else if (changeFlag && !endFlag)
		{
			weveSprite[weveCount]->SetPosition(Ease::easeIn({ 350.0f,250.0f }, { -1280.0f,250.0f }, timeRate));
			if (upTimer > countNum)
			{
				upTimer = 0;
				changeFlag = false;
				endFlag = true;
			}
		}

		if (weveStartTimer >= 300)
		{
			weveStartTimer = 0;
			upTimer = 0;
			endFlag = false;
			showingFlag = false;
		}
	}
	
	//�f�o�b�N�e�L�X�g

	particleMan->Update();

	for (auto& object : objects) {
		object->Update();
	}

	//�v���C���[�X�V
	playerObject->Update();
	//�j�ЍX�V
	Debris::StaticUpdate();
	//�G�l�~�[�X�V
	EnemyManager::GetIns()->Update();


	float contact_pos = 0.0f;

	// �S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckBroadCollisions();

	/*line.startPos = Enemy::enemys[0]->GetPos();
	line.endPos = playerObject->GetPos();
	aabb.length = { 200,0,200 };

	for (int z = 0; z < 34; z++) {
		for (int x = 0; x < 57; x++) {
			if (MapChip::GetInstance()->GetChipNum(x, z) == 1) {
				aabb.center = { 200.0f * x - 100.0f ,0.0f,-200.0f * z + 100.0f };
				if (Collision::CheckLine2Box(line, aabb)) {
					DebugText::GetInstance()->Print("hit", 0, 80, 3);
				}
			}
		}
	}*/
	 
	//�ŏI�X�V
	EnemyManager::GetIns()->FinalUpdate();
	playerObject.get()->FinalUpdate();

	//�S�Ă̈ړ��ŏI�K������
	playerObject.get()->Adaptation();
	Debris::StaticAdaptation();
	EnemyManager::GetIns()->Adaptation();
	MapChip::GetInstance()->Adaptation();

	if (playerObject.get()->GetHp() == 0) {
		DebugText::GetInstance()->Print("Game Over", 0, 240, 5);
	}
	DebugText::GetInstance()->VariablePrint(0, 180, "weveCount", weveCount, 3);

	//�J�E���^�[�����Z
	counter++;
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
	//testStage->Draw(DirectXCommon::GetInstance()->GetCommandList());
	MapChip::GetInstance()->Draw();
	Debris::StaticDraw();
	EnemyManager::GetIns()->Draw();
	playerObject->Draw();
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