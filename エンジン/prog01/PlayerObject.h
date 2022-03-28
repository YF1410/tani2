#pragma once
#include "FbxObject3d.h"
#include "Vector3.h"
#include "SphereCollider.h"
#include "GameObjCommon.h"

enum DESTRUCT_TYPE {
	DIRECTIVITY	=1,	//�w����
	CIRCLE		=2,	//�S����
};

class PlayerObject :
	public GameObjCommon
{
public:
	PlayerObject(XMFLOAT3 startPos);
	//������
	void Initialize() override;
	//���t���[������
	void Update() override;
	
	//�Փˎ��R�[���o�b�N
	void OnCollision(const CollisionInfo &info) override;
	
	
	float size;	//����

	//�Q�b�^�[
	Vector3 GetPos() { return pos; }
	Vector3 *GetPosPointer() { return &pos; }
	float GetSuction() { return suction; }
	float GetSpeed() { return velocity.Length(); }
	float GetScale() { return scalef; }
	BOX2D GetBox() { return rect2d; }

	//�ǂƂ̏Փ�
	void HitWall(
		const XMVECTOR &hitPos,		//�Փˈʒu
		const Vector3 &normal);
private:	//�Փˎ��̏����֐�

private: // �����o�ϐ�
	//�L�[�{�[�h�ړ��p
	float moveSpead;
	
	//�X�^�[�g�ʒu
	XMFLOAT3 startPos;
	//�X�P�[���ɑ΂���z���䗦
	const float suctionRatio = 300.0f;
	//�z���͈�
	float suction;
	//�T�C�Y
	float scalef;//�傫��



	//�����^�C�v
	DESTRUCT_TYPE destructType;
	//������
	const int  destructPow = 10;

	//�R���C�_�[
	SphereCollider *hitCollider;	//�Փ˔���p
	SphereCollider *absorptionCollider;	//�Փ˔���p

	BOX2D rect2d;					//�}�b�v�`�b�v�v�Z�p
	void AdjustToMapchipEdgePosition(EdgeType contact_edge, float contact_pos);

};

