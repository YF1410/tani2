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
#include "EnemyHelperManager.h"
#include "Ease.h"

using namespace DirectX;

int GameScene::counter;


GameScene::GameScene(int parameter) {
	//�X�e�[�W�Z�b�g
	nowStageNum = parameter;
	//�}�b�v����
	MapChip::GetInstance()->SetMapName((MapChip::MAP_NAME)nowStageNum);
	MapChip::GetInstance()->CreateStage();


	//�v���C���[����
	playerObject = std::make_unique<PlayerObject>(MapChip::GetInstance()->GetStartPos());
	enemyManager = std::make_unique<EnemyManager>(playerObject.get());
	//UI����
	ui = std::make_unique<UserInterface>(&enemyManager->nowWave,playerObject.get(),enemyManager.get());
	//�w�i�Z�b�g

	//�J��������
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// �J���������_���Z�b�g
	camera->SetTarget(Vector3(playerObject.get()->GetPos() + targetDistanceDef));
	camera->SetEye(Vector3(playerObject.get()->GetPos() + eyeDistanceDef));
	camera->SetUp({ 0,1,0 });
}

GameScene::~GameScene() {
	Finalize();
}

void GameScene::Initialize() {

	//�o�߃t���[����0��
	counter = 0;

	collisionManager = CollisionManager::GetInstance();


	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera.get());
	// FBX�I�u�W�F�N�g�ɃJ�������Z�b�g
	FbxObject3d::SetCamera(camera.get());
	//
	ParticleEmitter::SetCamera(camera.get());


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

	light->SetDirLightDir(0, Vector3(0, -1, -0.4).Normal());


	//�v���C���[�̏�����
	playerObject->Initialize();

	//�G�l�~�[�Ƀv���C���[�̏���n��
	enemyManager.get()->Initialize();


	sprite = Sprite::Create(35, { 0,0 });
	sprite->SetSize({ 1280.0f,720.0f });

	//�f�u�����Z�b�g
	Debris::StaticInitialize(playerObject.get());

	//UI�̏�����
	ui->Initialize();
}

void GameScene::Finalize() {
	Debris::Finalize();
	MapChip::GetInstance()->Filnalize();
	ParticleManager::GetInstance()->Finalize();
}

void GameScene::Update() {
	//�J�����X�V
		//�v���C���[�̏�������œ_�ɂ���
		//�J�����X�V
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
	if (debrisLengthMax > 5000) {
		debrisLengthMax = 5000;
	}

	const float velocityOffset = 17.0f;
	//�J�����̃C�[�W���O����
	eyeDistance = Ease(Out, Quad, 0.05f,
		camera.get()->GetEye(),
		Vector3(playerObject.get()->GetPos() +
			eyeDistanceDef +
			Vector3(0, debrisLengthMax * 0.7f, 0) +
			playerObject.get()->velocity * velocityOffset
		));
	camera->CameraMoveEyeVector(Vector3(eyeDistance - Vector3(camera.get()->GetEye())));
	//�v���C���[�̏�������œ_�ɂ���
	targetDistance = Ease(Out, Quad, 0.05f,
		camera.get()->GetTarget(),
		Vector3(playerObject.get()->GetPos() +
			targetDistanceDef +
			playerObject.get()->velocity * velocityOffset));

	camera->CameraMoveTargetVector(Vector3(targetDistance - Vector3(camera.get()->GetTarget())));

	camera->Update();


	//if (Input::GetInstance()->TriggerKey(DIK_1)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new Enemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_2)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new CushionEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));

	//	}
	//}
	//if (Input::GetInstance()->TriggerKey(DIK_3)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new BoundEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_4)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new AvoidanceEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_5)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new RandomMoveEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_6)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new EscapeEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_7)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new DefenseEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_8)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new KiteEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_9)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new SuctionEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_0)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new GetawayEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_P)) {
	//	for (int i = 0; i < 3; i++) {
	//		EnemyManager::enemys.push_back(new RouteMoveEnemy(Vector3(playerObject.get()->pos + playerObject.get()->velocity.Normal() * 1500.0f), playerObject.get()));
	//	}
	//}

	//�}�b�v�`�b�v�X�V
	//MapChip::GetInstance()->Update(MapChip::TEST_MAP);


	//���͍X�V
	Input* input = Input::GetInstance();

	//���C�g�X�V
	light->Update();

	EnemyHelperManager::GetIns()->Update();
	EnemyHelperManager::GetIns()->LustUpdate();
	//�X�e�[�W�X�V
	//testStage->Update();

	//�f�o�b�N�e�L�X�g

	
	//�v���C���[�X�V
	playerObject->Update();
	//�j�ЍX�V
	Debris::StaticUpdate();
	//�G�l�~�[�X�V
	enemyManager.get()->Update();


	float contact_pos = 0.0f;

	// �S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckBroadCollisions();

	//�ŏI�X�V
	ui.get()->Update();
	enemyManager.get()->FinalUpdate();
	playerObject.get()->LustUpdate();
	Debris::StaticLustUpdate();
	//�S�Ă̈ړ��ŏI�K������
	playerObject.get()->Adaptation();
	Debris::StaticAdaptation();
	enemyManager.get()->Adaptation();
	MapChip::GetInstance()->Adaptation();

	if (playerObject.get()->GetEnergy() <= 0) {
		DebugText::GetInstance()->Print("Game Over", 0, 240, 5);
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)||
			Input::GetInstance()->TriggerPadButton(BUTTON_A)){

		}
	}
	//�p�[�e�B�N���S�ẴA�b�v�f�[�g
	ParticleManager::GetInstance()->Update();

	//�J�E���^�[�����Z
	counter++;

	camera->CameraShake();

}

void GameScene::LastUpdate() {

	if (enemyManager.get()->isEndFlag())
	{
		sceneChange.SceneChangeStart("ClearScene");
	}

	if (playerObject.get()->GetEnergy() == 0) {
		SceneManager::GetInstance()->ChangeScene("GameOverScene");
	}
	if (playerObject.get()->GetEnergy() <= 0) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) ||
			Input::GetInstance()->TriggerPadButton(BUTTON_A)) {
			sceneChange.SceneChangeStart("TitleScene");
		}
	}

	sceneChange.Update();

}

void GameScene::Draw() {
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

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


#pragma region 3D�I�u�W�F�N�g(FBX)�`��
	//testStage->Draw(DirectXCommon::GetInstance()->GetCommandList());
	EnemyHelperManager::GetIns()->Draw();
	MapChip::GetInstance()->Draw();
	Debris::StaticDraw();
	playerObject->Draw();
	enemyManager.get()->Draw();
	
#pragma endregion 3D�I�u�W�F�N�g(FBX)�`��
	ParticleManager::GetInstance()->Draw(cmdList);

#pragma region �p�[�e�B�N��

#pragma endregion �p�[�e�B�N��


#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	ui.get()->Draw();
	Sprite::PreDraw(cmdList);
	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);
	

	//�V�[���J�ڗp
	sceneChange.Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��


}