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

class ClearScene : public BaseScene
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
	~ClearScene();
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

	void Bounce();
	void OutBack();

private: // �����o�ϐ�
	//�X�v���C�g
	std::unique_ptr<Sprite> sprite;
	//�J����
	std::unique_ptr<Camera> camera;
	//���C�g
	std::unique_ptr<LightGroup> light;

	std::unique_ptr<Object3d> stageclearObject3d;
	std::unique_ptr<Object3d> nextStageObject3d;
	std::unique_ptr<Object3d> clearEscapeObject3d;

	bool flag = true;
	bool shakeTimerFlag = false;
	int shakeTimer = 0;
	int attenuation = 0;

	bool bounceTimerFlag = false;
	int bounceTimer = 0;
	int maxBounceTimer = 70;

	XMFLOAT3 maxNextStageScale = { 30, 1, 10 };
	XMFLOAT3 maxClearEscapeScale = { 10, 1, 5 };
	int nextStageScaleTimer = 0;
	int maxNextStageScaleTimer = 20;
	int clearEscapeTimer = 0;
	int maxClearEscapeTimer = 20;

	XMFLOAT3 savePos = {};
};