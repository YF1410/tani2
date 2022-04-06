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
	LoadModel(CHECK_BLOCK, "checkBlock");

	LoadModel(SLIME, "slime");
	model[SLIME].get()->SetMetalness(0.6f);
	model[SLIME].get()->SetSpecular(0.8f);
	model[SLIME].get()->SetRoughness(0.3f);
	model[SLIME].get()->SetAlpha(0.6f);
	model[SLIME].get()->TransferMaterial();
	LoadModel(SLIME_BREAK, "slime"/*_break"*/ );
	model[SLIME_BREAK].get()->SetMetalness(0.6f);
	model[SLIME_BREAK].get()->SetSpecular(0.8f);
	model[SLIME_BREAK].get()->SetRoughness(0.3f);
	model[SLIME_BREAK].get()->SetAlpha(1.0f);
	model[SLIME_BREAK].get()->TransferMaterial();
	LoadModel(SLIME_CORE,"core");



	LoadModel(ENEMY, "human"/*_break"*/);
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = FbxLoader::GetInstance()->LoadModelFromFile(fileName);
	
}
