#pragma once
#include "AbstractSceneFactory.h"

class SceneFactory : public AbstractSceneFactory
{
public:
	//ÉVÅ[Éìê∂ê¨
	BaseScene* CreateScene(const std::string& sceneName, int parameter = 0) override;
};