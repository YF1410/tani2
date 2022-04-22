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
		DEFAULT_BLOCK,
		CHECK_BLOCK,
		

		SLIME,
		SLIME_BREAK,
		SLIME_CORE,
		SLIME_CORE_UNDER,

		ENEMY_MACARON,


		//�ő吔
		MAX
	};

public:
	static ModelManager *GetIns();		//�擾�p

	void Initialize();
	void Finalize();
	void LoadModel(const ModelName modelName, std::string fileName);
	FbxModel *GetModel(ModelName modelName) { return model[modelName].get(); }

private:
	std::map<ModelManager::ModelName, std::unique_ptr<FbxModel>> model;	//���f���i�[�}�b�v
};

