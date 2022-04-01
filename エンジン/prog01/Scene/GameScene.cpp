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

using namespace DirectX;

GameScene::GameScene() {
	MapChip::GetInstance()->Initialize();
	//マップ生成
	MapChip::GetInstance()->SetMapName(MapChip::TEST_MAP);
	MapChip::GetInstance()->CreateStage();


	//プレイヤー生成
	playerObject = std::make_unique<PlayerObject>(MapChip::GetInstance()->GetStartPos());
}

GameScene::~GameScene() {
	Finalize();
}

void GameScene::Initialize() {
	collisionManager = CollisionManager::GetInstance();

	// カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera.get());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera.get());

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
	}
	// デバッグテキスト初期化
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png")) {
		assert(0);
	}

	// 背景スプライト生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetSize({ 100.0f,100.0f });
	sprite->SetPosition({ 100.0f,100.0f });

	// パーティクルマネージャ生成
	particleMan = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());

	//ライト生成
	light = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(light.get());
	FbxObject3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, true);
	light->SetDirLightActive(2, true);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, true);
	
	//敵初期配置
	Enemy::enemys.push_back(new Enemy({ 4000,0,-3200 }));
	Enemy::enemys.push_back(new Enemy({ 6200,0,-3200 }));
	Enemy::enemys.push_back(new Enemy({ 5200,0,-2400 }));
	Enemy::enemys.push_back(new Enemy({ 6000,0,-2200 }));
	Enemy::enemys.push_back(new Enemy({ 4200,0,-3200 }));
	Enemy::enemys.push_back(new Enemy({ 5200,0,-1100 }));

	Enemy::enemys.push_back(new Enemy({ 1600,0,-5000 }));
	Enemy::enemys.push_back(new Enemy({ 1600,0,-5000 }));
	Enemy::enemys.push_back(new Enemy({ 1800,0,-5800 }));


	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,1600,-500 });
	camera->SetUp({ 0,1,0 });
	
	//プレイヤーの初期化
	playerObject->Initialize();

	checkPoint = false;
}

void GameScene::Finalize() {
}

void GameScene::Update() {
	//カメラ更新
	//プレイヤーの少し上を焦点にする
	//カメラ更新
	Vector3 camEye = camera.get()->GetEye();
	float debrisLengthMax = 0.0f;
	for (int i = 0; i < Debris::debris.size(); i++) {
		if (Debris::debris[i]->isFirstAttack &&
			debrisLengthMax <= Vector3(Debris::debris[i]->pos - playerObject.get()->GetPos()).Length()) {
			debrisLengthMax = Vector3(Debris::debris[i]->pos - playerObject.get()->GetPos()).Length();
		}
	}
	eyeDistance = Ease(Out, Quad, 0.01f,
		camera.get()->GetEye(),
		Vector3(playerObject.get()->GetPos() + eyeDistanceDef + Vector3(0, debrisLengthMax, 0)));
	camera->SetEye(eyeDistance);
	//プレイヤーの少し上を焦点にする
	camera->SetTarget(Ease(Out, Quad, 0.01f,
		camera.get()->GetTarget(),
		Vector3(playerObject.get()->GetPos() + targetDistance))
	);
	camera->Update();

	//追加の敵
	if (playerObject.get()->isCheckPoint && !checkPoint) {
		Enemy::enemys.push_back(new Enemy({ 2000,0,-5000 }));
		Enemy::enemys.push_back(new Enemy({ 2000,0,-5000 }));
		Enemy::enemys.push_back(new Enemy({ 1600,0,-5000 }));
		Enemy::enemys.push_back(new Enemy({ 1600,0,-5000 }));
		Enemy::enemys.push_back(new Enemy({ 1800,0,-5800 }));
		checkPoint = true;

	}
	//マップチップ更新
	//MapChip::GetInstance()->Update(MapChip::TEST_MAP);


	//入力更新
	Input *input = Input::GetInstance();
	
	//ライト更新
	light->Update();
	//ステージ更新
	//testStage->Update();
	
	//デバックテキスト
	DebugText::GetInstance()->VariablePrint(0, 0, "playerSize", playerObject.get()->size, 3);
	DebugText::GetInstance()->VariablePrint(0, 40, "DebrisCount", Debris::debris.size(), 3);

	particleMan->Update();

	for (auto& object : objects) {
		object->Update();
	}

	//プレイヤー更新
	playerObject->Update();
	//破片更新
	Debris::StaticUpdate();
	//エネミー更新
	Enemy::StaticUpdate();


	float contact_pos = 0.0f;

	// 全ての衝突をチェック
	collisionManager->CheckBroadCollisions();
	//全ての移動最終適応処理
	playerObject.get()->Adaptation();
	Debris::StaticAdaptation();
	Enemy::StaticAdaptation();
	MapChip::GetInstance()->Adaptation();
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//sprite->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景スプライト描画

#pragma region 3Dオブジェクト描画
	// 3Dオブクジェクトの描画
	Object3d::PreDraw(cmdList);
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画


#pragma region 3Dオブジェクト(FBX)描画
	//testStage->Draw(DirectXCommon::GetInstance()->GetCommandList());
	MapChip::GetInstance()->Draw();
	playerObject->Draw();
	Debris::StaticDraw();
	Enemy::StaticDraw();
#pragma endregion 3Dオブジェクト(FBX)描画


#pragma region パーティクル
	// パーティクルの描画
	particleMan->Draw(cmdList);
#pragma endregion パーティクル


#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画

	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_C)) {
		SceneManager::GetInstance()->ChangeScene("EnemyTestScene");
	}
	else if (input->TriggerKey(DIK_B)) {
		SceneManager::GetInstance()->ChangeScene("PlayerTestScene");
	}
}
//
//void GameScene::AttackDebrisToEnemy()
//{
//	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
//		/*float minLength = Enemy::enemys[enemysNum]->sarchLength;
//		bool isHoming = false;*/
//
//		for (int enemysNum = 0; enemysNum < Enemy::enemys.size(); enemysNum++) {
//			//どちらも攻撃状態でなければエネミーを押し出し
//			if (!Debris::debris[debrisNum]->isAttack &&				//破片が攻撃状態ではなく
//				Enemy::enemys[enemysNum]->state !=Enemy::ATTACK &&	//エネミーが攻撃状態ではなく
//				
//				Collision::CheckSphere2Sphere(
//				Debris::debris[debrisNum]->collider.realSphere,	//破片の攻撃範囲
//				Enemy::enemys[enemysNum]->collider.realSphere		//エネミーのヒットスフィア	
//				//&hitPos,
//				//&hitNormal
//			)) {
//				//Enemy::enemys[enemysNum]->SetPos(hitPos + -hitNormal * Enemy::enemys[enemysNum]->collider.realSphere.radius);
//			}
//
//			//破片からエネミーへの攻撃
//			XMVECTOR hitPos;
//			if (!Enemy::enemys[enemysNum]->isInvincible &&			//エネミーが無敵ではないとき	
//				Collision::CheckSphere2Sphere(
//					Debris::debris[debrisNum]->collider.attackSphere,	//破片の攻撃範囲
//					Enemy::enemys[enemysNum]->collider.hitSphere,		//エネミーのヒットスフィア	
//					&hitPos
//				))
//			{
//				Vector3 hitNormal = hitPos - Enemy::enemys[enemysNum]->collider.hitSphere.center;
//				Debris::debris[debrisNum]->Bounse(hitPos, Vector3(hitNormal).Normalize());
//				if(Debris::debris[debrisNum]->isAttack){
//					Debris::debris[debrisNum]->isAttackFlame = true;
//					Enemy::enemys[enemysNum]->Damage(1/*testダメージ*/);
//
//				}
//			}
//			//エネミーから破片への攻撃
//			//if (!Debris::debris[debrisNum]->isAttack &&				//破片が攻撃状態ではなく
//			//	Collision::CheckSphere2Sphere(
//			//		Enemy::enemys[enemysNum]->collider.attackSphere,		//エネミーのヒットスフィア
//			//		Debris::debris[debrisNum]->collider.hitSphere	//破片の攻撃範囲
//			//	))
//			//{
//
//			//	Debris::debris[debrisNum]->Damage(0.1f);
//			//}
//
//			//エネミーから破片の探索
//			//if (!Debris::debris[debrisNum]->isAttack &&			//攻撃中以外の破片を除外
//			//	Collision::CheckSphere2Sphere(
//			//	Debris::debris[debrisNum]->collider.realSphere,
//			//	Enemy::enemys[enemysNum]->collider.searchArea)) {
//			//	//距離が現在の最小値以下なら対象を更新
//			//	float length = Vector3(Debris::debris[debrisNum]->GetPos() - Enemy::enemys[enemysNum]->GetPos()).Length();
//
//			//	if(length < minLength){
//			//		isHoming = true;
//			//		Enemy::enemys[enemysNum]->HomingObjectCheck(Debris::debris[debrisNum]->GetPos());
//			//	}
//			//}
//		}
//	}
//}

//void GameScene::PlayerToDebris()
//{
//	//破片とプレイヤーの衝突
//	for (int i = 0; i < Debris::debris.size(); i++) {
//		//攻撃中の物とは判定をとらない
//		if (Debris::debris[i]->isAttack) continue;
//		//吸い寄せ判定
//		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.suctionSphere, Debris::debris[i]->collider.hitSphere)) {
//			Debris::debris[i]->SuckedPlayer(playerObject->GetPos(), playerObject->GetSuction());
//		}
//		//吸収判定
//		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.absorbSphere, Debris::debris[i]->collider.hitSphere)) {
//			playerObject.get()->Absorb(Debris::debris[i]->AbsorbedToPlayer());
//		}
//	}
//}
