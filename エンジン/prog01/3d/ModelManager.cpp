#include "ModelManager.h"
#include "FbxLoader.h"

std::map<ModelManager::ModelName, std::unique_ptr<FbxModel>> ModelManager::model;

ModelManager *ModelManager::GetIns()
{
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize()
{
	LoadModel(TESTS_TAGE, "stage");
	LoadModel(DEFAULT_BLOCK, "testBlock");
	LoadModel(PASSING_BLOCK, "passingBlock");
	LoadModel(SLIME, "slime");
	LoadModel(SLIME_BREAK, "slime"/*_break"*/ );
	LoadModel(ENEMY, "human"/*_break"*/);
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = FbxLoader::GetInstance()->LoadModelFromFile(fileName);
	
}
