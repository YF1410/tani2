#pragma once

#include "ParticleManager.h"
#include "Vector3.h"

#include <string>

#include <string>

class ParticleEmitter
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	ParticleEmitter* Create(std::wstring fName = L"effect1");

	static void SetCamera(Camera* _camera) { camera = _camera; };

private: //  �ÓI�����o�ϐ�
	static Camera* camera;

public: // �����o�֐�
	// �ǉ�
	void RandAdd(int count = 10, int life = 60, XMFLOAT3 position = { 0,0,0 });
	void Add(int count = 10, int life = 60, XMFLOAT3 position = { 0,0,0 });
	void AddHeal(int count = 10, int life = 60, XMFLOAT3 position = { 0,0,0 });
	void AddBoom(int count = 10, int life = 60, XMFLOAT3 position = { 0,0,0 });
	void AddRef(int count = 10, int life = 60, XMFLOAT3 position = { 0,0,0 },Vector3 velocity = {0,0,0});
	// ���t���[������
	void Update();
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	// ���W�̐ݒ�
	void SetCenter(const float md_pos) { this->md_pos = md_pos; }
	// ���x�̐ݒ�
	void SetVelocity(float md_vel) { this->md_vel = md_vel; }
	// �����x�̐ݒ�
	void SetAccel(float md_acc) { this->md_acc = md_acc; }
	//�F(RGBA)�����l�̐ݒ�
	void SetStartColor(XMFLOAT4 s_color) { this->s_color = s_color; }
	//�F(RGBA)�ŏI�l�̐ݒ�
	void SetEndColor(XMFLOAT4 e_color) { this->e_color = e_color; }
	// �X�P�[�������l�̐ݒ�
	void SetStartScale(const float s_scale) { this->s_scale = s_scale; }
	// �X�P�[���ŏI�l�̐ݒ�
	void SetEndScale(const float e_scale) { this->e_scale = e_scale; }

private: // �����o�ϐ�
	//���W
	XMFLOAT3 position = {};
	//���x
	XMFLOAT3 velocity = {};
	//�����x
	XMFLOAT3 accel = {};
	//���݃t���[��
	int frame = 0;
	//�I���t���[��
	int num_frame = 0;
	// �X�P�[�������l
	float s_scale = 1.0f;
	// �X�P�[���ŏI�l
	float e_scale = 0.0f;
	// �F(RGBA)�����l
	XMFLOAT4 s_color = { 1, 1, 1, 1 };
	// �F(RGBA)�ŏI�l
	XMFLOAT4 e_color = {};
	//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
	float md_pos = 10.0f;
	//X,Y,Z�S��[-0.05,+0.05]�Ń����_���ɕ��z
	float md_vel = 0.1f;
	//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
	float md_acc = 0.001f;

	std::unique_ptr<ParticleManager> particleMan;
};