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
#include "MapChip.h"
#include "Easing.h"

using namespace DirectX;

GameScene::GameScene() {
	MapChip::GetInstance()->Initialize();
	//�}�b�v����
	MapChip::GetInstance()->SetMapName(MapChip::TEST_MAP);
	MapChip::GetInstance()->CreateStage();


	//�v���C���[����
	playerObject = std::make_unique<PlayerObject>(MapChip::GetInstance()->GetStartPos());
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


	Enemy::enemys.push_back(new Enemy({ 2000,0,-5000 }));
	Enemy::enemys.push_back(new Enemy({ 2000,0,-5000 }));
	Enemy::enemys.push_back(new Enemy({ 1600,0,-5000 }));
	Enemy::enemys.push_back(new Enemy({ 1600,0,-5000 }));
	Enemy::enemys.push_back(new Enemy({ 1800,0,-5800 }));


	//�T�E���h�Đ�
	Audio::GetInstance()->LoadWave(0, "Resources/Alarm01.wav");

	// �J���������_���Z�b�g
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,1600,-500 });
	camera->SetUp({ 0,1,0 });
	
	//�v���C���[�̏�����
	playerObject->Initialize();

	checkPoint = false;
}

void GameScene::Finalize() {
}

void GameScene::Update() {
	//�J�����X�V
	//�v���C���[�̏�������œ_�ɂ���
	camera->SetEye(
		Ease(Out, Quad, 0.01f,
			camera.get()->GetEye(),
			Vector3(playerObject.get()->GetPos() + eyeDistance))
	);

	camera->SetTarget(Ease(Out, Quad, 0.01f,
		camera.get()->GetTarget(),
		Vector3(playerObject.get()->GetPos() + targetDistance))
	);
	camera->Update();


	//�}�b�v�`�b�v�X�V
	//MapChip::GetInstance()->Update(MapChip::TEST_MAP);


	//���͍X�V
	Input* input = Input::GetInstance();

	//���C�g�X�V
	light->Update();
	//�X�e�[�W�X�V
	//testStage->Update();
	
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


	////�}�b�v�`�b�v�Ƃ̓����蔻��
	EdgeType contact_edge = EdgeType::EdgeTypeNon;
	float contact_pos = 0.0f;
	////X��
	//if (MapChip::GetInstance()->CollisionRectAndMapchipEdgeVersion(
	//	playerObject.get()->GetBox(),
	//	Vector3(playerObject.get()->velocity.x,0,0),
	//	contact_edge,
	//	contact_pos,
	//	MapChip::TEST_MAP
	//)) {
	//	Vector3 hitPos = {
	//		contact_pos,
	//		0,
	//		playerObject.get()->pos.z,
	//	};
	//	Vector3 normal;
	//	if (contact_edge == EdgeType::EdgeTypeLeft) {
	//		DebugText::GetInstance()->Print("hitLeftMap", 0, 90, 3);

	//		normal = { -1,0,0 };
	//	}
	//	else {
	//		DebugText::GetInstance()->Print("hitRigthMap", 0, 90, 3);
	//		normal = { 1,0,0 };
	//	}
	//	playerObject.get()->HitWall(hitPos, normal);
	//}
	////Y��
	//if (MapChip::GetInstance()->CollisionRectAndMapchipEdgeVersion(
	//	playerObject.get()->GetBox(),
	//	Vector3( 0, 0, playerObject.get()->velocity.z ),
	//	contact_edge,
	//	contact_pos,
	//	MapChip::TEST_MAP
	//)) {
	//	Vector3 hitPos = {
	//		playerObject.get()->pos.x,
	//		0,
	//		contact_pos
	//	};
	//	Vector3 normal;
	//	if (contact_edge == EdgeType::EdgeTypeBottom) {
	//		normal = { 0,0,-1 };
	//		DebugText::GetInstance()->Print("hitBottomMap", 0, 120, 3);

	//	}
	//	else {
	//		DebugText::GetInstance()->Print("hitTopMap", 0, 120, 3);
	//		normal = { 0,0,1 };
	//	}
	//	playerObject.get()->HitWall(hitPos, normal);

	//}



	//�c�[�ƃ}�b�v�`�b�v
	for (int i = 0; i < Debris::debris.size(); i++) {

		//X��
		if (MapChip::GetInstance()->CollisionRectAndMapchipEdgeVersion(
			Debris::debris[i]->GetBox(),
			Vector3(Debris::debris[i]->velocity.x, 0, 0),
			contact_edge,
			contact_pos,
			MapChip::TEST_MAP
		)) {
			Vector3 hitPos = {
				contact_pos,
				0,
				Debris::debris[i]->pos.z,
			};
			Vector3 normal;
			if (contact_edge == EdgeType::EdgeTypeLeft) {
				DebugText::GetInstance()->Print("hitLeftMap", 0, 90, 3);

				normal = { -1,0,0 };
			}
			else {
				DebugText::GetInstance()->Print("hitRigthMap", 0, 90, 3);
				normal = { 1,0,0 };
			}
			Debris::debris[i]->HitWall(hitPos, normal);
		}
		//Y��
		if (MapChip::GetInstance()->CollisionRectAndMapchipEdgeVersion(
			Debris::debris[i]->GetBox(),
			Vector3(0, 0, Debris::debris[i]->velocity.z),
			contact_edge,
			contact_pos,
			MapChip::TEST_MAP
		)) {
			Vector3 hitPos = {
				Debris::debris[i]->pos.x,
				0,
				contact_pos
			};
			Vector3 normal;
			if (contact_edge == EdgeType::EdgeTypeBottom) {
				normal = { 0,0,-1 };
				DebugText::GetInstance()->Print("hitBottomMap", 0, 120, 3);

			}
			else {
				DebugText::GetInstance()->Print("hitTopMap", 0, 120, 3);
				normal = { 0,0,1 };
			}
			Debris::debris[i]->HitWall(hitPos, normal);

		}

	}

	// �S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();

	line.start = Enemy::enemys[0]->GetPos();
	line.end = playerObject->GetPos();
	line.vec = line.end - line.start;

	Collision::CheckLine2Box(line, )

	bool isCollision = ColRayAABB(lineStart, &(*lineEnd - *lineStart), &aabb[i], &box_WorldMat[i], col_t);
	//�S�Ă̈ړ��ŏI�K������
	playerObject.get()->Adaptation();
	Debris::StaticAdaptation();
	Enemy::StaticAdaptation();
	MapChip::GetInstance()->Adaptation();
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
	//testStage->Draw(DirectXCommon::GetInstance()->GetCommandList());
	MapChip::GetInstance()->Draw();
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

//
//void GameScene::AttackDebrisToEnemy()
//{
//	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
//		/*float minLength = Enemy::enemys[enemysNum]->sarchLength;
//		bool isHoming = false;*/
//
//		for (int enemysNum = 0; enemysNum < Enemy::enemys.size(); enemysNum++) {
//			//�ǂ�����U����ԂłȂ���΃G�l�~�[�������o��
//			if (!Debris::debris[debrisNum]->isAttack &&				//�j�Ђ��U����Ԃł͂Ȃ�
//				Enemy::enemys[enemysNum]->state !=Enemy::ATTACK &&	//�G�l�~�[���U����Ԃł͂Ȃ�
//				
//				Collision::CheckSphere2Sphere(
//				Debris::debris[debrisNum]->collider.realSphere,	//�j�Ђ̍U���͈�
//				Enemy::enemys[enemysNum]->collider.realSphere		//�G�l�~�[�̃q�b�g�X�t�B�A	
//				//&hitPos,
//				//&hitNormal
//			)) {
//				//Enemy::enemys[enemysNum]->SetPos(hitPos + -hitNormal * Enemy::enemys[enemysNum]->collider.realSphere.radius);
//			}
//
//			//�j�Ђ���G�l�~�[�ւ̍U��
//			XMVECTOR hitPos;
//			if (!Enemy::enemys[enemysNum]->isInvincible &&			//�G�l�~�[�����G�ł͂Ȃ��Ƃ�	
//				Collision::CheckSphere2Sphere(
//					Debris::debris[debrisNum]->collider.attackSphere,	//�j�Ђ̍U���͈�
//					Enemy::enemys[enemysNum]->collider.hitSphere,		//�G�l�~�[�̃q�b�g�X�t�B�A	
//					&hitPos
//				))
//			{
//				Vector3 hitNormal = hitPos - Enemy::enemys[enemysNum]->collider.hitSphere.center;
//				Debris::debris[debrisNum]->Bounse(hitPos, Vector3(hitNormal).Normalize());
//				if(Debris::debris[debrisNum]->isAttack){
//					Debris::debris[debrisNum]->isAttackFlame = true;
//					Enemy::enemys[enemysNum]->Damage(1/*test�_���[�W*/);
//
//				}
//			}
//			//�G�l�~�[����j�Ђւ̍U��
//			//if (!Debris::debris[debrisNum]->isAttack &&				//�j�Ђ��U����Ԃł͂Ȃ�
//			//	Collision::CheckSphere2Sphere(
//			//		Enemy::enemys[enemysNum]->collider.attackSphere,		//�G�l�~�[�̃q�b�g�X�t�B�A
//			//		Debris::debris[debrisNum]->collider.hitSphere	//�j�Ђ̍U���͈�
//			//	))
//			//{
//
//			//	Debris::debris[debrisNum]->Damage(0.1f);
//			//}
//
//			//�G�l�~�[����j�Ђ̒T��
//			//if (!Debris::debris[debrisNum]->isAttack &&			//�U�����ȊO�̔j�Ђ����O
//			//	Collision::CheckSphere2Sphere(
//			//	Debris::debris[debrisNum]->collider.realSphere,
//			//	Enemy::enemys[enemysNum]->collider.searchArea)) {
//			//	//���������݂̍ŏ��l�ȉ��Ȃ�Ώۂ��X�V
//			//	float length = Vector3(Debris::debris[debrisNum]->GetPos() - Enemy::enemys[enemysNum]->GetPos()).Length();
//
//			//	if(length < minLength){
//			//		isHoming = true;
//			//		Enemy::enemys[enemysNum]->HomingObjectCheck(Debris::debris[debrisNum]->GetPos());
//			//	}
//			//}
//		}
//	}
//}

//void GameScene::PlayerToDebris()
//{
//	//�j�Ђƃv���C���[�̏Փ�
//	for (int i = 0; i < Debris::debris.size(); i++) {
//		//�U�����̕��Ƃ͔�����Ƃ�Ȃ�
//		if (Debris::debris[i]->isAttack) continue;
//		//�z���񂹔���
//		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.suctionSphere, Debris::debris[i]->collider.hitSphere)) {
//			Debris::debris[i]->SuckedPlayer(playerObject->GetPos(), playerObject->GetSuction());
//		}
//		//�z������
//		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.absorbSphere, Debris::debris[i]->collider.hitSphere)) {
//			playerObject.get()->Absorb(Debris::debris[i]->AbsorbedToPlayer());
//		}
//	}
//}
