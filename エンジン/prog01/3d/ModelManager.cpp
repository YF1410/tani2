#include "ModelManager.h"
#include "FbxLoader.h"

std::map<ModelName, std::unique_ptr<FbxModel>> ModelManager::model;

ModelManager *ModelManager::GetIns()
{
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize()
{
	LoadModel(TESTS_TAGE, "stage");
	LoadModel(TESTS_MAP_BLOCK, "testBlock");
	LoadModel(SLIME, "slime");
	LoadModel(SLIME_BREAK, "slime"/*_break"*/ );
	LoadModel(ENEMY, "human"/*_break"*/);
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = FbxLoader::GetInstance()->LoadModelFromFile(fileName);
	
}
