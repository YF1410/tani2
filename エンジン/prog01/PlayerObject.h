#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include "Collision.h"

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
	PlayerObject(FbxModel *coreModel = nullptr);
	~PlayerObject();
	//������
	void Init();
	// ���t���[������
	void Update();
	// �`��
	void Draw();

	//�c�[���z��������
	void Absorb(float size);
	
	//�����蔻��
	struct COLLIDER {
		Sphere absorbSphere;
	}collider;

	float size;	//����


private: // �����o�ϐ�
	std::unique_ptr<FbxObject3d> slime;
	//�L�[�{�[�h�ړ��p
	float moveSpead = 30.0f;
	
	//�v���C���[�̊���W
	Vector3 pos;
	//���ړ���
	Vector3 moveVec;

	//�T�C�Y
	float scale;//�傫��

	//�����^�C�v
	DESTRUCT_TYPE destructType;
	//������
	DESTRUCT_POW destructPow;
	//�����t���O
	bool destructFlag = false;


};

