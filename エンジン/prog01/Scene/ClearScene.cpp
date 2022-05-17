#include "ClearScene.h"
#include "SceneManager.h"
#include "ObjFactory.h"
#include "Easing.h"

ClearScene::~ClearScene()
{
	Finalize();
}

void ClearScene::Initialize()
{
	// カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera.get());

	// 背景スプライト生成
	sprite = Sprite::Create(36, { 0.0f,0.0f });
	sprite->SetSize({ 1280.0f,720.0f });

	//ライト生成
	light = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, false);

	stageclearObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("stageclear"));
	stageclearObject3d->SetRotation({ -105,-35,0 });
	stageclearObject3d->SetScale({ 75, 1, 15 });
	stageclearObject3d->SetPosition({ 0,0,0 });

	nextStageObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("nextStage"));
	nextStageObject3d->SetRotation({ -90,25,0 });
	nextStageObject3d->SetScale({ 0, 0, 0 });
	nextStageObject3d->SetPosition({ 30,-7,0 });
	nextStageObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });

	clearEscapeObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("clearEscape"));
	clearEscapeObject3d->SetRotation({ -90,25,0 });
	clearEscapeObject3d->SetScale({ 0, 0, 0 });
	clearEscapeObject3d->SetPosition({ 30,-17,0 });

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,-10,-5 });
	camera->SetUp({ 0,1,0 });
}

void ClearScene::Finalize()
{
}

void ClearScene::Update()
{
	Bounce();

	stageclearObject3d->Update();
	nextStageObject3d->Update();
	clearEscapeObject3d->Update();
}

void ClearScene::LastUpdate()
{
}

void ClearScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	sprite->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 3Dオブジェクト描画
	// 3Dオブクジェクトの描画
	Object3d::PreDraw(cmdList);
	stageclearObject3d->Draw();
	nextStageObject3d->Draw();
	clearEscapeObject3d->Draw();
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画

	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void ClearScene::Bounce() {

	if (maxBounceTimer >= bounceTimer) {
		bounceTimer++;
	}

	if (maxBounceTimer <= bounceTimer) {
		OutBack();
	}

	XMFLOAT3 pos = stageclearObject3d->GetPosition();

	float eTime = (float)(bounceTimer / static_cast<double>(maxBounceTimer));

	pos = Ease(Out, ease::Bounce, eTime, { 5,50,0 }, { 5,5,0 });

	stageclearObject3d->SetPosition(pos);
}

void ClearScene::OutBack() {
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

	nextStageObject3d->SetScale(nextStageScale);
	clearEscapeObject3d->SetScale(clearEscapeScale);

	if (maxClearEscapeTimer <= clearEscapeTimer) {
		Select();
	}
}

void ClearScene::Select()
{
	Input* input = Input::GetInstance();

	if (!flag)
	{
		nextStageObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		nextStageObject3d->SetScale({ 10, 1, 5 });
		clearEscapeObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		clearEscapeObject3d->SetScale({ 30, 1, 10 });
	}
	else if (flag)
	{
		nextStageObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		nextStageObject3d->SetScale({ 30, 1, 10 });
		clearEscapeObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		clearEscapeObject3d->SetScale({ 10, 1, 5 });
	}

	if (input->TriggerPadButton(BUTTON_A))
	{
		if (flag)
		{
			SceneManager::GetInstance()->ChangeScene("TitleScene");
		}
		else if (!flag)
		{
			exit(1);
		}
	}

	if (input->TriggerUp() && !isShake || input->PushPadStickUp() && !isShake)
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
		isShake = true;
	}
	else if (input->TriggerDown() && !isShake || input->PushPadStickDown() && !isShake)
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
		isShake = true;
	}

	Shake();
}

void ClearScene::Shake()
{
	Input* input = Input::GetInstance();
	input->SetVibrationPower(5000);

	if (!flag && isShake)
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
			clearEscapeObject3d->SetPosition(shake);
		}
		else if (attenuation >= 6)
		{
			shakeTimer = 0;
			attenuation = 0;
			isShake = false;
			input->SetVibration(false);
			clearEscapeObject3d->SetPosition(savePos);
		}
	}
	else if (flag && isShake)
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
			nextStageObject3d->SetPosition(shake);
		}
		else if (attenuation >= 6)
		{
			shakeTimer = 0;
			attenuation = 0;
			isShake = false;
			input->SetVibration(false);
			nextStageObject3d->SetPosition(savePos);
		}
	}
}