#include "TitleScene.h"
#include "SceneManager.h"
#include "ObjFactory.h"
#include "Ease.h"

TitleScene::~TitleScene()
{
	Finalize();
}

void TitleScene::Initialize()
{
	// �J��������
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera.get());

	// �w�i�X�v���C�g����
	sprite = Sprite::Create(36, { 0.0f,0.0f });
	sprite->SetSize({ 1280.0f,720.0f });

	//���C�g����
	light = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, false);

	titleObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("title"));
	titleObject3d->SetRotation({ -105,-35,0 });
	titleObject3d->SetScale({ 75, 1, 15 });
	titleObject3d->SetPosition({ 5,5,0 });

	startObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("start"));
	startObject3d->SetRotation({ -90,25,0 });
	startObject3d->SetScale({ 30, 1, 10 });
	startObject3d->SetPosition({ 30,-7,0 });
	startObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });

	endObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("end"));
	endObject3d->SetRotation({ -90,25,0 });
	endObject3d->SetScale({ 10, 1, 5 });
	endObject3d->SetPosition({ 30,-17,0 });

	// �J���������_���Z�b�g
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,-10,-5 });
	camera->SetUp({ 0,1,0 });
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	if (input->TriggerPadButton(BUTTON_A))
	{
		if (flag)
		{
			SceneManager::GetInstance()->ChangeScene("ClearScene");
		}
		else if (!flag)
		{
			exit(1);
		}
	}

	if (input->TriggerUp() && !shakeTimerFlag)
	{
		if (!flag)
		{
			flag = true;
			savePos = { 30,-7,0 };
		}
		else if (flag)
		{
			flag = false;
			savePos = { 30,-17,0 };
		}
		shakeTimerFlag = true;
	}
	else if (input->TriggerDown() && !shakeTimerFlag)
	{
		if (!flag)
		{
			flag = true;
			savePos = { 30,-7,0 };
		}
		else if (flag)
		{
			flag = false;
			savePos = { 30,-17,0 };
		}
		shakeTimerFlag = true;
	}

	Select();

	titleObject3d->Update();
	startObject3d->Update();
	endObject3d->Update();
}

void TitleScene::LastUpdate()
{
}

void TitleScene::Draw()
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
	titleObject3d->Draw();
	startObject3d->Draw();
	endObject3d->Draw();
	Object3d::PostDraw();
#pragma endregion 3D�I�u�W�F�N�g�`��
#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��
}

void TitleScene::Select()
{
	if (!flag)
	{
		startObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		startObject3d->SetScale({ 10, 1, 5 });
		endObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		endObject3d->SetScale({ 30, 1, 10 });
	}
	else if (flag)
	{
		startObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		startObject3d->SetScale({ 30, 1, 10 });
		endObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		endObject3d->SetScale({ 10, 1, 5 });
	}

	Shake();
}

void TitleScene::Shake()
{
	Input* input = Input::GetInstance();

	if (!flag && shakeTimerFlag)
	{
		XMFLOAT3 shake = {};
		shakeTimer++;

		input->SetVibration(true);

		if (shakeTimer > 0)
		{
			shake.x = (rand() % (7 - attenuation) - 3) + savePos.x;
			shake.y = (rand() % (7 - attenuation) - 3) + savePos.y;
			shake.z = savePos.z;
		}

		if (shakeTimer >= attenuation * 2)
		{
			attenuation += 1;
			endObject3d->SetPosition(shake);
		}
		else if (attenuation >= 6)
		{
			shakeTimer = 0;
			attenuation = 0;
			shakeTimerFlag = 0;
			input->SetVibration(false);
			endObject3d->SetPosition(savePos);
		}
	}
	else if (flag && shakeTimerFlag)
	{
		XMFLOAT3 shake = {};
		shakeTimer++;
		input->SetVibration(true);

		if (shakeTimer > 0)
		{
			shake.x = (rand() % (7 - attenuation) - 3) + savePos.x;
			shake.y = (rand() % (7 - attenuation) - 3) + savePos.y;
			shake.z = savePos.z;
		}

		if (shakeTimer >= attenuation * 2)
		{
			attenuation += 1;
			startObject3d->SetPosition(shake);
		}
		else if (attenuation >= 6)
		{
			shakeTimer = 0;
			attenuation = 0;
			shakeTimerFlag = 0;
			input->SetVibration(false);
			startObject3d->SetPosition(savePos);
		}
	}
}