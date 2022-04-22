#include "ParticleEmitter.h"
#include "DirectXCommon.h"


ParticleEmitter* ParticleEmitter::Create(Camera* camera, std::wstring fName)
{
	ParticleEmitter* particleEmitter = new ParticleEmitter();
	if (particleEmitter == nullptr)
	{
		return nullptr;
	}

	particleEmitter->particleMan = particleEmitter->particleMan->Create(DirectXCommon::GetInstance()->GetDevice(), camera);

	particleEmitter->particleMan->LoadTexture(fName);

	return particleEmitter;
}

void ParticleEmitter::Add(int count, int life, XMFLOAT3 position)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		this->position.x = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.x;
		this->position.y = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.y;
		this->position.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		velocity.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		velocity.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		accel.y = -(float)rand() / RAND_MAX * md_acc;

		//追加
		particleMan->Add(life, this->position, velocity, accel, s_scale, e_scale, s_color, e_color);
	}
}

void ParticleEmitter::Update()
{
	particleMan->Update();
}

void ParticleEmitter::Draw(ID3D12GraphicsCommandList* cmdList)
{
	particleMan->Draw(cmdList);
}