#include "ParticleEmitter.h"
#include "DirectXCommon.h"

Camera* ParticleEmitter::camera = nullptr;

ParticleEmitter* ParticleEmitter::Create(std::wstring fName)

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

void ParticleEmitter::RandAdd(int count, int life, XMFLOAT3 position)
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

void ParticleEmitter::Add(int count, int life, XMFLOAT3 position)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		this->position.x = position.x;
		this->position.y = position.y;
		this->position.z = position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity.x = md_vel;
		velocity.y = md_vel;
		velocity.z = md_vel;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		accel.y = md_acc;

		//追加
		particleMan->Add(life, this->position, velocity, accel, s_scale, e_scale, s_color, e_color);
	}
}

void ParticleEmitter::AddHeal(int count, int life, XMFLOAT3 position, Vector3 velocity)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		this->position.x = ((float)rand() / RAND_MAX * (md_pos / 2) - (md_pos / 2) / 2.0f) + position.x;
		this->position.y = position.y + 200.0f;
		this->position.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		//this->velocity.x = velocity.x;
		//velocity.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//this->velocity.z = velocity.z + ((float)rand() / RAND_MAX * md_vel / 2.0f);
		this->velocity.z = (float)rand() / RAND_MAX * md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		accel.z = (float)rand() / RAND_MAX * 0.5;

		//追加
		particleMan->Add(life, this->position, this->velocity, accel, s_scale, e_scale, s_color, e_color);
	}
}

void ParticleEmitter::AddBoom(int count, int life, XMFLOAT3 position)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		this->position.x = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.x;
		this->position.y = position.y + 200.0f;
		this->position.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//velocity.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		velocity.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		accel.x = (float)rand() / RAND_MAX * 0.5f - 0.5f / 2.0f;
		accel.z = -(float)rand() / RAND_MAX * 0.5;

		//追加
		particleMan->Add(life, this->position, velocity, accel, s_scale, e_scale, s_color, e_color);
	}
}

void ParticleEmitter::AddAttack(int count, int life, XMFLOAT3 position, Vector3 velocity,float rotation)
{
	for (int i = 0; i < count; i++)
	{

		Vector3 startVec;		//速度*向きベクトル
		float shotRad;			//角度決定

		//発射スピード
		float shotSpeed = (rand() % 20)*3;

		//-15~15度で計算
		shotRad = XMConvertToRadians(rand() % 90 - 45);

		startVec = velocity.Normal() * shotSpeed;

		startVec.AddRotationY(shotRad);

		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		this->position.x = position.x;
		//this->position.y = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.y;
		this->position.z = position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		this->velocity.x = -startVec.x;
		//velocity.y = startVec.y;
		this->velocity.z = -startVec.z;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		/*accel.x = (float)rand() / RAND_MAX * 0.5f - 0.5f / 2.0f;
		accel.z = -(float)rand() / RAND_MAX * 0.5;*/

		//追加
		particleMan->Add(life, this->position, this->velocity, accel, s_scale, e_scale, s_color, e_color,rotation);

	}
}

void ParticleEmitter::AddRef(int count, int life, XMFLOAT3 position, Vector3 velocity) {
	for (int i = 0; i < count; i++) 	{

		Vector3 startVec;		//速度*向きベクトル
		float shotRad;			//角度決定

		//発射スピード
		float shotSpeed = (rand() % 20) * 3;

		//-15~15度で計算
		shotRad = XMConvertToRadians(rand() % 90 - 45);

		startVec = velocity.Normal() * shotSpeed;

		startVec.AddRotationY(shotRad);

		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		this->position.x = position.x;
		//this->position.y = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.y;
		this->position.z = position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		this->velocity.x = startVec.x;
		//velocity.y = startVec.y;
		this->velocity.z = startVec.z;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		/*accel.x = (float)rand() / RAND_MAX * 0.5f - 0.5f / 2.0f;
		accel.z = -(float)rand() / RAND_MAX * 0.5;*/

		//追加
		particleMan->Add(life, this->position, this->velocity, accel, s_scale, e_scale, s_color, e_color);
	}
	camera->SetShakeFlag(true, 401);
}

void ParticleEmitter::AddDefeat(int count, int life, XMFLOAT3 position,DEFEAT_TYPE defeatType) {
	switch (defeatType) 	{
	case SHOCKWAVE:
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		this->position.x = position.x;
		this->position.y = position.y + 300.0f;
		this->position.z = position.z;
		//追加
		particleMan->Add(life, this->position, velocity, accel, s_scale, e_scale, s_color, e_color);
		break;
	case STAR:
		for (int i = 0; i < count; i++) {
			//X,Y,Z全て[-5.0,+5.0]でランダムに分布
			this->position.x = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.x;
			this->position.y = position.y + 300.0f;
			this->position.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.z;
			//X,Y,Z全て[-0.05,+0.05]でランダムに分布
			velocity.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			//velocity.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			velocity.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			//追加
			particleMan->Add(life, this->position, velocity, accel, s_scale, e_scale, s_color, e_color);
		}
	default:
		break;
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