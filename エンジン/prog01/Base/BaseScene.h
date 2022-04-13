#pragma once

class SceneManager;

class BaseScene
{
public: // �����o�֐�
	virtual ~BaseScene() = default;

	// ������
	virtual void Initialize() = 0;
	//�I��
	virtual void Finalize() = 0;
	// ���t���[������
	virtual void Update() = 0;
	//�Œ�A�b�v�f�[�g
	virtual void FixedUpdate() {};
	//�ŏI�X�V
	virtual void LastUpdate() {};
	// �`��
	virtual void Draw() = 0;
};