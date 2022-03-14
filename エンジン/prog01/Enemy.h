#pragma once
#include "Object3d.h"

class Enemy {
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	Enemy();
	~Enemy();
	// ���t���[������
	void Update();
	// �`��
	void Draw();
	//�f�o�b�O�e�L�X�g�o�͂Ɏg�p
	float GetAngle() { return angle; }
	//�G�̓���
	void enemyMove();

private: // �����o�ϐ�
	std::unique_ptr<Object3d> coreObj;
	std::unique_ptr<Model> coreModel;

	std::unique_ptr<Object3d> sphereObj;
	std::unique_ptr<Model> sphereModel;

	float angle = 0.0f;
	float searchPlayerLen = 5.0f;
	bool playerContact = false;
	float speed = 0.03f;
	int moveTime = 0;
	int maxMoveTime = 180;
};