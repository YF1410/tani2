#include "MapChip.h"
#include "CheckBlock.h"
#include "SlimeMath.h"
#include "DebugText.h"
#include "slimeMath.h"

const std::string MapChip::baseDirectory = "Resources/Map/";

void MapChip::Initialize()
{
	CsvLoad(TEST_MAP, "testMap",57,34);
}

void MapChip::CsvLoad(MAP_NAME nowMap, std::string fName, int mapChipMaxX, int mapChipMaxY)
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
	//ファイル名を保存
	loadData.mapName = baseDirectory + fName + ".csv";
	//Xの最大値を保存
	loadData.wide = mapChipMaxX;
	//Yの最大値を保存
	loadData.high = mapChipMaxY;

	//マップネームに対応した位置に格納
	mapData[nowMap] = loadData;
}

int MapChip::GetChipNum(int x, int y)
{
	if (x < 0 || x >= mapData[nowMap].wide || y < 0 || y >= mapData[nowMap].high)
	{
		return NULL;
	}

	std::vector<int> map = mapData[nowMap].mapChip;

	return map[y * mapData[nowMap].wide + x];
}

void MapChip::CreateStage()
{
	for (int z = 0; z < mapData[nowMap].high; z++) {
		for (int x = 0; x < mapData[nowMap].wide; x++) {
			switch (MapChip::GetInstance()->GetChipNum(x, z))
			{
			case 0:		//0には何も配置しない
				break;
			case 1:		//1はベースブロック
				mapChipObj[nowMap].push_back(new BaseBlock(Vector3(chipSize * x, 0, -chipSize * z)));
				break;

			case 9:
				startPos[nowMap] = { 200.0f * x, 0.0f, -200.0f * z };
				break;

			case 10:		//1はベースブロック
				mapChipObj[nowMap].push_back(new CheckBlock(Vector3(chipSize * x, 0, -chipSize * z)));
				break;
			default:
				break;
			}
		}
	}
}

void MapChip::Update()
{
	for (int i = 0; i < mapChipObj[nowMap].size(); i++) {
		mapChipObj[nowMap][i]->Update();
	}
}

void MapChip::Adaptation()
{
	for (int i = 0; i < mapChipObj[nowMap].size(); i++) {
		mapChipObj[nowMap][i]->Adaptation();
	}
}

void MapChip::Draw()
{
	for (int i = 0; i < mapChipObj[nowMap].size(); i++) {
		mapChipObj[nowMap][i]->Draw();
	}
}

bool MapChip::CheckHitMapChip(Box2DCollider *boxCollider,Vector3 *vel, Vector3 *hitpos)
{
	Vector3 hitPositon = { 0,0,0 };
	Box2DCollider box = *boxCollider;
	int nowChipX = (box.center.x + (chipSize / 2) - fmodf(box.center.x + (chipSize / 2),chipSize)) / chipSize;
	int nowChipY = (-box.center.z - (chipSize / 2) - fmodf(-box.center.z - (chipSize / 2), chipSize)) / chipSize + 1;

	int rig = ((box.center.x + box.Right) - (chipSize / 2) - fmodf((box.center.x + box.Right) - (chipSize / 2), chipSize)) / chipSize + 1;
	int lef = ((box.center.x + box.Left) + (chipSize / 2) - fmodf((box.center.x + box.Left) + (chipSize / 2), chipSize)) / chipSize;
	int up = (-box.center.z + box.Top + (chipSize / 2) - fmodf(-box.center.z + box.Top + (chipSize / 2), chipSize)) / chipSize;
	int down = (-box.center.z + box.Bottom - (chipSize / 2) - fmodf(-box.center.z + box.Bottom - (chipSize / 2), chipSize)) / chipSize + 1;

	bool hit = false;
	//上方向への移動があるとき
	if (0 < vel->z) {
		if (GetChipNum(nowChipX, up) == 1) {
			hitPositon.z = -up * chipSize - chipSize / 2;
			hit = true;
		}
	}
	//下側
	if (0 > vel->z) {
		if (GetChipNum(nowChipX, down) == 1) {
			hitPositon.z = -down * chipSize + chipSize / 2;
			hit = true;

		}
	}
	//右側
	if (0 < vel->x) {
		if (GetChipNum(rig, nowChipY) == 1) {
			hitPositon.x = rig * chipSize - chipSize / 2;
			hit = true;
		}
	}
	//左側
	if (vel->x < 0) {
		if (GetChipNum(lef, nowChipY) == 1) {
			hitPositon.x = lef * chipSize + chipSize / 2;
			hit = true;
		}
	}
	if (hitpos != nullptr) {
		*hitpos = hitPositon;
	}

	//Vector3 normal;
	//if (hitPositon.x != 0) {
	//	int vec = 1;	//向き
	//	int offset = -1;
	//	if (0 < vel->x) {
	//		vec = -1;
	//		offset = 0;
	//	}
	//	boxCollider->GetObject3d()->pos.x = hitPositon.x + boxCollider->GetRadiusX() * vec + offset;
	//	normal.x = offset;
	//}
	//if (hitPositon.z != 0) {
	//	int vec = 1;	//向き
	//	int offset = 0;
	//	if (vel->z < 0) {
	//		vec = -1;
	//		//offset = -1;
	//	}
	//	boxCollider->GetObject3d()->pos.z = hitPositon.z - boxCollider->GetRadiusY() * vec + offset;
	//	normal.y = offset;
	//}

	return hit;
}

MapChip* MapChip::GetInstance()
{
	static MapChip instance;
	return &instance;
}