#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

#include "FbxModel.h"
#include "FbxLoader.h"
#include "Camera.h"
#include "LightGroup.h"



class FbxObject3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:// 定数
//ボーンの最大数
	static const int MAX_BONES = 32;

public: // サブクラス
	// 定数バッファ用データ構造体（座標変換行列用）
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
	};

	// 定数バッファ用データ構造体（スキニング）
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
		XMFLOAT4 color;
	};

	struct Animation
	{
		FbxAnimStack* animstack;
		FbxTakeInfo* takeinfo;
	};

public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	// 静的破棄
	static void StaticFinalize();
	// グラフィックパイプラインの生成
	static void CreateGraphicsPipeline();
	// 3Dオブジェクト生成
	static std::unique_ptr<FbxObject3d> Create(FbxModel* model = nullptr, bool isAnimation = false,bool isBillbord = false);
	// setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }
	static void SetLight(LightGroup* light) { FbxObject3d::light = light; }

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// カメラ
	static Camera* camera;
	//ライト
	static LightGroup* light;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

public: // メンバ関数
	//デストラクタ
	~FbxObject3d();
	// 初期化
	bool Initialize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// 行列の更新
	void UpdateWorldMatrix();
	//モデルを設定
	void SetModel(FbxModel* model) { this->model = model; }
	//モデルのゲッター
	FbxModel *GetModel() {return model; }
	//アニメーションのロード
	void LoadAnimation();
	//アニメーション開始
	void PlayAnimation(int animationNumber = 0 ,bool isLoop = true);

	// 座標の取得
	const XMFLOAT3& GetPos() { return position; }
	// 座標の設定
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	// X,Y,Z軸回りの取得
	const XMFLOAT3& GetRotation() { return rotation; }
	// X,Y,Z軸回りの設定
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	// スケールの取得
	const XMFLOAT3& GetScale() { return scale; }
	// スケールの設定
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	void SetScale(float scale) { this->scale = { scale,scale,scale }; }
	//ビルボード設定
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	//アニメーションを止めるか
	void SetIsPlay(bool _isPlay) { isPlay = _isPlay; }

	void SetAnimationCurrentTime(FbxTime currentTime) {	this->currentTime = currentTime; }
	FbxTime GetAnimationCurrentTime() { return currentTime; }

	XMMATRIX GetMatWorld() { return matWorld; }

	void SetAlpha(float _alpha) { model->SetAlpha(_alpha); }

	float GetAlpha() { return model->GetAlpha(); }



	//カラーの設定
	void SetColor(XMFLOAT4 color) { this->color = color; }
	XMFLOAT4 GetColor() { return color; }

protected: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;
	// 定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// モデル
	FbxModel* model = nullptr;
	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間(アニメーション)
	FbxTime currentTime;

	//ビルボード
	bool isBillboard = false;

	//アニメーション再生中
	bool isPlay = false;
	//ループフラグ
	bool isLoop = false;
	//アニメーションの保存
	std::vector<Animation> animationData;

	XMFLOAT4 color = { 1,1,1,1 };

	//クラス名(デバック用)
	const char *name = nullptr;
	
};