#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "Vector3.h"
#include <vector>

class Enemy {
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	Enemy();
	~Enemy();
	// 毎フレーム処理
	void Update();
	//最終反映
	void Reflection();
	// 描画
	void Draw();

	void StaticUpdate();
	void StaticReflection();
	void StaticDraw();

	//デバッグテキスト出力に使用
	float GetAngle() { return angle; }
	//当たり判定に使用
	Vector3 GetPos() { return pos; }
	//アングルセッター
	void SetAngle(float angle) { this->angle = angle; }
	//playerContactセッター
	void SetPlayerContact(bool isPlayerContact) { this->isPlayerContact = isPlayerContact; }
	//敵の動き
	void enemyMove();

	static std::vector<Enemy*> enemys;

private: // メンバ変数
	std::unique_ptr<Object3d> coreObj;
	std::unique_ptr<Model> coreModel;

	std::unique_ptr<Object3d> sphereObj;
	std::unique_ptr<Model> sphereModel;

	std::unique_ptr<FbxObject3d> enemyObj;

	float angle = 0.0f;
	float searchPlayerLen = 5.0f;
	bool isPlayerContact = false;
	bool isWandering = false;
	int wanderingCount = 0;
	float speed = 3.0f;
	int moveTime = 180;
	int maxMoveTime = 180;
	bool isAlive;
	Vector3 pos;
	Vector3 afterPos;
	float scale;
};