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
	LoadModel(SLIME, "slime");
	LoadModel(SLIME_BREAK, "slime"/*_break"*/ );
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = FbxLoader::GetInstance()->LoadModelFromFile(fileName);
	
}
