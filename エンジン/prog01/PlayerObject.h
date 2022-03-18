#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include "Collision.h"

enum DESTRUCT_TYPE {
	DIRECTIVITY	=1,	//�w����
	CIRCLE		=2,	//�S����
};

enum DESTRUCT_POW {
	WEAK	= 8,	//�ア
	STRONG	= 16	//����
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
	//���t���[������
	void Update();
	//�ŏI�X�V
	void Adaptation();
	//�`��
	void Draw();

	

	float size;	//����

	//�Q�b�^�[
	Vector3 GetPos() { return pos; }
	float GetSuction() { return suction; }
	float GetSpeed() { return moveVec.Length(); }
	float GetScale() { return scale; }

public:		//�Փˎ��֌W

	//�����蔻��
	struct COLLIDER {
		Sphere realSphere;		//�����ڂ��厖�ȓ����蔻��
		Sphere suctionSphere;	//�j�Ђ̋z���񂹗p�����蔻��
		Sphere absorbSphere;	//�z���p
	}collider;
	//�����蔻��ꊇ�X�V�p
	void UpdateCollider();


	//�R�[���o�b�N
	//�ǂƂ̏Փ�
	void HitWall(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal);
	//�c�[���z��������
	void Absorb(float size);

private: // �����o�ϐ�
	//�X���C���I�u�W�F�N�g
	std::unique_ptr<FbxObject3d> slimeObj;
	//�L�[�{�[�h�ړ��p
	float moveSpead;
	
	//�v���C���[�̊���W
	Vector3 pos;
	//���ړ���
	Vector3 moveVec;
	//�ړ��\�z�ʒu
	Vector3 afterPos;
	
	//�X�P�[���ɑ΂���z���䗦
	const float suctionRatio = 300.0f;
	//�z���͈�
	float suction;
	//�T�C�Y
	float scale;//�傫��



	//�����^�C�v
	DESTRUCT_TYPE destructType;
	//������
	DESTRUCT_POW destructPow;
	//�����t���O
	bool destructFlag = false;


};

