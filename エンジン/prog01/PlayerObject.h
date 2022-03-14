#pragma once
#include "Object3d.h"
#include "Slime.h"
#include "Vector3.h"

enum DESTRUCT_TYPE {
	CIRCLE,			//�S����
	DIRECTIVITY		//�w����
};

enum DESTRUCT_POW {
	WEAK	= 15,	//�ア
	STRONG	= 30	//����
};

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
	//������
	void Init();
	// ���t���[������
	void Update();
	// �`��
	void Draw();

private: // �����o�ϐ�
	Slime* slime = nullptr;

	std::unique_ptr<Object3d> sphereOBJ;
	std::unique_ptr<Model> sphereModel;

	float moveSpead = 4.0f;
	
	//�v���C���[�̊���W
	Vector3 pos;
	//���ړ���
	Vector3 moveVec;
	//�ړ���
	float speed = 1.0f;

	//�����^�C�v
	DESTRUCT_TYPE destructType;
	//������
	DESTRUCT_POW destructPow;
	//�����t���O
	bool destructFlag = false;


};

