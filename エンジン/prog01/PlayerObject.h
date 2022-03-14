#pragma once
#include "Object3d.h"
#include "Slime.h"
#include "Vector3.h"

enum DESTRUCT_TYPE {
	CIRCLE,			//全方向
	DIRECTIVITY		//指向性
};

enum DESTRUCT_POW {
	WEAK	= 15,	//弱い
	STRONG	= 30	//強い
};

class PlayerObject
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

public:
	PlayerObject(Model* model = nullptr, Model* model2 = nullptr);
	~PlayerObject();
	//初期化
	void Init();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();

private: // メンバ変数
	Slime* slime = nullptr;

	std::unique_ptr<Object3d> sphereOBJ;
	std::unique_ptr<Model> sphereModel;

	float moveSpead = 4.0f;
	
	//プレイヤーの基準座標
	Vector3 pos;
	//総移動量
	Vector3 moveVec;
	//移動量
	float speed = 1.0f;

	//自爆タイプ
	DESTRUCT_TYPE destructType;
	//自爆力
	DESTRUCT_POW destructPow;
	//自爆フラグ
	bool destructFlag = false;


};

