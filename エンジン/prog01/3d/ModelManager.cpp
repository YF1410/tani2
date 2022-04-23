#include "ModelManager.h"
#include "FbxLoader.h"

//std::map<ModelManager::ModelName, std::unique_ptr<FbxModel>> ModelManager::model;

ModelManager::~ModelManager()
{
	Finalize();
}

ModelManager *ModelManager::GetIns()
{
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize()
{

	LoadModel(SLIME, "slime");
	model[SLIME].get()->SetMetalness(0.2f);		
	model[SLIME].get()->SetSpecular(0.4f);
	model[SLIME].get()->SetRoughness(0.4f);
	model[SLIME].get()->SetAlpha(0.8f);
	model[SLIME].get()->TransferMaterial();

	LoadModel(TESTS_TAGE, "stage");
	LoadModel(DEFAULT_BLOCK, "testBlock");
	LoadModel(CHECK_BLOCK, "checkBlock");

	LoadModel(SLIME_BREAK, "slime"/*_break"*/ );
	model[SLIME_BREAK].get()->SetMetalness(0.2f);
	model[SLIME_BREAK].get()->SetSpecular(0.4f);
	model[SLIME_BREAK].get()->SetRoughness(0.4f);
	model[SLIME_BREAK].get()->SetAlpha(0.8f);
	model[SLIME_BREAK].get()->TransferMaterial();
	LoadModel(SLIME_CORE,"core");



	LoadModel(ENEMY_MACARON, "macaron"/*_break"*/);
	model[ENEMY_MACARON].get()->SetMetalness(0.0f);
	model[ENEMY_MACARON].get()->SetSpecular(0.1f);
	model[ENEMY_MACARON].get()->SetRoughness(0.1f);
	model[ENEMY_MACARON].get()->TransferMaterial();


}

void ModelManager::Finalize()
{
	for (auto& a : model)
	{
		a.second.reset();
	}
	model.clear();
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = FbxLoader::GetInstance()->LoadModelFromFile(fileName);
	
}
