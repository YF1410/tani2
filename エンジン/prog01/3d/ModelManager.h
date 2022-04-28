#pragma once
#include <map>
#include <string>
#include "FbxModel.h"

class ModelManager final
{
private:
	ModelManager() = default;
	~ModelManager();
	ModelManager(const ModelManager &r) = default;
	ModelManager &operator= (const ModelManager &r) = default;

public:
	enum ModelName {
		TESTS_TAGE,
		DEFAULT_BLOCK,
		CHECK_BLOCK,
		

		SLIME,
		SLIME_BREAK,
		SLIME_CORE,
		SLIME_CORE_UNDER,

		ENEMY_ROBO_1,
		ENEMY_ROBO_2,
		ENEMY_ROBO_3,
		ENEMY_ROBO_4,
		ENEMY_ROBO_5,
		ENEMY_ROBO_6,
		ENEMY_ROBO_7,
		ENEMY_ROBO_8,


		//最大数
		MAX
	};

public:
	static ModelManager *GetIns();		//取得用

	void Initialize();
	void Finalize();
	void LoadModel(const ModelName modelName, std::string fileName);
	FbxModel *GetModel(ModelName modelName) { return model[modelName].get(); }

private:
	std::map<ModelManager::ModelName, std::unique_ptr<FbxModel>> model;	//モデル格納マップ
};

