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
		TUTORIAL,
		Stage1,
		Stage2,
		Stage3,
		Stage4,
		Stage5,
		MAX
	};

	struct MAP_DATA {
		std::string mapName;				//�}�b�v�̖��O
		std::vector<int> mapChip;	//�}�b�v�`�b�v�̃f�[�^
		int wide;					//�}�b�v�`�b�v�̉���
		int high;					//�}�b�v�`�b�v�̏c��
	};

public:
	// ���[�g�p�X
	static const std::string baseDirectory;

public:
	static MapChip* GetInstance();
	//�S�X�e�[�W�f�[�^�ǂݍ���
	void Initialize();
	void Filnalize();
	//CSV�t�@�C���ǂݍ���
	void CsvLoad(MAP_NAME mapName, std::string fName);
	
	//�}�b�v���Z�b�g
	void SetMapName(MAP_NAME mapName) { nowMap = mapName; }
	//�}�b�v�i���o�[�w��
	int GetChipNum(int x, int y);
	//�I�u�W�F�N�g����
	void CreateStage(MAP_NAME mapName);
	
	//�X�V
	void Update();
	//�K��
	void Adaptation();
	//�`��
	void Draw();

	XMFLOAT3 GetStartPos() { return startPos[nowMap]; }

	MAP_DATA GetMapData() { return mapData[nowMap]; }
	

	//�}�b�v�`�b�v�Ƌ�`�̓����蔻��
	bool CheckMapChipToBox2d(Box2DCollider *boxCollider, Vector3 *vel, Vector3 *hitpos, Vector3 *normal,Vector3 *oldPos);
	bool CheckMapChipAreaToBox2d(Box2DCollider *boxCollider, Vector3 *vel, Vector3 *hitpos, Vector3 *normal);
	//�}�b�v�`�b�v�Ƌ��̓����蔻��
	bool CheckMapChipToSphere2d(SphereCollider *sphereCollider,Vector3 *vel,Vector3 *hitPos);

	//Block�̃T�C�Y
	const int chipSize = 200;
	//���݂̃}�b�v
	MAP_NAME nowMap;
private:
	//�}�b�v�f�[�^
	MAP_DATA mapData[MAX];


	//�I�u�W�F�N�g�f�[�^
	std::vector<std::unique_ptr<BaseBlock>> mapChipObj[MAX];

	//�v���C���[�̃X�^�[�g�|�W�V����
	XMFLOAT3 startPos[MAX];
};