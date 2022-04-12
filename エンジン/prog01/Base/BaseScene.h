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
	// ���t���[�����Ƃ̍Ō�̏���
	virtual void LastUpdate() = 0;
	// �`��
	virtual void Draw() = 0;
};