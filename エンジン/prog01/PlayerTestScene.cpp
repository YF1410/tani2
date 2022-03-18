#include "PlayerTestScene.h"
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

PlayerTestScene::PlayerTestScene() {
	//���f���}�l�[�W���[�ɑS���f�����[�h
	ModelManager::GetIns()->Initialize();
}

PlayerTestScene::~PlayerTestScene() {
	Finalize();
}

void PlayerTestScene::Initialize() {


	collisionManager = CollisionManager::GetInstance();

	// �J��������
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera.get());
	// FBX�I�u�W�F�N�g�ɃJ�������Z�b�g
	FbxObject3d::SetCamera(camera.get());

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) 	{
		assert(0);
	}
	// �f�o�b�O�e�L�X�g������
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png")) 	{
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
	playerObject->Init();


	testStage = FbxObject3d::Create(ModelManager::GetIns()->GetModel(TESTS_TAGE));

	// �J���������_���Z�b�g
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,1000, -1000 });
	camera->SetUp({ 0,1,0 });
	camera->Update();

	//Debris::StaticInit();


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

void PlayerTestScene::Finalize() {
}

void PlayerTestScene::Update() {
	camera->SetEye(Vector3(playerObject.get()->GetPos() + Vector3(0,1,-1) * 1000));
	camera->SetTarget(playerObject.get()->GetPos());
	camera->Update();
	Input* input = Input::GetInstance();
	light->Update();
	particleMan->Update();
	testStage->Update();

	//�J�������v���C���[�̈ʒu����v�Z

	DebugText::GetInstance()->VariablePrint(0, 0, "playerSize", playerObject.get()->size, 3);
	DebugText::GetInstance()->VariablePrint(0, 40, "DebrisCount", Debris::debris.size(), 3);


	if (input->TriggerKey(DIK_C)) 	{
		SceneManager::GetInstance()->ChangeScene("EnemyTestScene");
	}


	for (auto& object : objects) {
		object->Update();
	}
	//�v���C���[�X�V
	playerObject->Update();
	//�j�ЍX�V
	Debris::StaticUpdate();

	//�j�ЂƃX�e�[�W�̏Փ�
	for (int debrisNum = 0; debrisNum < Debris::debris.size(); debrisNum++) {
		//��~��Ԃ̕��Ƃ͔�����Ƃ�Ȃ�
		if (Debris::debris[debrisNum]->isStop) {
			continue;
		}
		XMVECTOR hitPos;
		Triangle hitPolygon;
		if (CheckSphere2Mesh(Debris::debris[debrisNum]->collider.realSphere, stagePolygon,&hitPos,&hitPolygon)) {
			Debris::debris[debrisNum]->Bounse(hitPos,hitPolygon.normal);
		}
	}

	//�v���C���[�ƕǂ̏Փ�
	XMVECTOR hitPos;
	Triangle hitPolygon;
	if (CheckSphere2Mesh(playerObject.get()->collider.realSphere, stagePolygon, &hitPos, &hitPolygon)) {
		DebugText::GetInstance()->VariablePrint(0, 80, "HitStage", 1, 3);
		playerObject.get()->HitWall(hitPos, hitPolygon.normal);
	}


	//�j�Ђƃv���C���[�̏Փ�
	for (int i = 0; i < Debris::debris.size(); i++) {
		//�U�����̕��Ƃ͔�����Ƃ�Ȃ�
		if (Debris::debris[i]->isAttack) continue;
		//�z���񂹔���
		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.suctionSphere, Debris::debris[i]->collider.absorbedSphere)) {
			Debris::debris[i]->SuckedPlayer(playerObject->GetPos(), playerObject->GetSuction());
		}
		//�z������
		if (Collision::CheckSphere2Sphere(playerObject.get()->collider.absorbSphere, Debris::debris[i]->collider.absorbedSphere)) {
			playerObject.get()->Absorb(Debris::debris[i]->AbsorbedToPlayer());
		}
	}

	//fbxObject3d->Update();
	// �S�Ă̏Փ˂��`�F�b�N
	//collisionManager->CheckAllCollisions();

	//�S�Ă̈ړ��ŏI�K������
	playerObject.get()->Adaptation();
	Debris::StaticReflection();
}

void PlayerTestScene::Draw() {
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
}

bool PlayerTestScene::CheckSphere2Mesh(
	Sphere &sphere,				//��
	std::vector<Triangle> meshDate,					//���b�V���f�[�^
	XMVECTOR *HitPos,			//�Փˈʒu
	Triangle *hitTriangle
)
{
	for (int polygonNum = 0; polygonNum < meshDate.size(); polygonNum++) {
		if (Collision::CheckSphere2Triangle(sphere, meshDate[polygonNum],HitPos)) {
			if (hitTriangle != nullptr) {
				*hitTriangle = meshDate[polygonNum];
			}
			return true;
		}
	}
	return false;
}
