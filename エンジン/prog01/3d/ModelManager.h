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
		
		PLAYER,
		DEBRI,

		SLIME,
		SLIME_BREAK,
		SLIME_CORE,
		SLIME_CORE_UNDER,
		SHOTBOSS,

		ENEMY_ROBO_5,
		ENEMY_ROBO_8,

		ENEMY_HP_FRAME,
		ENEMY_HP_GAGE,

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

