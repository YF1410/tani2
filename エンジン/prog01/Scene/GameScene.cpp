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
#include "ObjFactory.h"


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
	ui = std::make_unique<UserInterface>(&enemyManager->nowWave, playerObject.get(), enemyManager.get());
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

	clearScreen.stateSprite = Sprite::Create(41, { 0,0 });
	clearScreen.selectSprite = Sprite::Create(43, { 0,250 });
	clearScreen.selectSprite->SetSize({ 750, 180 });
	clearScreen.endSprite = Sprite::Create(45, { -80,400 });
	clearScreen.endSprite->SetSize({ 750, 180 });

	gameOverScreen.stateSprite = Sprite::Create(42, { 0,0 });
	gameOverScreen.selectSprite = Sprite::Create(44, { 0,250 });
	gameOverScreen.selectSprite->SetSize({ 750, 180 });
	gameOverScreen.endSprite = Sprite::Create(45, { 0,400 });
	gameOverScreen.endSprite->SetSize({ 750, 180 });

	backSprite = Sprite::Create(40, { 0,0 });
	backSprite->SetSize({ 1280.0f,720.0f });
	backSprite->SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });

	stageclearObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("stageclear"));
	stageclearObject3d->SetRotation({ -50,0,-20 });
	stageclearObject3d->SetScale({ 5000, 5000, 2000 });
	stageclearObject3d->SetPosition({ 0,0,500 });

	nextStageObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("nextStage"));
	nextStageObject3d->SetRotation({ -40,0,15 });
	nextStageObject3d->SetScale({ 0, 0, 0 });
	//nextStageObject3d->SetPosition({ 3000,0,-300 });
	nextStageObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });

	clearEscapeObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("clearEscape"));
	clearEscapeObject3d->SetRotation({ -40,0,15 });
	clearEscapeObject3d->SetScale({ 0, 0, 0 });
	//clearEscapeObject3d->SetPosition({ 3000,0,-1700 });

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

	if (enemyManager.get()->isEndFlag())
	{
		clearFlag = true;
	}
	else if (playerObject.get()->GetEnergy() <= 0)
	{
		gameOverFlag = true;
	}

	playerObject->SetEndFlag(clearFlag, gameOverFlag);

	if (clearFlag) {
		Clear();
	}

	//Select();

	//���C�g�X�V
	light->Update();

	EnemyHelperManager::GetIns()->Update();

	for (auto& object : objects) {
		object->Update();
	}

	//�v���C���[�X�V
	playerObject->Update();
	//�j�ЍX�V
	Debris::StaticUpdate();
	//�G�l�~�[�X�V
	enemyManager.get()->Update();

	stageclearObject3d->Update();
	nextStageObject3d->Update();
	clearEscapeObject3d->Update();

	camera->CameraShake();
}

void GameScene::LastUpdate()
{
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
	//�p�[�e�B�N���S�ẴA�b�v�f�[�g
	ParticleManager::GetInstance()->Update();

	//�J�E���^�[�����Z
	counter++;
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
	enemyManager.get()->Draw();
	playerObject->Draw();

#pragma endregion 3D�I�u�W�F�N�g(FBX)�`��
	ParticleManager::GetInstance()->Draw(cmdList);

#pragma region �p�[�e�B�N��

#pragma endregion �p�[�e�B�N��


#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����

	if (!clearFlag && !gameOverFlag)
	{
		ui.get()->Draw();
	}

	Sprite::PreDraw(cmdList);
	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);

	if (clearFlag || gameOverFlag)
	{
		backSprite->Draw();
	}

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion �O�i�X�v���C�g�`��


	Object3d::PreDraw(cmdList);

	//�������牺�ɏ���
	if (clearFlag)
	{
		stageclearObject3d->Draw();
		nextStageObject3d->Draw();
		clearEscapeObject3d->Draw();
	}
	else if (gameOverFlag)
	{

	}
	Object3d::PostDraw();


	/*if (enemyManager.get()->isEndFlag())
	{
		SceneManager::GetInstance()->ChangeScene("ClearScene");
	}

	if (playerObject.get()->GetEnergy() == 0) {
		SceneManager::GetInstance()->ChangeScene("GameOverScene");
	}
	if (playerObject.get()->GetEnergy() <= 0) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) ||
			Input::GetInstance()->TriggerPadButton(BUTTON_A)) {
			SceneManager::GetInstance()->ChangeScene("TitleScene");
		}
	}*/
}

void GameScene::Select()
{
	//���͍X�V
	Input* input = Input::GetInstance();
	if ((input->TriggerUp() && !shakeTimerFlag) || (input->TriggerDown() && !shakeTimerFlag))
	{
		if (!selectFlag)
		{
			selectFlag = true;
			saveNextStagePos = nextStagePos;
		}
		else if (selectFlag)
		{
			selectFlag = false;
			saveClearEscapePos = clearEscapePos;
		}
		shakeTimerFlag = true;
	}

	if (input->TriggerPadButton(BUTTON_A))
	{
		if (!selectFlag)
		{
			exit(1);
		}
	}

	if (clearFlag)
	{
		if (!selectFlag)
		{
			nextStageObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			nextStageObject3d->SetScale(maxNextStageScale);
			clearEscapeObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			clearEscapeObject3d->SetScale(selectScale);
		}
		else if (selectFlag)
		{
			nextStageObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			nextStageObject3d->SetScale(selectScale);
			clearEscapeObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			clearEscapeObject3d->SetScale(maxClearEscapeScale);
		}
	}
	if (gameOverFlag)
	{
		if (!selectFlag)
		{
			gameOverScreen.selectSprite->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			gameOverScreen.selectSprite->SetPosition({ 0,250 });
			gameOverScreen.selectSprite->SetSize({ 750, 180 });
			gameOverScreen.endSprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			gameOverScreen.endSprite->SetPosition({ 90,400 });
			gameOverScreen.endSprite->SetSize({ 450, 100 });
		}
		else if (selectFlag)
		{
			gameOverScreen.selectSprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			gameOverScreen.selectSprite->SetPosition({ 80,280 });
			gameOverScreen.selectSprite->SetSize({ 450, 100 });
			gameOverScreen.endSprite->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
			gameOverScreen.endSprite->SetPosition({ 0,350 });
			gameOverScreen.endSprite->SetSize({ 750, 180 });
		}
	}

	Shake(input);
}

void GameScene::Clear() {
	if (maxBounceTimer >= bounceTimer) {
		bounceTimer++;
	}
	else if (maxBounceTimer <= bounceTimer) {
		OutBack();
	}

	XMFLOAT3 pos = stageclearObject3d->GetPosition();

	float eTime = (float)(bounceTimer / static_cast<double>(maxBounceTimer));

	Vector3 startPos = playerObject->GetPos();
	startPos += {-700,100,5000};

	Vector3 endPos = playerObject->GetPos();
	endPos += {-700,100,1200};

	pos = Ease(Out, ease::Bounce, eTime, startPos, endPos);

	stageclearObject3d->SetPosition(pos);
}

void GameScene::OutBack() {
	if (maxNextStageScaleTimer >= nextStageScaleTimer) {
		nextStageScaleTimer++;
	}

	if (nextStageScaleTimer >= maxNextStageScaleTimer / 2) {
		if (maxClearEscapeTimer >= clearEscapeTimer) {
			clearEscapeTimer++;
		}
	}

	XMFLOAT3 nextStageScale = nextStageObject3d->GetScale();
	XMFLOAT3 clearEscapeScale = clearEscapeObject3d->GetScale();

	float nextStageETime = (float)(nextStageScaleTimer / static_cast<double>(maxNextStageScaleTimer));
	float clearEscapeETime = (float)(clearEscapeTimer / static_cast<double>(maxClearEscapeTimer));

	nextStageScale = Ease(Out, ease::Back, nextStageETime, { 0,0,0 }, maxNextStageScale);
	clearEscapeScale = Ease(Out, ease::Back, clearEscapeETime, { 0,0,0 }, maxClearEscapeScale);
	
	nextStagePos = playerObject->GetPos();
	clearEscapePos = playerObject->GetPos();
	nextStagePos += {1400, 100, -300};
	clearEscapePos += {1200, 100, -1000};


	nextStageObject3d->SetPosition(nextStagePos);
	clearEscapeObject3d->SetPosition(clearEscapePos);
	nextStageObject3d->SetScale(nextStageScale);
	clearEscapeObject3d->SetScale(clearEscapeScale);

	if (maxClearEscapeTimer <= clearEscapeTimer) {
		Select();
	}
}

void GameScene::Shake(Input *input)
{
	//input->SetVibrationPower(5000);

	if (!selectFlag && shakeTimerFlag)
	{
		XMFLOAT3 shake = {};
		shakeTimer++;

		input->SetVibration(true);

		if (shakeTimer > 0)
		{
			shake.x = (rand() % (100 - attenuation) - 50) + saveClearEscapePos.x;
			shake.y = saveClearEscapePos.y;
			shake.z = (rand() % (100 - attenuation) - 50) + saveClearEscapePos.z;
		}

		if (shakeTimer >= attenuation * 2)
		{
			attenuation += 1;
			clearEscapeObject3d->SetPosition(shake);
		}
		else if (attenuation >= 6)
		{
			shakeTimer = 0;
			attenuation = 0;
			shakeTimerFlag = 0;
			input->SetVibration(false);
			clearEscapeObject3d->SetPosition(saveClearEscapePos);
		}
	}
	else if (selectFlag && shakeTimerFlag)
	{
		XMFLOAT3 shake = {};
		shakeTimer++;
		input->SetVibration(true);

		if (shakeTimer > 0)
		{
			shake.x = (rand() % (100 - attenuation) - 50) + saveNextStagePos.x;
			shake.y = saveNextStagePos.y;
			shake.z = (rand() % (100 - attenuation) - 50) + saveNextStagePos.z;
		}

		if (shakeTimer >= attenuation * 2)
		{
			attenuation += 1;
			nextStageObject3d->SetPosition(shake);
		}
		else if (attenuation >= 6)
		{
			shakeTimer = 0;
			attenuation = 0;
			shakeTimerFlag = 0;
			input->SetVibration(false);
			nextStageObject3d->SetPosition(saveNextStagePos);
		}
	}
}