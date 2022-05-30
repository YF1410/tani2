#include "PlayerAfterimage.h"
#include "DirectXCommon.h"

std::vector<PlayerAfterimage*> PlayerAfterimage::playerAfterimages;

PlayerAfterimage::PlayerAfterimage(FbxObject3d player){
	playerObj = std::make_unique <FbxObject3d>(player);
	playerObj->SetIsPlay(false);
	isAlive = true;
	alpha = 0.8f;
}

PlayerAfterimage::~PlayerAfterimage(){}

void PlayerAfterimage::Update() {
	alpha -= 0.05f;
	playerObj->SetAlpha(alpha);
	if (alpha <= 0.0f) {
		isAlive = false;
	}
}

void PlayerAfterimage::LustUpdate() {
	
}

void PlayerAfterimage::StaticUpdate() {
	//�폜
	for (int i = playerAfterimages.size() - 1; i >= 0; i--) {
		if (!playerAfterimages[i]->isAlive) {
			delete playerAfterimages[i];
			playerAfterimages.erase(playerAfterimages.begin() + i);
		}
	}
	//�X�V
	for (int i = 0; i < playerAfterimages.size(); i++) {
		playerAfterimages[i]->Update();
	}
}

void PlayerAfterimage::StaticLustUpdate() {
	//�X�V
	for (int i = 0; i < playerAfterimages.size(); i++) {
		playerAfterimages[i]->LustUpdate();
	}
}

void PlayerAfterimage::StaticDraw() {
	for (int i = 0; i < playerAfterimages.size(); i++) {
		playerAfterimages[i]->playerObj.get()->Draw(DirectXCommon::GetInstance()->GetCommandList());
	}
}

void PlayerAfterimage::Finalize() {
	//�폜
	for (auto& a : playerAfterimages) 	{
		delete a;
	}
	playerAfterimages.clear();
}