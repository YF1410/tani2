#pragma once
#include "Object3d.h"

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
	// 描画
	void Draw();
	//デバッグテキスト出力に使用
	float GetAngle() { return angle; }
	//敵の動き
	void enemyMove();

private: // メンバ変数
	std::unique_ptr<Object3d> coreObj;
	std::unique_ptr<Model> coreModel;

	std::unique_ptr<Object3d> sphereObj;
	std::unique_ptr<Model> sphereModel;

	float angle = 0.0f;
	float searchPlayerLen = 5.0f;
	bool playerContact = false;
	float speed = 0.03f;
	int moveTime = 0;
	int maxMoveTime = 180;
};