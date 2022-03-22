#pragma once
#include <map>
#include <string>
#include "FbxModel.h"

class ModelManager final
{
private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager &r) = default;
	ModelManager &operator= (const ModelManager &r) = default;

public:
	enum ModelName {
		TESTS_TAGE,
		TESTS_MAP_BLOCK,
		SLIME,
		SLIME_BREAK,
		ENEMY,

		//最大数
		MAX
	};

public:
	static ModelManager *GetIns();		//取得用

	void Initialize();
	void LoadModel(const ModelName modelName, std::string fileName);
	FbxModel *GetModel(ModelName modelName) { return model[modelName].get(); }

private:
	static std::map<ModelName, std::unique_ptr<FbxModel>> model;	//モデル格納マップ
};

