#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	Node* parent = nullptr;
};

//テクスチャデータ
struct TextureData
{
	//テクスチャメタデータ
	DirectX::TexMetadata metaData = {};
	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	//SRVのGPUハンドル
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};

class FbxModel
{
public:
	//フレンドクラス
	friend class FbxLoader;

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:// 定数
	//ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;
	//テクスチャの最大数
	static const int MAX_TEXTURES = 4;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;              //xyz座標
		DirectX::XMFLOAT3 normal;           //法線ベクトル
		DirectX::XMFLOAT2 uv;               //uv座標
		UINT boneIndex[MAX_BONE_INDICES];   //ボーン番号
		float boneWeight[MAX_BONE_INDICES]; //ボーン重み
	};

	//ボーン構造体
	struct Bone
	{
		//名前
		std::string name;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		//コンストラクタ
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

	//定数バッファ用のデータ構造体(マテリアル)
	struct ConstBufferDataMaterial
	{
		//アルベド
		XMFLOAT3 baseColor;
		//金属度
		float metalness;
		//鏡面反射度
		float specular;
		//粗さ
		float roughness;
		//アルファ
		float alpha;
		//パディング(16Byte境界)
		float pad[2];
	};

public:
	//バッファ生成
	void CreateBuffers(ID3D12Device* device);
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//モデルの変形行列取得
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	//マテリアルパラメータ転送
	void TransferMaterial();
	//テクスチャバッファ生成
	void CreateTexture(TextureData& texture, ID3D12Device* device, int srvIndex);
	//getrer
	std::vector<Bone>& GetBones() { return bones; }
	FbxScene* GetFbxScene() { return fbxScene; }
	const XMFLOAT3& GetBaseColor() { return baseColor; }
	float GetMetalness() { return metalness; }
	float GetSpecular() { return specular; }
	float GetRoughness() { return roughness; }
	std::wstring GetHLSLfName() { return HLSLfName; }
	//setter
	void SetBaseColor(const XMFLOAT3& _baseColor) { baseColor = _baseColor; TransferMaterial(); }
	void SetMetalness(float _metalness) { metalness = _metalness;TransferMaterial(); }
	void SetSpecular(float _specular) { specular = _specular;TransferMaterial(); }
	void SetRoughness(float _roughness) { roughness = _roughness;TransferMaterial(); }
	void SetAlpha(float _alpha) { alpha = _alpha;TransferMaterial(); }
	//頂点情報の取得
	std::vector<VertexPosNormalUvSkin> *GetVertices() { return &vertices; }
	//インデックスの取得
	std::vector<unsigned short> *GetIndces() { return &indices; }

	//デストラクタ
	~FbxModel();

private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
	//ボーン配列
	std::vector<Bone> bones;
	//FBXシーン
	FbxScene* fbxScene = nullptr;
	//メッシュを持つノード
	Node* meshNode = nullptr;
	//頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	//頂点インデックス配列
	std::vector<unsigned short> indices;
	//アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	//ディフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;


private:
	//アルベド
	XMFLOAT3 baseColor = { 1,1,1 };
	//金属度
	float metalness = 0.0f;
	//鏡面反射度
	float specular = 1.0f;
	//粗さ
	float roughness = 1.0f;
	//アルファ
	float alpha = 1.0f;
	//定数バッファ(マテリアル)
	ComPtr<ID3D12Resource> constBufferMaterial;
	//ベーステクスチャ
	TextureData baseTexture;
	//メタルネステクスチャ
	TextureData metalnessTexture;
	//法線テクスチャ
	TextureData normalTexture;
	//ラフネステクスチャ
	TextureData roughnessTexture;
	//シェーダの名前
	std::wstring HLSLfName;

	// メッシュコンテナ
	//std::vector<std::unique_ptr<Mesh>> meshes;
};