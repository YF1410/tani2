#pragma once
#include "AbstractSceneFactory.h"

class SceneFactory : public AbstractSceneFactory
{
public:
	//シーン生成
	BaseScene* CreateScene(const std::string& sceneName, int parameter = 0) override;
};