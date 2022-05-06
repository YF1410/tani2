#pragma once
#include "Sprite.h"
#include "SceneManager.h"
#include <wrl.h>
class SceneChange
{
public:
	//フェードタイプ
	enum TYPE {
		FADE_OUT = -1,
		FADE_IN = 1,
	};


	SceneChange();
	void Update();
	void Draw();

	bool SceneChangeStart(const std::string &sceneName, int parameter = 0);

	bool startFlag;
	bool endFlag;

	struct BLACK_BOX {
		std::unique_ptr<Sprite> sprite;
		void spin();

	}blackBox[32][18];
private:
	TYPE type;
};
