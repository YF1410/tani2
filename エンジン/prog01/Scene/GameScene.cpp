#include "GameScene.h"
#include "Collision.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "CollisionManager.h"
#include "ContactableObject.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Debris.h"

using namespace DirectX;

GameScene::GameScene() {
	//���f���}�l�[�W���[�ɑS���f�����[�h
	ModelManager::GetIns()->Initialize();
}

GameScene::~GameScene() {
	Finalize();
}

void GameScene::Initialize() {
	collisionManager = CollisionManager::GetInstance();

	// �J��������
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera.get());
	// FBX�I�u�W�F�N�g�ɃJ�������Z�b�g
	FbxObject3d::SetCamera(camera.get());

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
	}
	// �f�o�b�O�e�L�X�g������
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png")) {
		assert(0);
	}

	// �w�i�X�v���C�g����
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetSize({ 100.0f,100.0f });
	sprite->SetPosition({ 100.0f,100.0f });

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());

	//���C�g����
	light = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, true);
	light->SetDirLightActive(2, true);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, true);


	// 3D�I�u�W�F�N�g����
	playerObject = std::make_unique<PlayerObject>();
	Enemy::enemys.push_back(new Enemy({ 2000,0,-2000 }));
	Enemy::enemys.push_back(new Enemy({ 2000,0,-1600 }));
	Enemy::enemys.push_back(new Enemy({ 1600,0,-2000 }));
	Enemy::enemys.push_back(new Enemy({ 1600,0,-1600 }));
	Enemy::enemys.push_back(new Enemy({ 1800,0,-1800 }));

	testStage = FbxObject3d::Create(ModelManager::GetIns()->GetModel(TESTS_TAGE));

	//�T�E���h�Đ�
	Audio::GetInstance()->LoadWave(0, "Resources/Alarm01.wav");

	// �J���������_���Z�b�g
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,1600,-500 });
	camera->SetUp({ 0,1,0 });

	//Debris::StaticInit();
	playerObject->Init();

	//�X�e�[�W�̒��_�f�[�^�ۑ�
	testStage.get()->Update();
	stagePolygon.clear();
	auto vertices = *testStage->GetModel()->GetVertices();
	auto indices = *testStage->GetModel()->GetIndces();
	XMMATRIX matWorld = testStage->GetModel()->GetModelTransform() * testStage->GetMatWorld();
	for (int Num = 0; Num < indices.size() - 3; Num += 3) {
		Triangle polygon;
		//���_����polygon�ɕϊ�
		int a = indices[Num];
		polygon = {
			XMVector4Transform(XMLoadFloat3(&vertices[indices[Num]].pos),matWorld),
			XMVector4Transform(XMLoadFloat3(&vertices[indices[Num + 1]].pos),matWorld),
			XMVector4Transform(XMLoadFloat3(&vertices[indices[Num + 2]].pos),matWorld),
		};

		//�@���v�Z
		Vector3 v1 = polygon.p1 - polygon.p0;
		Vector3 v2 = polygon.p2 - polygon.p0;
		polygon.normal = v1.VCross(v2);
		polygon.normal = Vector3(polygon.normal).Normalize();

		stagePolygon.push_back(polygon);
	}
}

void GameScene::Finalize() {
}

void GameScene::Update() {
	//�J�����X�V
	camera->SetEye(Vector3(playerObject.get()->GetPos() + Vector3(0, 1, -1) * 1000));
	camera->SetTarget(playerObject.get()->GetPos());
	camera->Update();

	//���͍X�V
	Input* input = Input::GetInstance();

	//���C�g�X�V
	light->Update();
	//�X�e�[�W�X�V
	testStage->Update();

	//�f�o�b�N�e�L�X�g
	DebugText::GetInstance()->VariablePrint(0, 0, "playerSize", playerObject.get()->size, 3);
	DebugText::GetInstance()->VariablePrint(0, 40, "DebrisCount", Debris::debris.size(), 3);

	particleMan->Update();

	for (auto& object : objects) {
		object->Update();
	}

	//�v���C���[�X�V
	playerObject->Update();
	//�j�ЍX�V
	Debris::StaticUpdate();
	//�G�l�~�[�X�V
	Enemy::StaticUpdate();


	//�j�ЂƃX�e�[�W�̏Փ�
	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
		//��~��Ԃ̕��Ƃ͔�����Ƃ�Ȃ�
		/*if (Debris::debris[debrisNum]->isStop) {
			continue;
		}*/
		XMVECTOR hitPos;
		Triangle hitPolygon;
		if (CheckSphere2Mesh(Debris::debris[debrisNum]->collider.realSphere, stagePolygon, &hitPos, &hitPolygon)) {
			Debris::debris[debrisNum]->Bounse(hitPos, hitPolygon.normal);
		}
	}


	float playerEnemyLen;	//�v���C���[�ƃG�l�~�[�̋���
	float debrisEnemyLen;	//�j�ЂƃG�l�~�[�̋���
	float lenTmp;			//�j�ЂƃG�l�~�[�̋����ŏ��l�ۑ��p
	Vector3 posTmp;			//lenTmp�ł̍ŏ������̔j�Ђ̈ʒu�ۑ��p
	//playerEnemyLen = Vector3(playerObject->GetPos() - enemyObject->GetPos()).Length();

	//�j�ЂƃG�l�~�[�̓����蔻��
	AttackDebrisToEnemy();
	//�v���C���[�Ɣj��
	PlayerToDebris();

	//fbxObject3d->Update();
	// �S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();
	//�S�Ă̈ړ��ŏI�K������
	playerObject.get()->Adaptation();
	Debris::StaticReflection();
	Enemy::StaticAdaptation();
}

void GameScene::Draw() {
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	//sprite->Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion �w�i�X�v���C�g�`��

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�N�W�F�N�g�̕`��
	Object3d::PreDraw(cmdList);
	Object3d::PostDraw();
#pragma endregion 3D�I�u�W�F�N�g�`��


#pragma region 3D�I�u�W�F�N�g(FBX)�`��
	testStage->Draw(DirectXCommon::GetInstance()->GetCommandList());
	playerObject->Draw();
	Debris::StaticDraw();
	Enemy::StaticDraw();
#pragma endregion 3D�I�u�W�F�N�g(FBX)�`��


#pragma region �p�[�e�B�N��
	// �p�[�e�B�N���̕`��
	particleMan->Draw(cmdList);
#pragma endregion �p�[�e�B�N��


#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��

	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_C)) {
		SceneManager::GetInstance()->ChangeScene("EnemyTestScene");
	}
	else if (input->TriggerKey(DIK_B)) {
		SceneManager::GetInstance()->ChangeScene("PlayerTestScene");
	}
}

void GameScene::AttackDebrisToEnemy() {
	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
		/*float minLength = Enemy::enemys[enemysNum]->sarchLength;
		bool isHoming = false;*/

		for (int enemysNum = 0; enemysNum < Enemy::enemys.size(); enemysNum++) {
			//�ǂ�����U����ԂłȂ���΃G�l�~�[�������o��
			if (!Debris::debris[debrisNum]->isAttack &&				//�j�Ђ��U����Ԃł͂Ȃ�
				Enemy::enemys[enemysNum]->state != Enemy::ATTACK &&	//�G�l�~�[���U����Ԃł͂Ȃ�

				Collision::CheckSphere2Sphere(
					Debris::debris[debrisNum]->collider.realSphere,	//�j�Ђ̍U���͈�
					Enemy::enemys[enemysNum]->collider.realSphere		//�G�l�~�[�̃q�b�g�X�t�B�A	
					//&hitPos,
					//&hitNormal
				)) {
				//Enemy::enemys[enemysNum]->SetPos(hitPos + -hitNormal * Enemy::enemys[enemysNum]->collider.realSphere.radius);
			}

			//�j�Ђ���G�l�~�[�ւ̍U��
			XMVECTOR hitPos;
			if (!Enemy::enemys[enemysNum]->isInvincible &&			//�G�l�~�[�����G�ł͂Ȃ��Ƃ�	
				Collision::CheckSphere2Sphere(
					Debris::debris[debrisNum]->collider.attackSphere,	//�j�Ђ̍U���͈�
					Enemy::enemys[enemysNum]->collider.hitSphere,		//�G�l�~�[�̃q�b�g�X�t�B�A	
					&hitPos
				)) {
				Vector3 hitNormal = hitPos - Enemy::enemys[enemysNum]->collider.hitSphere.center;
				Debris::debris[debrisNum]->Bounse(hitPos, Vector3(hitNormal).Normalize());
				if (Debris::debris[debrisNum]->isAttack) {
					Debris::debris[debrisNum]->isAttackFlame = true;
					Enemy::enemys[enemysNum]->Damage(1/*test�_���[�W*/);

				}
			}
			//�G�l�~�[����j�Ђւ̍U��
			//if (!Debris::debris[debrisNum]->isAttack &&				//�j�Ђ��U����Ԃł͂Ȃ�
			//	Collision::CheckSphere2Sphere(
			//		Enemy::enemys[enemysNum]->collider.attackSphere,		//�G�l�~�[�̃q�b�g�X�t�B�A
			//		Debris::debris[debrisNum]->collider.hitSphere	//�j�Ђ̍U���͈�
			//	))
			//{

			//	Debris::debris[debrisNum]->Damage(0.1f);
			//}

			//�G�l�~�[����j�Ђ̒T��
			//if (!Debris::debris[debrisNum]->isAttack &&			//�U�����ȊO�̔j�Ђ����O
			//	Collision::CheckSphere2Sphere(
			//	Debris::debris[debrisNum]->collider.realSphere,
			//	Enemy::enemys[enemysNum]->collider.searchArea)) {
			//	//���������݂̍ŏ��l�ȉ��Ȃ�Ώۂ��X�V
			//	float length = Vector3(Debris::debris[debrisNum]->GetPos() - Enemy::enemys[enemysNum]->GetPos()).Length();

			//	if(length < minLength){
			//		isHoming = true;
			//		Enemy::enemys[enemysNum]->HomingObjectCheck(Debris::debris[debrisNum]->GetPos());
			//	}
			//}
		}
	}
}

void GameScene::AttackEnemyToDebris() {
	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
		/*float minLength = Enemy::enemys[enemysNum]->sarchLength;
		bool isHoming = false;*/

		for (int enemysNum = 0; enemysNum < Enemy::enemys.size(); enemysNum++) {

			//�G�l�~�[����j�Ђւ̍U��
			if (!Debris::debris[debrisNum]->isAttack &&				//�j�Ђ��U����Ԃł͂Ȃ�
				Collision::CheckSphere2Sphere(
					Enemy::enemys[enemysNum]->collider.attackSphere,		//�G�l�~�[�̃q�b�g�X�t�B�A
					Debris::debris[debrisNum]->collider.hitSphere		//�j�Ђ̍U���͈�
				)) 			{
				Debris::debris[debrisNum]->Damage(0.1f);
			}

			//�G�l�~�[����j�Ђ̒T��
			if (!Debris::debris[debrisNum]->isAttack &&			//�U�����ȊO�̔j�Ђ����O
				Collision::CheckSphere2Sphere(
					Debris::debris[debrisNum]->collider.realSphere,
					Enemy::enemys[enemysNum]->collider.searchArea)) {
				Ray ray;
				Sphere s;
				ray.start = Enemy::enemys[enemysNum]->GetPos();
				ray.dir = Debris::debris[debrisNum]->pos - Enemy::enemys[enemysNum]->GetPos();
				//���������݂̍ŏ��l�ȉ��Ȃ�Ώۂ��X�V
				if (Collision::CheckRay2Sphere(ray,)) {
					float length = Vector3(Debris::debris[debrisNum]->GetPos() - Enemy::enemys[enemysNum]->GetPos()).Length();
				}

				/*if(length < minLength){
					isHoming = true;
					Enemy::enemys[enemysNum]->HomingObjectCheck(Debris::debris[debrisNum]->GetPos());
				}*/
			}
		}
	}
}

void GameScene::AttackEnemyToPlayer() {
}

void GameScene::PlayerToDebris() {
	//�j�Ђƃv���C���[�̏Փ�
	for (int i = 0; i < Debris::debris.size(); i++) {
		//�U�����̕��Ƃ͔�����Ƃ�Ȃ�
		if (Debris::debris[i]->isAttack) continue;
		//�z���񂹔���
		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.suctionSphere, Debris::debris[i]->collider.hitSphere)) {
			Debris::debris[i]->SuckedPlayer(playerObject->GetPos(), playerObject->GetSuction());
		}
		//�z������
		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.absorbSphere, Debris::debris[i]->collider.hitSphere)) {
			playerObject.get()->Absorb(Debris::debris[i]->AbsorbedToPlayer());
		}
	}
}


bool GameScene::CheckSphere2Mesh(
	Sphere& sphere,				//��
	std::vector<Triangle> meshDate,					//���b�V���f�[�^
	XMVECTOR* HitPos,			//�Փˈʒu
	Triangle* hitTriangle
) {
	for (int polygonNum = 0; polygonNum < meshDate.size(); polygonNum++) {
		if (Collision::CheckSphere2Triangle(sphere, meshDate[polygonNum], HitPos)) {
			if (hitTriangle != nullptr) {
				*hitTriangle = meshDate[polygonNum];
			}
			return true;
		}
	}
	return false;
}