#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include "Collision.h"

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
	PlayerObject(FbxModel *coreModel = nullptr);
	~PlayerObject();
	//初期化
	void Init();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();

	//残骸を吸収した時
	void Absorb(float size);
	
	//当たり判定
	struct COLLIDER {
		Sphere absorbSphere;
	}collider;

	float size;	//質量


private: // メンバ変数
	std::unique_ptr<FbxObject3d> slime;
	//キーボード移動用
	float moveSpead = 30.0f;
	
	//プレイヤーの基準座標
	Vector3 pos;
	//総移動量
	Vector3 moveVec;

	//サイズ
	float scale;//大きさ

	//自爆タイプ
	DESTRUCT_TYPE destructType;
	//自爆力
	DESTRUCT_POW destructPow;
	//自爆フラグ
	bool destructFlag = false;


};

