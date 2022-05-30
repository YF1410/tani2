#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Audio.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "LightGroup.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "FbxLoader.h"
#include "PlayerObject.h"
#include "MapChip.h"

#include <vector>
#include <memory>

#include "EnemyManager.h"
#include "BaseScene.h"
#include "UserInterface.h"

#include "SceneChange.h"


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

public: // �T�u�N���X
	struct ScreenData
	{
		std::unique_ptr<Sprite> stateSprite;
		std::unique_ptr<Sprite> selectSprite;
		std::unique_ptr<Sprite> endSprite;
	};

public: // �����o�֐�
	//�R���X�g���N�^
	GameScene(int parameter = 0);
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

	void Select();

	//�{�^���֌W
	void Shake(Input* input);

	void Clear();
	void OutBack();

	void Gameover();

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
	std::unique_ptr<Sprite> sprite;
	//�I�u�W�F�N�g
	std::unique_ptr<Model> slimeModel;
	std::unique_ptr<Model> modelSphere;

	std::unique_ptr<PlayerObject> playerObject;
	std::unique_ptr<PlayerObject> playerAfterimage1;
	std::unique_ptr<PlayerObject> playerAfterimage2;
	bool afterimageflag;
	int afterimageTimer;

	//�G�l�~�[�}�l�[�W���[
	std::unique_ptr<EnemyManager> enemyManager;
	//UI
	std::unique_ptr<UserInterface> ui;
	//�w�i
	std::unique_ptr<Sprite> bg;


	std::unique_ptr<Model> modelPlane;

	std::unique_ptr<Object3d> stageclearObject3d;
	std::unique_ptr<Object3d> nextStageObject3d;
	std::unique_ptr<Object3d> clearEscapeObject3d;

	std::unique_ptr<Object3d> gameoverObject3d;
	std::unique_ptr<Object3d> retryObject3d;
	std::unique_ptr<Object3d> gameoverEscapeObject3d;

	std::unique_ptr<Object3d> recoveryEffectObject3d;
	std::unique_ptr<Object3d> recoveryEffect2Object3d;

	std::unique_ptr<Object3d> stageBGObject3d;

	int frame = 0;
	bool frameF = false;
	int healFrame = 3;
	int boomFrame = 2;

	bool saveCount = 0;

	bool isShake = false;
	int shakeTimer = 0;
	int attenuation = 0;

	int clearTimer = 0;
	int maxClearTimer = 70;

	int gameoverTimer = 0;
	int maxGameoverTimer = 70;

	Vector3 sceneBouncePos = { 0,0,0 };
	Vector3 sceneBouncePosDown = { 0,0,0 };
	Vector3 sceneBouncePosUp = { 0,0,0 };

	Vector3 selectBouncePos = { 0,0,0 };
	Vector3 selectBouncePosDown = { 0,0,0 };
	Vector3 selectBouncePosUp = { 0,0,0 };

	Vector3 sceneRotate = { 0,0,0 };
	Vector3 sceneRotateRight = { 0,0,0 };
	Vector3 sceneRotateLeft = { 0,0,0 };
	int bounceTimer = 0;
	int maxBounceTimer = 20;
	int rotateTimer = 0;
	int maxRotateTimer = 70;
	bool isUp = true;
	bool isRight = true;
	Vector3 bounceAmount = { 0,0,100 };
	Vector3 rotateAmount = { 0,-20,10 };

	XMFLOAT3 maxNextStageScale = { 1800, 100, 700 };
	XMFLOAT3 maxClearEscapeScale = { 1800, 100, 700 };
	XMFLOAT3 maxRetryScale = { 1800, 100, 700 };
	XMFLOAT3 maxGameoverEscapeScale = { 1800, 100, 700 };
	XMFLOAT3 selectScale = { 2200,100,1000 };
	int nextStageScaleTimer = 0;
	int maxNextStageScaleTimer = 20;
	int clearEscapeTimer = 0;
	int maxClearEscapeTimer = 20;
	int retryScaleTimer = 0;
	int maxRetryScaleTimer = 20;
	int gameoverEscapeTimer = 0;
	int maxGameoverEscapeTimer = 20;
	Vector3 stageclearPos = { 0,0,0 };
	Vector3 gameoverPos = { 0,0,0 };
	Vector3 nextStagePos = { 0,0,0 };
	Vector3 clearEscapePos = { 0,0,0 };
	Vector3 retryPos = { 0,0,0 };
	Vector3 gameoverEscapePos = { 0,0,0 };

	XMFLOAT3 saveStageclearPos = { 0,0,0 };
	XMFLOAT3 saveGameoverRot = { 0,0,0 };
	XMFLOAT3 saveNextStagePos = { 0,0,0 };
	XMFLOAT3 saveClearEscapePos = { 0,0,0 };
	XMFLOAT3 saveRetryPos = { 0,0,0 };
	XMFLOAT3 saveGameoverEscapePos = { 0,0,0 };

	//���C�g
	std::unique_ptr<LightGroup> light;
	//�������������l
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	float searchPlayerLen = 500.0f;

	//�����蔻��
	CollisionManager* collisionManager = nullptr;

	SceneChange sceneChange;
	bool checkPoint;
	//��ԂȂ�
	//�X�e�[�W�ԍ�
	int nowStageNum;

	//�N���A�t���O
	bool clearFlag = false;
	//�Q�[���I�[�o�[�t���O
	bool gameOverFlag = false;

	bool isSceneChange = false;
	//
	bool selectFlag = true;
	ScreenData gameOverScreen;
	ScreenData clearScreen;
	std::unique_ptr<Sprite> backSprite;

	//BGM�̐؂�ւ�
	bool isChangeBGM = false;

	//�`���[�g���A���t���O
	bool tutorialFlag = false;

	int longPushTimer = 0;
};