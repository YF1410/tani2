#include "PlayerTestScene.h"
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

PlayerTestScene::PlayerTestScene() {
	//モデルマネージャーに全モデルロード
	ModelManager::GetIns()->Initialize();
}

PlayerTestScene::~PlayerTestScene() {
	Finalize();
}

void PlayerTestScene::Initialize() {


	collisionManager = CollisionManager::GetInstance();

	// カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera.get());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera.get());

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) 	{
		assert(0);
	}
	// デバッグテキスト初期化
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png")) 	{
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
	playerObject->Init();


	testStage = FbxObject3d::Create(ModelManager::GetIns()->GetModel(TESTS_TAGE));

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,1000, -1000 });
	camera->SetUp({ 0,1,0 });
	camera->Update();

	//Debris::StaticInit();


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

void PlayerTestScene::Finalize() {
}

void PlayerTestScene::Update() {
	camera->SetEye(Vector3(playerObject.get()->GetPos() + Vector3(0,1,-1) * 1000));
	camera->SetTarget(playerObject.get()->GetPos());
	camera->Update();
	Input* input = Input::GetInstance();
	light->Update();
	particleMan->Update();
	testStage->Update();

	//カメラをプレイヤーの位置から計算

	DebugText::GetInstance()->VariablePrint(0, 0, "playerSize", playerObject.get()->size, 3);
	DebugText::GetInstance()->VariablePrint(0, 40, "DebrisCount", Debris::debris.size(), 3);


	if (input->TriggerKey(DIK_C)) 	{
		SceneManager::GetInstance()->ChangeScene("EnemyTestScene");
	}


	for (auto& object : objects) {
		object->Update();
	}
	//プレイヤー更新
	playerObject->Update();
	//破片更新
	Debris::StaticUpdate();

	//破片とステージの衝突
	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
		//停止状態の物とは判定をとらない
		if (Debris::debris[debrisNum]->isStop) {
			continue;
		}
		XMVECTOR hitPos;
		Triangle hitPolygon;
		if (CheckSphere2Mesh(Debris::debris[debrisNum]->collider.realSphere, stagePolygon,&hitPos,&hitPolygon)) {
			Debris::debris[debrisNum]->Bounse(hitPos,hitPolygon.normal);
		}
	}

	//プレイヤーと壁の衝突
	XMVECTOR hitPos;
	Triangle hitPolygon;
	if (CheckSphere2Mesh(playerObject.get()->collider.realSphere, stagePolygon, &hitPos, &hitPolygon)) {
		DebugText::GetInstance()->VariablePrint(0, 80, "HitStage", 1, 3);
		playerObject.get()->HitWall(hitPos, hitPolygon.normal);
	}


	//破片とプレイヤーの衝突
	for (int i = 0; i < Debris::debris.size(); i++) {
		//攻撃中の物とは判定をとらない
		if (Debris::debris[i]->isAttack) continue;
		//吸い寄せ判定
		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.suctionSphere, Debris::debris[i]->collider.absorbedSphere)) {
			Debris::debris[i]->SuckedPlayer(playerObject->GetPos(), playerObject->GetSuction());
		}
		//吸収判定
		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.absorbSphere, Debris::debris[i]->collider.absorbedSphere)) {
			playerObject.get()->Absorb(Debris::debris[i]->AbsorbedToPlayer());
		}
	}

	//fbxObject3d->Update();
	// 全ての衝突をチェック
	//collisionManager->CheckAllCollisions();

	//全ての移動最終適応処理
	playerObject.get()->Adaptation();
	Debris::StaticReflection();
}

void PlayerTestScene::Draw() {
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
}

bool PlayerTestScene::CheckSphere2Mesh(
	Sphere &sphere,				//球
	std::vector<Triangle> meshDate,					//メッシュデータ
	XMVECTOR *HitPos,			//衝突位置
	Triangle *hitTriangle
)
{
	for (int polygonNum = 0; polygonNum < meshDate.size(); polygonNum++) {
		if (Collision::CheckSphere2Triangle(sphere, meshDate[polygonNum],HitPos)) {
			if (hitTriangle != nullptr) {
				*hitTriangle = meshDate[polygonNum];
			}
			return true;
		}
	}
	return false;
}
