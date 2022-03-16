#include "PlayerObject.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Debris.h"

using namespace DirectX;

PlayerObject::PlayerObject(FbxModel *coreModel)
{
	slime = FbxObject3d::Create(ModelManager::GetIns()->GetModel(SLIME));

	pos = { 0,0,0 };
	srand(time(NULL));
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::Init()
{
	destructFlag = false;
	destructType = CIRCLE;
	destructPow = WEAK;
}

void PlayerObject::Update()
{
	Input* input = Input::GetInstance();

	//移動量減衰処理
	moveVec = {0,0,0};

	//キーボード移動
	if (input->PushKey(DIK_A))	//左
	{
		moveVec.x -= speed;
	}
	else if (input->PushKey(DIK_D))	//右
	{
		moveVec.x += speed;
	}
	if (input->PushKey(DIK_S))	//後ろ
	{
		moveVec.z -= speed;
	}
	else if (input->PushKey(DIK_W))	//前
	{
		moveVec.z += speed;

	}

	//爆破威力変更
	if (input->PushKey(DIK_1)) {
		destructPow = STRONG;
	}
	if (input->PushKey(DIK_2)) {
		destructPow = WEAK;
	}

	//自爆
	if (input->TriggerKey(DIK_SPACE) && !destructFlag)
	{
		destructFlag = true;
	}
	
	
	//スライムの移動適応
	pos += moveVec;
	slime->SetPosition(pos);
	slime->Update();

	//爆破処理
	if (destructFlag) {


		//爆発威力と爆発方向から破片の数を計算
		for (int i = 0; i < destructPow; i++) {
			Vector3 startVec;		//速度*向きベクトル
			float shotRad;			//角度決定用
			float speed = rand() % 40 / 10.0f + destructPow / 2.0f;		//発射速度
			float size = 1.0f;		//残骸のサイズ
			switch (destructType)
			{
			case CIRCLE:		//円形爆発
				shotRad = XMConvertToRadians(rand() % 360);		//360度で計算
				startVec = {
					cos(shotRad),
					0,
					sin(shotRad)
				};
				break;
			case DIRECTIVITY:	//指向性爆発
				//shotRad = XMConvertToRadians(rand() % 60);		//360度で計算
				//startVec = {
				//	cos(shotRad),
				//	0,
				//	sin(shotRad)
				//};

				break;
			default:
				break;
			}
			//Debrisのコンテナに追加
			Debris::debris.push_back(new Debris(pos, startVec.Normalize() * speed, size));
		}
		//爆発終了
		destructFlag = false;
	}



}

void PlayerObject::Draw()
{
	slime->Draw(DirectXCommon::GetInstance()->GetCommandList());
}