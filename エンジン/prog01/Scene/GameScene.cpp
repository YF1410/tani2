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

using namespace DirectX;

GameScene::GameScene() {
	//モデルマネージャーに全モデルロード
	ModelManager::GetIns()->Initialize();
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
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, true);
	light->SetDirLightActive(2, true);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, true);


	// 3Dオブジェクト生成
	playerObject = std::make_unique<PlayerObject>();
	Enemy::enemys.push_back(new Enemy({ 2000,0,-2000 }));
	Enemy::enemys.push_back(new Enemy({ 2000,0,-1600 }));
	Enemy::enemys.push_back(new Enemy({ 1600,0,-2000 }));
	Enemy::enemys.push_back(new Enemy({ 1600,0,-1600 }));
	Enemy::enemys.push_back(new Enemy({ 1800,0,-1800 }));

	testStage = FbxObject3d::Create(ModelManager::GetIns()->GetModel(TESTS_TAGE));

	//サウンド再生
	Audio::GetInstance()->LoadWave(0, "Resources/Alarm01.wav");

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,1600,-500 });
	camera->SetUp({ 0,1,0 });

	//Debris::StaticInit();
	playerObject->Init();

	//ステージの頂点データ保存
	testStage.get()->Update();
	stagePolygon.clear();
	auto vertices = *testStage->GetModel()->GetVertices();
	auto indices = *testStage->GetModel()->GetIndces();
	XMMATRIX matWorld = testStage->GetModel()->GetModelTransform() * testStage->GetMatWorld();
	for (int Num = 0; Num < indices.size() - 3; Num += 3) {
		Triangle polygon;
		//頂点からpolygonに変換
		int a = indices[Num];
		polygon = {
			XMVector4Transform(XMLoadFloat3(&vertices[indices[Num]].pos),matWorld),
			XMVector4Transform(XMLoadFloat3(&vertices[indices[Num + 1]].pos),matWorld),
			XMVector4Transform(XMLoadFloat3(&vertices[indices[Num + 2]].pos),matWorld),
		};

		//法線計算
		Vector3 v1 = polygon.p1 - polygon.p0;
		Vector3 v2 = polygon.p2 - polygon.p0;
		polygon.normal = v1.VCross(v2);
		polygon.normal = Vector3(polygon.normal).Normalize();

		stagePolygon.push_back(polygon);
	}
}

void GameScene::Finalize() {
}

void GameScene::Update() {
	//カメラ更新
	camera->SetEye(Vector3(playerObject.get()->GetPos() + Vector3(0, 1, -1) * 1000));
	camera->SetTarget(playerObject.get()->GetPos());
	camera->Update();

	//入力更新
	Input* input = Input::GetInstance();

	//ライト更新
	light->Update();
	//ステージ更新
	testStage->Update();

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


	//破片とステージの衝突
	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
		//停止状態の物とは判定をとらない
		/*if (Debris::debris[debrisNum]->isStop) {
			continue;
		}*/
		XMVECTOR hitPos;
		Triangle hitPolygon;
		if (CheckSphere2Mesh(Debris::debris[debrisNum]->collider.realSphere, stagePolygon, &hitPos, &hitPolygon)) {
			Debris::debris[debrisNum]->Bounse(hitPos, hitPolygon.normal);
		}
	}


	float playerEnemyLen;	//プレイヤーとエネミーの距離
	float debrisEnemyLen;	//破片とエネミーの距離
	float lenTmp;			//破片とエネミーの距離最小値保存用
	Vector3 posTmp;			//lenTmpでの最小距離の破片の位置保存用
	//playerEnemyLen = Vector3(playerObject->GetPos() - enemyObject->GetPos()).Length();

	//破片とエネミーの当たり判定
	AttackDebrisToEnemy();
	//プレイヤーと破片
	PlayerToDebris();

	//fbxObject3d->Update();
	// 全ての衝突をチェック
	collisionManager->CheckAllCollisions();
	//全ての移動最終適応処理
	playerObject.get()->Adaptation();
	Debris::StaticReflection();
	Enemy::StaticAdaptation();
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
	testStage->Draw(DirectXCommon::GetInstance()->GetCommandList());
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

void GameScene::AttackDebrisToEnemy() {
	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
		/*float minLength = Enemy::enemys[enemysNum]->sarchLength;
		bool isHoming = false;*/

		for (int enemysNum = 0; enemysNum < Enemy::enemys.size(); enemysNum++) {
			//どちらも攻撃状態でなければエネミーを押し出し
			if (!Debris::debris[debrisNum]->isAttack &&				//破片が攻撃状態ではなく
				Enemy::enemys[enemysNum]->state != Enemy::ATTACK &&	//エネミーが攻撃状態ではなく

				Collision::CheckSphere2Sphere(
					Debris::debris[debrisNum]->collider.realSphere,	//破片の攻撃範囲
					Enemy::enemys[enemysNum]->collider.realSphere		//エネミーのヒットスフィア	
					//&hitPos,
					//&hitNormal
				)) {
				//Enemy::enemys[enemysNum]->SetPos(hitPos + -hitNormal * Enemy::enemys[enemysNum]->collider.realSphere.radius);
			}

			//破片からエネミーへの攻撃
			XMVECTOR hitPos;
			if (!Enemy::enemys[enemysNum]->isInvincible &&			//エネミーが無敵ではないとき	
				Collision::CheckSphere2Sphere(
					Debris::debris[debrisNum]->collider.attackSphere,	//破片の攻撃範囲
					Enemy::enemys[enemysNum]->collider.hitSphere,		//エネミーのヒットスフィア	
					&hitPos
				)) {
				Vector3 hitNormal = hitPos - Enemy::enemys[enemysNum]->collider.hitSphere.center;
				Debris::debris[debrisNum]->Bounse(hitPos, Vector3(hitNormal).Normalize());
				if (Debris::debris[debrisNum]->isAttack) {
					Debris::debris[debrisNum]->isAttackFlame = true;
					Enemy::enemys[enemysNum]->Damage(1/*testダメージ*/);

				}
			}
			//エネミーから破片への攻撃
			//if (!Debris::debris[debrisNum]->isAttack &&				//破片が攻撃状態ではなく
			//	Collision::CheckSphere2Sphere(
			//		Enemy::enemys[enemysNum]->collider.attackSphere,		//エネミーのヒットスフィア
			//		Debris::debris[debrisNum]->collider.hitSphere	//破片の攻撃範囲
			//	))
			//{

			//	Debris::debris[debrisNum]->Damage(0.1f);
			//}

			//エネミーから破片の探索
			//if (!Debris::debris[debrisNum]->isAttack &&			//攻撃中以外の破片を除外
			//	Collision::CheckSphere2Sphere(
			//	Debris::debris[debrisNum]->collider.realSphere,
			//	Enemy::enemys[enemysNum]->collider.searchArea)) {
			//	//距離が現在の最小値以下なら対象を更新
			//	float length = Vector3(Debris::debris[debrisNum]->GetPos() - Enemy::enemys[enemysNum]->GetPos()).Length();

			//	if(length < minLength){
			//		isHoming = true;
			//		Enemy::enemys[enemysNum]->HomingObjectCheck(Debris::debris[debrisNum]->GetPos());
			//	}
			//}
		}
	}
}

void GameScene::AttackEnemyToDebris() {
	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
		/*float minLength = Enemy::enemys[enemysNum]->sarchLength;
		bool isHoming = false;*/

		for (int enemysNum = 0; enemysNum < Enemy::enemys.size(); enemysNum++) {

			//エネミーから破片への攻撃
			if (!Debris::debris[debrisNum]->isAttack &&				//破片が攻撃状態ではなく
				Collision::CheckSphere2Sphere(
					Enemy::enemys[enemysNum]->collider.attackSphere,		//エネミーのヒットスフィア
					Debris::debris[debrisNum]->collider.hitSphere		//破片の攻撃範囲
				)) 			{
				Debris::debris[debrisNum]->Damage(0.1f);
			}

			//エネミーから破片の探索
			if (!Debris::debris[debrisNum]->isAttack &&			//攻撃中以外の破片を除外
				Collision::CheckSphere2Sphere(
					Debris::debris[debrisNum]->collider.realSphere,
					Enemy::enemys[enemysNum]->collider.searchArea)) {
				Ray ray;
				Sphere s;
				ray.start = Enemy::enemys[enemysNum]->GetPos();
				ray.dir = Debris::debris[debrisNum]->pos - Enemy::enemys[enemysNum]->GetPos();
				//距離が現在の最小値以下なら対象を更新
				if (Collision::CheckRay2Sphere(ray,)) {
					float length = Vector3(Debris::debris[debrisNum]->GetPos() - Enemy::enemys[enemysNum]->GetPos()).Length();
				}

				/*if(length < minLength){
					isHoming = true;
					Enemy::enemys[enemysNum]->HomingObjectCheck(Debris::debris[debrisNum]->GetPos());
				}*/
			}
		}
	}
}

void GameScene::AttackEnemyToPlayer() {
}

void GameScene::PlayerToDebris() {
	//破片とプレイヤーの衝突
	for (int i = 0; i < Debris::debris.size(); i++) {
		//攻撃中の物とは判定をとらない
		if (Debris::debris[i]->isAttack) continue;
		//吸い寄せ判定
		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.suctionSphere, Debris::debris[i]->collider.hitSphere)) {
			Debris::debris[i]->SuckedPlayer(playerObject->GetPos(), playerObject->GetSuction());
		}
		//吸収判定
		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.absorbSphere, Debris::debris[i]->collider.hitSphere)) {
			playerObject.get()->Absorb(Debris::debris[i]->AbsorbedToPlayer());
		}
	}
}


bool GameScene::CheckSphere2Mesh(
	Sphere& sphere,				//球
	std::vector<Triangle> meshDate,					//メッシュデータ
	XMVECTOR* HitPos,			//衝突位置
	Triangle* hitTriangle
) {
	for (int polygonNum = 0; polygonNum < meshDate.size(); polygonNum++) {
		if (Collision::CheckSphere2Triangle(sphere, meshDate[polygonNum], HitPos)) {
			if (hitTriangle != nullptr) {
				*hitTriangle = meshDate[polygonNum];
			}
			return true;
		}
	}
	return false;
}