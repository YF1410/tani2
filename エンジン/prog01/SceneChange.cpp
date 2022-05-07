#include "SceneChange.h"
#include "Audio.h"


SceneChange::SceneChange():
	inEndFlag(false),
	outEndFlag(false)
{
	type = FADE_IN;
	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 18; y++) {
			blackBox[x][y].sprite = Sprite::Create(110, { (float)(20 + x*40),(float)(20 + y * 40 )}, { 1,1,1,1 }, { 0.5f,0.5f });
			blackBox[x][y].offsetTimer = (31 - x + y);
		}
	}
}


void SceneChange::Update()
{
	if (type == NOT) return;
	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 18; y++) {
			if (blackBox[x][y].offsetTimer <= 0) {
				blackBox[x][y].spin(type);
			}
			else {
				blackBox[x][y].offsetTimer--;
			}

		}
	}

	//末端がtrueならそれを全体に適応
	if (blackBox[0][17].inEnd) {
		inEndFlag = true;
	}
	if (blackBox[0][17].outEnd) {
		outEndFlag = true;
		Audio::GetInstance()->LoopStopWave();
		SceneManager::GetInstance()->ChangeScene(sceneName, parameter);
	}


}

void SceneChange::Draw()
{
	if (type == NOT) return;

	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 18; y++) {
			blackBox[x][y].sprite.get()->Draw();
		}
	}
}

bool SceneChange::SceneChangeStart(const std::string &sceneName, int parameter)
{
	if (type != FADE_OUT) {
		outEndFlag = false;

		for (int x = 0; x < 32; x++) {
			for (int y = 0; y < 18; y++) {
				blackBox[x][y].sprite.get()->SetRotation(0);
				blackBox[x][y].sprite.get()->SetScale(0);
				blackBox[x][y].offsetTimer = (31 - x + y);

			}
		}
	}
	type = FADE_OUT;

	this->sceneName = sceneName;
	this->parameter = parameter;
	isChange = true;
	
	return false;
}

void SceneChange::BLACK_BOX::spin(TYPE type)
{
	//終了していれば処理を省略
	if (type == FADE_IN && inEnd) return;
	if (type == FADE_OUT && outEnd) return;

	//回転
	float rotate = sprite.get()->GetRotation();
	rotate += 3;
	this->sprite.get()->SetRotation(rotate);

	//縮小
	float scale = sprite.get()->GetScale();
	scale -= 0.05f * type;
	//見えなくなったら終了
	if (type == FADE_IN && scale <= 0.0f) {
		scale = 0.0f;
		inEnd = true;

	}
	else if (type == FADE_OUT && scale >= 1.0f &&
		(int)rotate % 90 == 0) {
		scale = 1.0f;
		outEnd = true;

	}

	this->sprite.get()->SetScale(scale);
}
