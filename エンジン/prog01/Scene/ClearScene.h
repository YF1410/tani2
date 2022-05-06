#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
#include "Camera.h"

#include "Sprite.h"
#include "DebugText.h"
#include "Object3d.h"

#include <memory>

class ClearScene : public BaseScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // メンバ関数
	// デストラクタ
	~ClearScene();
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

	//ボタン関係
	void Select();
	void Shake();

	void Bounce();
	void OutBack();

private: // メンバ変数
	//スプライト
	std::unique_ptr<Sprite> sprite;
	//カメラ
	std::unique_ptr<Camera> camera;
	//ライト
	std::unique_ptr<LightGroup> light;

	std::unique_ptr<Object3d> stageclearObject3d;
	std::unique_ptr<Object3d> nextStageObject3d;
	std::unique_ptr<Object3d> clearEscapeObject3d;

	bool flag = true;
	bool shakeTimerFlag = false;
	int shakeTimer = 0;
	int attenuation = 0;

	bool bounceTimerFlag = false;
	int bounceTimer = 0;
	int maxBounceTimer = 70;

	XMFLOAT3 maxNextStageScale = { 30, 1, 10 };
	XMFLOAT3 maxClearEscapeScale = { 10, 1, 5 };
	int nextStageScaleTimer = 0;
	int maxNextStageScaleTimer = 20;
	int clearEscapeTimer = 0;
	int maxClearEscapeTimer = 20;

	XMFLOAT3 savePos = {};
};