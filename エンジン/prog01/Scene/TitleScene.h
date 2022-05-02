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

class TitleScene : public BaseScene
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
	~TitleScene();
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

private: // メンバ変数
	//スプライト
	std::unique_ptr<Sprite> sprite;
	//カメラ
	std::unique_ptr<Camera> camera;
	//ライト
	std::unique_ptr<LightGroup> light;

	std::unique_ptr<Object3d> titleObject3d;
	std::unique_ptr<Object3d> startObject3d;
	std::unique_ptr<Object3d> endObject3d;

	bool flag = true;
	bool shakeTimerFlag = false;
	int shakeTimer = 0;
	int attenuation = 0;

	XMFLOAT3 savePos = {};
};