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

	// 初期化
	void SetParticleEmitter(ParticleEmitter* _particleEmitter);
	// 後始末
	void Finalize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

private:
	std::vector<ParticleEmitter*> particleEmitters;
};