#include "TitleScene.h"
#include "SceneManager.h"
#include "ObjFactory.h"
#include "Ease.h"
#include "Audio.h"
#include "ModelManager.h"
#include "SlimeMath.h"

TitleScene::~TitleScene() {
	Finalize();
}

void TitleScene::Initialize() {
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

	titleObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("title"));
	titleObject3d->SetRotation({ -105,-35,0 });
	titleObject3d->SetScale({ 75, 1, 15 });
	titleObject3d->SetPosition(titleObjectPosition);

	startObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("start"));
	startObject3d->SetRotation({ -90,25,0 });
	startObject3d->SetScale({ 30, 1, 10 });
	startObject3d->SetPosition(startObjectPosition);
	startObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });

	endObject3d = Object3d::Create(ObjFactory::GetInstance()->GetModel("end"));
	endObject3d->SetRotation({ -90,25,0 });
	endObject3d->SetScale({ 10, 1, 5 });
	endObject3d->SetPosition(endObjectPosition);

	FbxObject3d::SetCamera(camera.get());
	FbxObject3d::SetLight(light.get());
	player = std::make_unique<GameObjCommon>(ModelManager::PLAYER, GameObjCommon::Notag, false, Vector3{ 0,0,100 }, Vector3{0.1f,0.1f,0.1f});
	player.get()->velocity = { 1,1,0 };
	player.get()->velocity.Normalize();
	player.get()->velocity*= 0.5f;
	player.get()->rotate.x = -50.0f;
	//player.get()->rotate.y = 180.0f;
	//player.get()->rotate.z = 90.0f;

	Audio::GetInstance()->LoopPlayWave(0, 0.5f);

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,-10,-5 });
	camera->SetUp({ 0,1,0 });
	sceneChange.type = SceneChange::FADE_IN;
	//やぶなか
}

void TitleScene::Finalize() {
}

void TitleScene::Update() {
	Input* input = Input::GetInstance();

	if ((input->TriggerPadButton(BUTTON_A) ||
		input->TriggerKey(DIK_SPACE))
		&& !sceneChangeFlag) {
		if (flag) {
			Audio::GetInstance()->PlayWave(16);
			sceneChangeFlag = true;
		}
		else if (!flag) {
			Audio::GetInstance()->PlayWave(16);
			exit(1);
		}
	}

	if ((input->TriggerUp() || input->TriggerDown() || input->TriggerPadStickUp() || input->TriggerPadStickDown()
		|| input->TriggerKey(DIK_UP) || input->TriggerKey(DIK_DOWN) || input->TriggerKey(DIK_W) || input->TriggerKey(DIK_S)) && !isShake && !sceneChangeFlag) {
		if (!flag) {
			flag = true;
			savePos = { 30,-7,0 };
		}
		else if (flag) {
			flag = false;
			savePos = { 30,-17,0 };
		}
		isShake = true;
		Audio::GetInstance()->PlayWave(15);
	}

	Select();

	if (sceneChangeFlag) {
		sceneChange.SceneChangeStart("SelectScene");
	}

	SpecifiedMove();


	{
		player.get()->PosAddVelocity();
			if (player.get()->pos.x > 200) {
			player.get()->velocity = CalcReflectVector(player.get()->velocity, { -1,0,0 });
		}
		if (player.get()->pos.x < -200) {
			player.get()->velocity = CalcReflectVector(player.get()->velocity, { 1,0,0 });
		}
		if (player.get()->pos.y > 150) {
			player.get()->velocity = CalcReflectVector(player.get()->velocity, { 0,-1,0 });
		}
		if (player.get()->pos.y < -150) {
			player.get()->velocity = CalcReflectVector(player.get()->velocity, { 0,1,0 });
		}
		player.get()->rotate.z += 1;
			/*ConvertNormalToDeg(player.get()->velocity.Normal(), Vector3{ 0,1,0 }).z;*/
	}

	titleObject3d->Update();
	startObject3d->Update();
	endObject3d->Update();
	player.get()->Adaptation();
}

void TitleScene::LastUpdate() {
	sceneChange.Update();
}

void TitleScene::Draw() {
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
	player.get()->Draw();
	Object3d::PreDraw(cmdList);
	titleObject3d->Draw();
	startObject3d->Draw();
	endObject3d->Draw();
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	//シーン遷移
	sceneChange.Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void TitleScene::Select() {
	if (!flag) {
		startObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		startObject3d->SetScale({ 10, 1, 5 });
		endObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		endObject3d->SetScale({ 30, 1, 10 });
	}
	else if (flag) {
		startObject3d->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		startObject3d->SetScale({ 30, 1, 10 });
		endObject3d->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		endObject3d->SetScale({ 10, 1, 5 });
	}

	Shake();
}

void TitleScene::Shake() {
	Input* input = Input::GetInstance();

	if (!flag && isShake) {
		XMFLOAT3 shake = {};
		shakeTimer++;

		input->SetVibration(true);

		if (shakeTimer > 0) {
			shake.x = (rand() % (7 - attenuation) - 3) + savePos.x;
			shake.y = (rand() % (7 - attenuation) - 3) + savePos.y;
			shake.z = savePos.z;
		}

		if (shakeTimer >= attenuation * 2) {
			attenuation += 1;
			endObject3d->SetPosition(shake);
		}
		else if (attenuation >= 6) {
			shakeTimer = 0;
			attenuation = 0;
			isShake = 0;
			input->SetVibration(false);
			endObject3d->SetPosition(savePos);
		}
	}
	else if (flag && isShake) {
		XMFLOAT3 shake = {};
		shakeTimer++;
		input->SetVibration(true);

		if (shakeTimer > 0) {
			shake.x = (rand() % (7 - attenuation) - 3) + savePos.x;
			shake.y = (rand() % (7 - attenuation) - 3) + savePos.y;
			shake.z = savePos.z;
		}

		if (shakeTimer >= attenuation * 2) {
			attenuation += 1;
			startObject3d->SetPosition(shake);
		}
		else if (attenuation >= 6) {
			shakeTimer = 0;
			attenuation = 0;
			isShake = 0;
			input->SetVibration(false);
			startObject3d->SetPosition(savePos);
		}
	}
}

void TitleScene::SpecifiedMove() {
	if (maxSpecifiedMoveTimer >= specifiedMoveTimer) {
		specifiedMoveTimer++;
		if (maxSpecifiedMoveTimer <= specifiedMoveTimer) {
			specifiedMoveTimer = 0;
			if (!isUp) {
				isUp = true;
			}
			else if (isUp) {
				isUp = false;
			}
		}
	}

	float eTime = (float)(specifiedMoveTimer / static_cast<double>(maxSpecifiedMoveTimer));


	if (isUp) {
		titleObjectPosition.y = Ease(Out, ease::Quint, eTime, specifiedBouncePosDown, specifiedBouncePosUp);
		if (flag) {
			startObjectPosition.y = Ease(Out, ease::Quint, eTime, specifiedBounceStartPosDown, specifiedBounceStartPosUp);
		}
		else if (!flag) {
			endObjectPosition.y = Ease(Out, ease::Quint, eTime, specifiedBounceEndPosDown, specifiedBounceEndPosUp);
		}
	}
	else if (!isUp) {
		titleObjectPosition.y = Ease(In, ease::Quint, eTime, specifiedBouncePosUp, specifiedBouncePosDown);
		if (flag) {
			startObjectPosition.y = Ease(In, ease::Quint, eTime, specifiedBounceStartPosUp, specifiedBounceStartPosDown);
		}
		else if (!flag) {
			endObjectPosition.y = Ease(In, ease::Quint, eTime, specifiedBounceEndPosUp, specifiedBounceEndPosDown);
		}
	}

	titleObject3d->SetPosition(titleObjectPosition);
	if (!isShake) {
		startObject3d->SetPosition(startObjectPosition);
		endObject3d->SetPosition(endObjectPosition);
	}
}