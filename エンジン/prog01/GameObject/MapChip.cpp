#include "MapChip.h"
#include "CheckBlock.h"
#include "SlimeMath.h"
#include "DebugText.h"
#include "slimeMath.h"
#include "Collision.h"

const std::string MapChip::baseDirectory = "Resources/Map/";

void MapChip::Initialize()
{
	CsvLoad(TEST_MAP, "testMap",63,63);
}

void MapChip::Filnalize()
{
	for (auto& a : mapChipObj)
	{
		a.clear();
	}
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
		return -1;
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
				mapChipObj[nowMap].push_back(std::make_unique<BaseBlock>(Vector3(chipSize * x, 0, -chipSize * z)));
				break;

			case 9:
				startPos[nowMap] = { 200.0f * x, 0.0f, -200.0f * z };
				break;

			case 10:		//1はベースブロック
				mapChipObj[nowMap].push_back(std::make_unique<BaseBlock>(Vector3(chipSize * x, 0, -chipSize * z)));
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

bool MapChip::CheckMapChipToBox2d(Box2DCollider *boxCollider, Vector3 *vel, Vector3 *hitpos, Vector3 *normal)
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
			normal->z = -1.0f;
			hit = true;
		}
	}
	//下側
	if (0 > vel->z) {
		
		if (GetChipNum(nowChipX, down) == 1) {
			hitPositon.z = -down * chipSize + chipSize / 2;
			normal->z = 1.0f;
			hit = true;
		}
	}
	//右側
	if (0 < vel->x) {
		if (GetChipNum(rig, nowChipY) == 1) {
			hitPositon.x = rig * chipSize - chipSize / 2;
			normal->x = -1.0f;
			hit = true;
		}
	}
	//左側
	if (vel->x < 0) {
		
		if (GetChipNum(lef, nowChipY) == 1) {
			hitPositon.x = lef * chipSize + chipSize / 2;
			normal->x = 1.0f;
			hit = true;
		}
	}


	if (hitpos != nullptr) {
		*hitpos = hitPositon;
	}


	return hit;
}

bool MapChip::CheckMapChipAreaToBox2d(Box2DCollider *boxCollider, Vector3 *vel, Vector3 *hitpos, Vector3 *normal)
{
	Vector3 hitPositon = { 0,0,0 };
	Box2DCollider box = *boxCollider;
	int nowChipX = (box.center.x + (chipSize / 2) - fmodf(box.center.x + (chipSize / 2), chipSize)) / chipSize;
	int nowChipY = (-box.center.z - (chipSize / 2) - fmodf(-box.center.z - (chipSize / 2), chipSize)) / chipSize + 1;

	int rig = ((box.center.x + box.Right) - (chipSize / 2) - fmodf((box.center.x + box.Right) - (chipSize / 2), chipSize)) / chipSize + 1;
	int lef = ((box.center.x + box.Left) + (chipSize / 2) - fmodf((box.center.x + box.Left) + (chipSize / 2), chipSize)) / chipSize;
	int up = (-box.center.z + box.Top + (chipSize / 2) - fmodf(-box.center.z + box.Top + (chipSize / 2), chipSize)) / chipSize;
	int down = (-box.center.z + box.Bottom - (chipSize / 2) - fmodf(-box.center.z + box.Bottom - (chipSize / 2), chipSize)) / chipSize + 1;

	bool hit = false;

	//マップの外にいたとき
	if (up <= 0) {
		hitPositon.z = -chipSize / 2;
		normal->z = -1.0f;
		hit = true;
	}
	else if (down >= mapData[nowMap].high - 1) {
		hitPositon.z = -(mapData[nowMap].high - 1) * chipSize /*+ chipSize / 2 + 1*/;
		normal->z = 1.0f;
		hit = true;
	}
	if (lef <= 0) {
		hitPositon.x = chipSize / 2;;
		normal->x = 1.0f;
		hit = true;
	}
	else if (rig >= mapData[nowMap].wide - 1) {
		hitPositon.x = (mapData[nowMap].wide - 1) * chipSize - chipSize / 2 - 1;
		normal->x = -1.0f;
		hit = true;
	}



	if (hitpos != nullptr) {
		*hitpos = hitPositon;
	}
	return hit;
}

bool MapChip::CheckMapChipToSphere2d(SphereCollider *sphereCollider, Vector3 *vel, Vector3 *hitPos)
{
	SphereCollider sphere = *sphereCollider;
	Vector3 center = sphere.center;
	int nowChipX = (center.x + (chipSize / 2) - fmodf(center.x + (chipSize / 2), chipSize)) / chipSize;
	int nowChipY = (-center.z - (chipSize / 2) - fmodf(-center.z - (chipSize / 2), chipSize)) / chipSize + 1;

	int rig = ((center.x + sphere.GetRadius()) - (chipSize / 2) - fmodf((center.x + sphere.GetRadius()) - (chipSize / 2), chipSize)) / chipSize + 1;
	int lef = ((center.x - sphere.GetRadius()) + (chipSize / 2) - fmodf((center.x - sphere.GetRadius()) + (chipSize / 2), chipSize)) / chipSize;
	int up = (-center.z - sphere.GetRadius() + (chipSize / 2) - fmodf(-center.z - sphere.GetRadius() + (chipSize / 2), chipSize)) / chipSize;
	int down = (-center.z + sphere.GetRadius() - (chipSize / 2) - fmodf(-center.z + sphere.GetRadius() - (chipSize / 2), chipSize)) / chipSize + 1;
	
	Vector3 hitPosition = { 0,0,0 };
	bool hit = false;

	//上方向への移動（下側への接触）
	if (0 < vel->z) {
		//ブロードフェイズ
		//横
		for (int x = lef; x <= rig; x++) {
			if (GetChipNum(x, up) == 1) {
				//細かい当たり判定
				for (int offset = -1; offset <= 1; offset +=2) {
					Vector3 cornerPos = {
						(float)(x * chipSize + (chipSize / 2)*offset),
						0,
						(float)(-up * chipSize - (chipSize / 2))
					};
					Sphere mapChipSphere = {
						cornerPos,
						sphere.GetRadius()
					};
					if (Collision::CheckSphere2Point(mapChipSphere, sphere.center)) {
						hitPosition.z = Vector3(cornerPos + Vector3(sphere.center- cornerPos).Normal() * mapChipSphere.radius).z;
						hit = true;
					}
				}
			}
		}
	}
	//下側
	if (0 > vel->z) {
		//ブロードフェイズ
		//横
		for (int x = lef; x <= rig; x++) {
			if (GetChipNum(x, down) == 1) {
				//細かい当たり判定
				for (int offset = -1; offset <= 1; offset += 2) {
					Vector3 cornerPos = {
						(float)(x * chipSize + (chipSize / 2) * offset),
						0,
						(float)(-down * chipSize + (chipSize / 2))
					};
					Sphere mapChipSphere = {
						cornerPos,
						sphere.GetRadius()
					};
					if (Collision::CheckSphere2Point(mapChipSphere, sphere.center)) {
						hitPosition.z = Vector3(cornerPos + Vector3(sphere.center - cornerPos).Normal() * mapChipSphere.radius).z;
						hit = true;
					}
				}
			}
		}
	}
	//右側
	if (0 < vel->x) {
		for (int y = up; y <= down; y++) {
			if (GetChipNum(rig,y) == 1) {
				//細かい当たり判定
				for (int offset = -1; offset <= 1; offset += 2) {
					Vector3 cornerPos = {
						(float)(rig * chipSize - (chipSize / 2)),
						0,
						(float)(-y * chipSize + (chipSize / 2) * offset),
					};
					Sphere mapChipSphere = {
						cornerPos,
						sphere.GetRadius()
					};
					if (Collision::CheckSphere2Point(mapChipSphere, sphere.center)) {
						hitPosition.x = Vector3(cornerPos + Vector3(sphere.center - cornerPos).Normal() * mapChipSphere.radius).x;
						hit = true;
					}
				}
			}
		}
	}
	//左側
	if (vel->x < 0) {
		for (int y = up; y <= down; y++) {
			if (GetChipNum(lef, y) == 1) {
				//細かい当たり判定
				for (int offset = -1; offset <= 1; offset += 2) {
					Vector3 cornerPos = {
						(float)(lef * chipSize + (chipSize / 2)),
						0,
						(float)(-y * chipSize + (chipSize / 2) * offset),
					};
					Sphere mapChipSphere = {
						cornerPos,
						sphere.GetRadius()
					};
					if (Collision::CheckSphere2Point(mapChipSphere, sphere.center)) {
						hitPosition.x = Vector3(cornerPos + Vector3(sphere.center - cornerPos).Normal() * mapChipSphere.radius).x;
						hit = true;
					}
				}
			}
		}
	}
	if (hitPos != nullptr) {
		*hitPos = hitPosition;
	}
	return hit;
}

MapChip::~MapChip()
{
	Filnalize();
}

MapChip* MapChip::GetInstance()
{
	static MapChip instance;
	return &instance;
}