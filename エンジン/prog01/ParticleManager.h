#pragma once
#include "ParticleEmitter.h"

#include <memory>
#include <vector>

class ParticleManager final
{
private:
	ParticleManager() = default;
	~ParticleManager() = default;

public:
	ParticleManager(const ParticleManager& r) = delete;
	ParticleManager& operator=(const ParticleManager& r) = delete;

	static ParticleManager* GetInstance();

	// ‰Šú‰»
	void SetParticleEmitter(ParticleEmitter* _particleEmitter);
	// Œãn––
	void Finalize();
	// –ˆƒtƒŒ[ƒ€ˆ—
	void Update();
	// •`‰æ
	void Draw(ID3D12GraphicsCommandList* cmdList);

private:
	std::vector<ParticleEmitter*> particleEmitters;
};