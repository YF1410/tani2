#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "SelectScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName, int parameter)
{
	//���̃V�[���𐶐�
	BaseScene* newScene = nullptr;
	
	if (sceneName == "TitleScene")
	{
		newScene = new TitleScene();
	}
	else if (sceneName == "SelectScene")
	{
		newScene = new SelectScene();
	}
	else if (sceneName == "GameScene")
	{
		newScene = new GameScene(parameter);
	}
	else if (sceneName == "ClearScene")
	{
		newScene = new ClearScene();
	}
	
	return newScene;
}
