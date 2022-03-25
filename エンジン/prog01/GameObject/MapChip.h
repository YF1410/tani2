#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <map>
#include "BaseBlock.h"

class MapChip
{
private:
	MapChip() = default;
	~MapChip() = default;
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
	//CSVファイル読み込み
	void CsvLoad(MAP_NAME mapName, std::string fName, int mapChipMaxX, int mapChipMaxY);
	//マップナンバー指定
	int GetChipNum(int x, int y, MAP_NAME mapName);
	//オブジェクト生成
	void CreateStage(MAP_NAME mapName);
	
	//更新
	void Update(MAP_NAME mapName);
	//適応
	void Adaptation(MAP_NAME mapName);
	//描画
	void Draw(MAP_NAME mapName);


private:
	//マップデータ
	MAP_DATA mapData[MAX];

	//オブジェクトデータ
	std::vector<BaseBlock *> mapChipObj[MAX];
};