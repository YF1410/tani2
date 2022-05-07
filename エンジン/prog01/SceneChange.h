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
		NOT =0,
		FADE_IN = 1,
	};


	SceneChange();
	void Update();
	void Draw();

	bool SceneChangeStart(const std::string &sceneName, int parameter = 0);

	bool inEndFlag;
	bool outEndFlag;

	struct BLACK_BOX {
		std::unique_ptr<Sprite> sprite;
		void spin(TYPE type);
		int offsetTimer;
		bool inEnd;
		bool outEnd;

	}blackBox[32][18];
	TYPE type;
private:
	bool isChange;
	std::string sceneName;
	int parameter;
};
