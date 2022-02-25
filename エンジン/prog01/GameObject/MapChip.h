#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

class MapChip
{
public:
	//CSVファイル読み込み
	void CsvLoad(int mapChipMaxX, int mapChipMaxY, std::string fName);
	//マップナンバー指定
	int GetChipNum(int x, int y, int mapChipSize, int mapNumber);

private:
	std::vector<std::vector<int>> mapData;
	std::vector<int> mapChipMaxX;
	std::vector<int> mapChipMaxY;
};