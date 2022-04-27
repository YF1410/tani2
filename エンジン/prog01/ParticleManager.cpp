#include "ParticleManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::SetParticleEmitter(ParticleEmitter* _particleEmitter)
{
	particleEmitters.push_back(_particleEmitter);
}

void ParticleManager::Finalize()
{
	for (auto& a : particleEmitters)
	{
		delete a;
	}
	particleEmitters.clear();
}

void ParticleManager::Update()
{
	for (auto& a : particleEmitters)
	{
		a->Update();
	}
}

void ParticleManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (auto& a : particleEmitters)
	{
		a->Draw(cmdList);
	}
}