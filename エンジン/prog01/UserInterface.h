#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>

class UserInterface
{
private: //�V���O���g���p�^�[��
	UserInterface() = default;
	~UserInterface() = default;
	UserInterface(const UserInterface & r) = default;
	UserInterface &operator= (const UserInterface & r) = default;
public:
	//�C���X�^���X
	static UserInterface *GetIns();
	void Initialize(int *nowWave);
	void Update();
	void Draw() const;

private:
	//�E�F�[�u�ύX���o
	static std::unique_ptr<Sprite> wave[3];
	static std::unique_ptr<Sprite> moveWave[3];


	//���݂̃E�F�[�u��
	static int *nowWave;
	static int oldWave;
	static float moveWaveTimer;
	static float movePosX;
	static bool isChangeWave;
};

