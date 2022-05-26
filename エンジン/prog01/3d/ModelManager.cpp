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
	LoadModel(PLAYER, "player");
	
	LoadModel(DEFAULT_BLOCK, "testBlock");


	LoadModel(CHECK_BLOCK, "checkBlock");

	LoadModel(SLIME, "slime");
	model[SLIME].get()->SetMetalness(0.2f);
	model[SLIME].get()->SetSpecular(0.4f);
	model[SLIME].get()->SetRoughness(0.4f);
	model[SLIME].get()->SetAlpha(0.6f);
	model[SLIME].get()->TransferMaterial();
	LoadModel(SLIME_BREAK, "slime"/*_break"*/ );
	model[SLIME_BREAK].get()->SetBaseColor({ 1.0f,1.0f,0.0f});
	LoadModel(SLIME_CORE,"core");

	LoadModel(SHOTBOSS, "slime");
	model[SHOTBOSS].get()->SetMetalness(0.2f);
	model[SHOTBOSS].get()->SetSpecular(0.4f);
	model[SHOTBOSS].get()->SetRoughness(0.4f);
	model[SHOTBOSS].get()->SetAlpha(0.8f);
	model[SHOTBOSS].get()->TransferMaterial();


	//LoadModel(ENEMY_ROBO_1, "robo_1"/*_break"*/);
	//model[ENEMY_ROBO_1].get()->SetMetalness(0.0f);
	//model[ENEMY_ROBO_1].get()->SetSpecular(0.1f);
	//model[ENEMY_ROBO_1].get()->SetRoughness(0.1f);
	//model[ENEMY_ROBO_1].get()->TransferMaterial();

	//LoadModel(ENEMY_ROBO_2, "robo_2"/*_break"*/);
	//model[ENEMY_ROBO_2].get()->SetMetalness(0.0f);
	//model[ENEMY_ROBO_2].get()->SetSpecular(0.1f);
	//model[ENEMY_ROBO_2].get()->SetRoughness(0.1f);
	//model[ENEMY_ROBO_2].get()->TransferMaterial();

	//LoadModel(ENEMY_ROBO_3, "robo_3"/*_break"*/);
	//model[ENEMY_ROBO_3].get()->SetMetalness(0.0f);
	//model[ENEMY_ROBO_3].get()->SetSpecular(0.1f);
	//model[ENEMY_ROBO_3].get()->SetRoughness(0.1f);
	//model[ENEMY_ROBO_3].get()->TransferMaterial();

	//LoadModel(ENEMY_ROBO_4, "robo_4"/*_break"*/);
	//model[ENEMY_ROBO_4].get()->SetMetalness(0.0f);
	//model[ENEMY_ROBO_4].get()->SetSpecular(0.1f);
	//model[ENEMY_ROBO_4].get()->SetRoughness(0.1f);
	//model[ENEMY_ROBO_4].get()->TransferMaterial();

	LoadModel(ENEMY_ROBO_5, "robo_5"/*_break"*/);
	model[ENEMY_ROBO_5].get()->SetMetalness(0.0f);
	model[ENEMY_ROBO_5].get()->SetSpecular(0.1f);
	model[ENEMY_ROBO_5].get()->SetRoughness(0.1f);
	model[ENEMY_ROBO_5].get()->TransferMaterial();

	//LoadModel(ENEMY_ROBO_6, "robo_6"/*_break"*/);
	//model[ENEMY_ROBO_6].get()->SetMetalness(0.0f);
	//model[ENEMY_ROBO_6].get()->SetSpecular(0.1f);
	//model[ENEMY_ROBO_6].get()->SetRoughness(0.1f);
	//model[ENEMY_ROBO_6].get()->TransferMaterial();

	//LoadModel(ENEMY_ROBO_7, "robo_7"/*_break"*/);
	//model[ENEMY_ROBO_7].get()->SetMetalness(0.0f);
	//model[ENEMY_ROBO_7].get()->SetSpecular(0.1f);
	//model[ENEMY_ROBO_7].get()->SetRoughness(0.1f);
	//model[ENEMY_ROBO_7].get()->TransferMaterial();

	LoadModel(ENEMY_ROBO_8, "robo_8"/*_break"*/);
	model[ENEMY_ROBO_8].get()->SetMetalness(0.0f);
	model[ENEMY_ROBO_8].get()->SetSpecular(0.1f);
	model[ENEMY_ROBO_8].get()->SetRoughness(0.1f);
	model[ENEMY_ROBO_8].get()->TransferMaterial();

	LoadModel(ENEMY_HP_FRAME, "EnemyHp"/*_break"*/);
	LoadModel(ENEMY_HP_GAGE, "EnemyHpGage"/*_break"*/);

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
