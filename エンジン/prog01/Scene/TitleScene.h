#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
#include "Camera.h"

#include "Sprite.h"
#include "DebugText.h"
#include "Object3d.h"

#include <memory>

#include "SceneChange.h"
#include "Easing.h"

class TitleScene : public BaseScene
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // �����o�֐�
	// �f�X�g���N�^
	~TitleScene();
	// ������
	void Initialize() override;
	//�I��
	void Finalize() override;
	// ���t���[������
	void Update() override;
	// ���t���[�����Ƃ̍Ō�̏���
	void LastUpdate() override;
	// �`��
	void Draw() override;

	//�{�^���֌W
	void Select();
	void Shake();

	void SpecifiedMove();

private: // �����o�ϐ�
	//�X�v���C�g
	std::unique_ptr<Sprite> sprite;
	//�J����
	std::unique_ptr<Camera> camera;
	//���C�g
	std::unique_ptr<LightGroup> light;

	std::unique_ptr<Object3d> titleObject3d;
	std::unique_ptr<Object3d> startObject3d;
	std::unique_ptr<Object3d> endObject3d;

	bool flag = true;
	bool isShake = false;
	int shakeTimer = 0;
	int attenuation = 0;
	

	XMFLOAT3 savePos = {};
	bool sceneChangeFlag = false;
	SceneChange sceneChange;
	
	XMFLOAT3 titleObjectPosition = { 5,5,0 };
	XMFLOAT3 startObjectPosition = { 30,-7,0 };
	XMFLOAT3 endObjectPosition = { 30,-17,0 };

	bool isUp = false;
	int specifiedMoveTimer = 0;
	int maxSpecifiedMoveTimer = 20;
	float bounceAmount = 1.0f;
	float specifiedBouncePosUp = titleObjectPosition.y + bounceAmount;
	float specifiedBouncePosDown = titleObjectPosition.y - bounceAmount;
	float specifiedBounceStartPosUp = startObjectPosition.y + bounceAmount;
	float specifiedBounceStartPosDown = startObjectPosition.y - bounceAmount;
	float specifiedBounceEndPosUp = endObjectPosition.y + bounceAmount;
	float specifiedBounceEndPosDown = endObjectPosition.y - bounceAmount;
};