#include "SceneChange.h"


SceneChange::SceneChange():
	startFlag(false),
	endFlag(false)
{
	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 18; y++) {
			blackBox[x][y].sprite = Sprite::Create(110, { (float)(20 + x*40),(float)(20 + y * 40 )}, { 1,1,1,1 }, { 0.5f,0.5f });
		}
	}
}


void SceneChange::Update()
{
	//if (startFlag) {
		for (int x = 0; x < 32; x++) {
			for (int y = 0; y < 18; y++) {
				blackBox[x][y].spin();
			}
		}
	//}
		if (blackBox[31][17].sprite.get()->GetScale() <= 0) {

		}
}

void SceneChange::Draw()
{
	//fadeが開始したら
	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 18; y++) {
			blackBox[x][y].sprite.get()->Draw();
		}
	}
}

bool SceneChange::SceneChangeStart(const std::string &sceneName, int parameter)
{
	type = FADE_OUT;
	//アニメーションが終わったら遷移
	if (endFlag) {
		SceneManager::GetInstance()->ChangeScene(sceneName, parameter);
	}
	return false;
}

void SceneChange::BLACK_BOX::spin()
{
	//回転
	float rotate = sprite.get()->GetRotation();
	rotate += 3;
	this->sprite.get()->SetRotation(rotate);

	//縮小
	float scale = sprite.get()->GetScale();
	scale -= 0.05f;
	//見えなくなったら終了
	if (scale <= 0.0f) {
		scale = 0.0f;
	}
	this->sprite.get()->SetScale(scale);
}
