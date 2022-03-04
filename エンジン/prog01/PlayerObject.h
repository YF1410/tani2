#pragma once
#include "Object3d.h"
#include "Slime.h"
#include "Weapon.h"

class PlayerObject
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

public:
	PlayerObject(Model* model = nullptr, Model* model2 = nullptr);
	~PlayerObject();
	// ���t���[������
	void Update();
	// �`��
	void Draw();

private: // �����o�ϐ�
	Slime* slime = nullptr;
	Weapon* weapon = nullptr;

	std::unique_ptr<Object3d> sphereOBJ;
	std::unique_ptr<Model> sphereModel;

	float angle = 0.0f;
	float len = 1.5f;
	float speed = 4.0f;

	bool flag = false;
};

