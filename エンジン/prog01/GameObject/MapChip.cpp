#include "MapChip.h"

void MapChip::CsvLoad(int mapChipMaxX, int mapChipMaxY, std::string fName)
{
	std::ifstream ifs(fName);
	std::string line;
	std::vector<int> csv;
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string field;
		std::vector<int> result;
		while (std::getline(stream, field, ','))
		{
			result.push_back(stoi(field));
		}
		for (auto i : result)
		{
			csv.push_back(i);
		}
	}
	this->mapChipMaxX.push_back(mapChipMaxX);
	this->mapChipMaxY.push_back(mapChipMaxY);
	mapData.push_back(csv);
}

int MapChip::GetChipNum(int x, int y, int mapChipSize, int mapNumber)
{
	const int X = x / mapChipSize;
	const int Y = y / mapChipSize;

	if (X < 0 || X >= mapChipMaxX[mapNumber] || Y < 0 || Y >= mapChipMaxY[mapNumber])
	{
		assert(0);
	}

	std::vector<int> map = mapData[mapNumber];

	return map[Y * mapChipMaxX[mapNumber] + X];
}