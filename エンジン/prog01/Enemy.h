#pragma once
#include "Object3d.h"
#include "Vector3.h"

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
	//�����蔻��Ɏg�p
	Vector3 GetPosition() { return enemyPos; }
	//�A���O���Z�b�^�[
	void SetAngle(float angle) { this->angle = angle; }
	//playerContact�Z�b�^�[
	void SetPlayerContact(bool isPlayerContact) { this->isPlayerContact = isPlayerContact; }
	//�G�̓���
	void enemyMove();

private: // �����o�ϐ�
	std::unique_ptr<Object3d> coreObj;
	std::unique_ptr<Model> coreModel;

	std::unique_ptr<Object3d> sphereObj;
	std::unique_ptr<Model> sphereModel;

	float angle = 0.0f;
	float searchPlayerLen = 5.0f;
	bool isPlayerContact = false;
	bool isWandering = false;
	int wanderingCount = 0;
	float speed = 3.0f;
	int moveTime = 180;
	int maxMoveTime = 180;
	Vector3 enemyPos;
};