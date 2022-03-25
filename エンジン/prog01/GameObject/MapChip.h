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
	//CSV�t�@�C���ǂݍ���
	void CsvLoad(MAP_NAME mapName, std::string fName, int mapChipMaxX, int mapChipMaxY);
	//�}�b�v�i���o�[�w��
	int GetChipNum(int x, int y, MAP_NAME mapName);
	//�I�u�W�F�N�g����
	void CreateStage(MAP_NAME mapName);
	
	//�X�V
	void Update(MAP_NAME mapName);
	//�K��
	void Adaptation(MAP_NAME mapName);
	//�`��
	void Draw(MAP_NAME mapName);


private:
	//�}�b�v�f�[�^
	MAP_DATA mapData[MAX];

	//�I�u�W�F�N�g�f�[�^
	std::vector<BaseBlock *> mapChipObj[MAX];
};