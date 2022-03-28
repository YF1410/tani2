#include "MapChip.h"
#include "CheckBlock.h"
#include "SlimeMath.h"

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

bool MapChip::CollisionRectAndMapchipEdgeVersion(BOX2D &src_rect, Vector3 velocity, EdgeType &contact_edge, float &contact_edge_position, MAP_NAME name)
{
	BOX2D rect = src_rect;
	// 矩形のX軸範囲
	int width_range_ids[]
	{
		(int)(rect.Left / chipSize),
		(int)(rect.Right / chipSize + 1)
	};

	// 矩形のY軸範囲
	int height_range_ids[]
	{
		(int)(rect.Top / chipSize),
		(int)(rect.Bottom / chipSize + 1)
	};

	for (int i = 0; i < 2; i++) {
		Clamp(&height_range_ids[i], 0, mapData[name].high);
		Clamp(&width_range_ids[i], 0, mapData[name].wide);
	}
	const int start = 0;
	const int end = 1;

	struct Vec2 {
		int x;
		int y;
		Vec2(int x, int y) {
			this->x = x;
			this->y = y;
		}
	};
	Vec2 edge_list[(int)EdgeType::EdgeTypeMax][2]{

		// 左辺
		{
			Vec2(width_range_ids[start], height_range_ids[start]),
			Vec2(width_range_ids[start], height_range_ids[end])
		},

		// 右辺
		{
			Vec2(width_range_ids[end], height_range_ids[start]),
			Vec2(width_range_ids[end], height_range_ids[end])
		},
		// 上辺
		{
			Vec2(width_range_ids[start], height_range_ids[start]),
			Vec2(width_range_ids[end], height_range_ids[start])
		},

		// 下辺
		{
			Vec2(width_range_ids[start], height_range_ids[end]),
			Vec2(width_range_ids[end], height_range_ids[end])
		},
	};

	bool is_use_edge_list[(int)EdgeType::EdgeTypeMax]
	{
		velocity.x < 0.0f ? true : false,	// LEFT
		velocity.x > 0.0f ? true : false,	// RIGHT
		velocity.z > 0.0f ? true : false, // BOTTOM
		velocity.z < 0.0f ? true : false, // TOP
	};


	for (int i = 0; i < (int)EdgeType::EdgeTypeMax; i++) {
		// 使わない辺は判定しない
		if (is_use_edge_list[i] == false) {
			continue;
		}
		for (int y = edge_list[i][start].y; y <= edge_list[i][end].y; y++)
		{
			for (int x = edge_list[i][start].x; x <= edge_list[i][end].x; x++)
			{
				if (GetChipNum(x, y) == 1)
				{
					// 接触した辺の座標を返す(座標はX or Yのどちらか)
					Vec2 chipPos = Vec2(x * chipSize, -y * chipSize);
					switch (EdgeType(i))
					{
					case EdgeTypeLeft:
						contact_edge = EdgeType::EdgeTypeRight;
						contact_edge_position = chipPos.x + chipSize / 2.0f;
						break;
					case EdgeTypeRight:
						contact_edge = EdgeType::EdgeTypeLeft;
						contact_edge_position = chipPos.x - chipSize / 2.0f;
						break;
					case EdgeTypeTop:
						contact_edge = EdgeType::EdgeTypeBottom;
						contact_edge_position = chipPos.y - chipSize/2.0f;
						break;
					case EdgeTypeBottom:
						contact_edge = EdgeType::EdgeTypeTop;
						contact_edge_position = chipPos.y + chipSize / 2.0f;
						break;
					default:
						break;
					}

					// 当たり
					return true;
				}
			}
		}
	}
	return false;
}

MapChip* MapChip::GetInstance()
{
	static MapChip instance;
	return &instance;
}