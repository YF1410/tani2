#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "Audio.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "LightGroup.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "FbxLoader.h"
#include "PlayerObject.h"
#include "MapChip.h"

#include <vector>
#include <memory>

#include "EnemyManager.h"
#include "BaseScene.h"
#include "UserInterface.h"


class CollisionManager;
class ContactableObject;

class GameScene : public BaseScene {
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数
	//コンストラクタ
	GameScene();
	// デストラクタ
	~GameScene();
	// 初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	// 毎フレーム処理
	void Update() override;
	// 毎フレームごとの最後の処理
	void LastUpdate() override;
	// 描画
	void Draw() override;


	static int counter;
private:	//当たり判定系関数
	//void AttackDebrisToEnemy();	//破片と敵
	//void PlayerToDebris();	//プレイヤーと破片

private: // メンバ変数
	//カメラ
	std::unique_ptr<Camera> camera;

	Vector3 eyeDistance = { 0,2700,-500 };
	const Vector3 eyeDistanceDef = { 0,2700,-1500 };
	Vector3 targetDistance = { 0,0,100 };
	const Vector3 targetDistanceDef = { 0,0,100 };
	//スプライト
	std::unique_ptr<Sprite> sprite;
	//オブジェクト
	std::unique_ptr<Model> slimeModel;
	std::unique_ptr<Model> modelSphere;

	std::unique_ptr<PlayerObject> playerObject;
	//エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyManager;
	//UI
	std::unique_ptr<UserInterface> ui;
	//背景
	std::unique_ptr<Sprite> bg;


	std::unique_ptr<Model> modelPlane;
	std::vector<std::unique_ptr<Object3d>> objects;

	int frame = 0;
	bool frameF = false;
	int healFrame = 3;
	int boomFrame = 2;

	int weveStartTimer = 0;
	float upTimer = 0.0f;
	bool changeFlag = false;
	bool endFlag = false;
	bool showingFlag = false;
	bool flag = false;


	//ライト
	std::unique_ptr<LightGroup> light;
	//光線方向初期値
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	float searchPlayerLen = 500.0f;

	Line line;
	AABB aabb;

	//当たり判定
	CollisionManager* collisionManager = nullptr;


	bool checkPoint;
};