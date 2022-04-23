#pragma once

#include "ParticleManager.h"
#include "Vector3.h"

#include <string>

#include <string>

class ParticleEmitter
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	ParticleEmitter* Create(std::wstring fName = L"effect1");

	static void SetCamera(Camera* _camera) { camera = _camera; };

private: //  静的メンバ変数
	static Camera* camera;

public: // メンバ関数
	// 追加
	void RandAdd(int count = 10, int life = 60, XMFLOAT3 position = { 0,0,0 });
	void Add(int count = 10, int life = 60, XMFLOAT3 position = { 0,0,0 });
	void AddHeal(int count = 10, int life = 60, XMFLOAT3 position = { 0,0,0 });
	void AddBoom(int count = 10, int life = 60, XMFLOAT3 position = { 0,0,0 });
	void AddRef(int count = 10, int life = 60, XMFLOAT3 position = { 0,0,0 },Vector3 velocity = {0,0,0});
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	// 座標の設定
	void SetCenter(const float md_pos) { this->md_pos = md_pos; }
	// 速度の設定
	void SetVelocity(float md_vel) { this->md_vel = md_vel; }
	// 加速度の設定
	void SetAccel(float md_acc) { this->md_acc = md_acc; }
	//色(RGBA)初期値の設定
	void SetStartColor(XMFLOAT4 s_color) { this->s_color = s_color; }
	//色(RGBA)最終値の設定
	void SetEndColor(XMFLOAT4 e_color) { this->e_color = e_color; }
	// スケール初期値の設定
	void SetStartScale(const float s_scale) { this->s_scale = s_scale; }
	// スケール最終値の設定
	void SetEndScale(const float e_scale) { this->e_scale = e_scale; }

private: // メンバ変数
	//座標
	XMFLOAT3 position = {};
	//速度
	XMFLOAT3 velocity = {};
	//加速度
	XMFLOAT3 accel = {};
	//現在フレーム
	int frame = 0;
	//終了フレーム
	int num_frame = 0;
	// スケール初期値
	float s_scale = 1.0f;
	// スケール最終値
	float e_scale = 0.0f;
	// 色(RGBA)初期値
	XMFLOAT4 s_color = { 1, 1, 1, 1 };
	// 色(RGBA)最終値
	XMFLOAT4 e_color = {};
	//X,Y,Z全て[-5.0,+5.0]でランダムに分布
	float md_pos = 10.0f;
	//X,Y,Z全て[-0.05,+0.05]でランダムに分布
	float md_vel = 0.1f;
	//重力に見立ててYのみ[-0.001f,0]でランダムに分布
	float md_acc = 0.001f;

	std::unique_ptr<ParticleManager> particleMan;
};