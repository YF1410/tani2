#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <map>
#include "BaseBlock.h"
#include "Box2dCollider.h"
#include "SphereCollider.h"

class MapChip
{
private:
	MapChip() = default;
	~MapChip();
	MapChip(const MapChip & mapChip) = delete;
	MapChip &operator=(const MapChip & mapChip) = delete;

public:
	enum MAP_NAME {
		TEST_MAP,
		MAX
	};

	struct MAP_DATA {
		std::string mapName;				//マップの名前
		std::vector<int> mapChip;	//マップチップのデータ
		int wide;					//マップチップの横幅
		int high;					//マップチップの縦幅
	};

public:
	// ルートパス
	static const std::string baseDirectory;

public:
	static MapChip* GetInstance();
	//全ステージデータ読み込み
	void Initialize();
	void Filnalize();
	//CSVファイル読み込み
	void CsvLoad(MAP_NAME mapName, std::string fName, int mapChipMaxX, int mapChipMaxY);
	
	//マップをセット
	void SetMapName(MAP_NAME mapName) { nowMap = mapName; }
	//マップナンバー指定
	int GetChipNum(int x, int y);
	//オブジェクト生成
	void CreateStage();
	
	//更新
	void Update();
	//適応
	void Adaptation();
	//描画
	void Draw();

	XMFLOAT3 GetStartPos() { return startPos[nowMap]; }

	MAP_DATA GetMapData() { return mapData[nowMap]; }
	

	//マップチップと矩形の当たり判定
	bool CheckMapChipToBox2d(Box2DCollider *boxCollider, Vector3 *vel, Vector3 *hitpos, Vector3 *normal);
	bool CheckMapChipAreaToBox2d(Box2DCollider *boxCollider, Vector3 *vel, Vector3 *hitpos);
	//マップチップと球の当たり判定
	bool CheckMapChipToSphere2d(SphereCollider *sphereCollider,Vector3 *vel,Vector3 *hitPos);

	//Blockのサイズ
	const int chipSize = 200;
private:
	//現在のマップ
	MAP_NAME nowMap;
	//マップデータ
	MAP_DATA mapData[MAX];


	//オブジェクトデータ
	std::vector<std::unique_ptr<BaseBlock>> mapChipObj[MAX];

	//プレイヤーのスタートポジション
	XMFLOAT3 startPos[MAX];
};