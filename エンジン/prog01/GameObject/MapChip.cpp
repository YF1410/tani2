#include "MapChip.h"

const std::string MapChip::baseDirectory = "Resources/Map/";

void MapChip::Initialize()
{
	CsvLoad(TEST_MAP, "testMap",33,20);
}

void MapChip::CsvLoad(MAP_NAME mapName, std::string fName, int mapChipMaxX, int mapChipMaxY)
{
	std::ifstream ifs(baseDirectory + fName + ".csv");
	MAP_DATA loadData;
	std::string line;
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
			loadData.mapChip.push_back(i);
		}
	}
	//�t�@�C������ۑ�
	loadData.mapName = baseDirectory + fName + ".csv";
	//X�̍ő�l��ۑ�
	loadData.wide = mapChipMaxX;
	//Y�̍ő�l��ۑ�
	loadData.high = mapChipMaxY;

	//�}�b�v�l�[���ɑΉ������ʒu�Ɋi�[
	mapData[mapName] = loadData;
}

int MapChip::GetChipNum(int x, int y, MAP_NAME mapName)
{
	if (x < 0 || x >= mapData[mapName].wide || y < 0 || y >= mapData[mapName].high)
	{
		assert(0);
	}

	std::vector<int> map = mapData[mapName].mapChip;

	return map[y * mapData[mapName].wide + x];
}

void MapChip::CreateStage(MAP_NAME mapName)
{
	for (int y = 0; y < mapData[mapName].high; y++) {
		for (int x = 0; x < mapData[mapName].wide; x++) {
			switch (MapChip::GetInstance()->GetChipNum(x, y, mapName))
			{
			case 0:		//0�ɂ͉����z�u���Ȃ�
				break;
			case 1:		//1�̓x�[�X�u���b�N
				mapChipObj[mapName].push_back(new BaseBlock(Vector3(200 * x, -200 * y, 0)));

			default:
				break;
			}
		}
	}
}

void MapChip::Update(MAP_NAME mapName)
{
	for (int i = 0; i < mapChipObj[mapName].size(); i++) {
		mapChipObj[mapName][i]->Update();
	}
}

void MapChip::Adaptation(MAP_NAME mapName)
{
	for (int i = 0; i < mapChipObj[mapName].size(); i++) {
		mapChipObj[mapName][i]->Adaptation();
	}
}

void MapChip::Draw(MAP_NAME mapName)
{
	for (int i = 0; i < mapChipObj[mapName].size(); i++) {
		mapChipObj[mapName][i]->Draw();
	}
}

MapChip* MapChip::GetInstance()
{
	static MapChip instance;
	return &instance;
}