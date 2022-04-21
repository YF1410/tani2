#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "Audio.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "LightGroup.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "FbxLoader.h"
#include "PlayerObject.h"
#include "Enemy.h"
#include "MapChip.h"

#include <vector>
#include <memory>

#include "BaseScene.h"


class CollisionManager;
class ContactableObject;

class GameScene : public BaseScene {
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private: // �ÓI�����o�ϐ�
	static const int debugTextTexNumber = 0;

public: // �����o�֐�
	//�R���X�g���N�^
	GameScene();
	// �f�X�g���N�^
	~GameScene();
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


	static int counter;
private:	//�����蔻��n�֐�
	//void AttackDebrisToEnemy();	//�j�ЂƓG
	//void PlayerToDebris();	//�v���C���[�Ɣj��

private: // �����o�ϐ�
	//�J����
	std::unique_ptr<Camera> camera;

	Vector3 eyeDistance = { 0,2700,-500 };
	const Vector3 eyeDistanceDef = { 0,2700,-1500 };
	Vector3 targetDistance = { 0,0,100 };
	const Vector3 targetDistanceDef = { 0,0,100 };
	//�X�v���C�g
	std::vector<std::unique_ptr<Sprite>> weveSprite;
	std::unique_ptr<Sprite> sprite;
	//�p�[�e�B�N��
	std::unique_ptr<ParticleManager> particleMan;
	//�I�u�W�F�N�g
	std::unique_ptr<Model> slimeModel;
	std::unique_ptr<Model> modelSphere;

	std::unique_ptr<PlayerObject> playerObject;

	std::unique_ptr<Model> modelPlane;
	std::vector<std::unique_ptr<Object3d>> objects;

	int weveStartTimer = 0;
	float upTimer = 0.0f;
	bool changeFlag = false;
	bool endFlag = false;
	bool showingFlag = false;
	bool flag = false;

	//�X�e�[�W
	//std::unique_ptr<FbxObject3d> testStage;
	//�X�e�[�W�̃|���S���f�[�^
	//std::vector<Triangle> stagePolygon;

	//���C�g
	std::unique_ptr<LightGroup> light;
	//�������������l
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	float searchPlayerLen = 500.0f;

	Line line;
	AABB aabb;

	//�����蔻��
	CollisionManager* collisionManager = nullptr;


	bool checkPoint;
};