#include "PlayerObject.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Debris.h"
#include "SlimeMath.h"

using namespace DirectX;

PlayerObject::PlayerObject(FbxModel *coreModel)
{
	slime = FbxObject3d::Create(ModelManager::GetIns()->GetModel(SLIME));
	slime->SetScale(1.0f);
	


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
	//ポジション初期化
	pos = { 0,0,0 };
	//サイズ初期化
	size = 100.0f;
	scale = 1.0f;
	//当たり判定初期化
	collider.absorbSphere.radius = 100.0f;
}

void PlayerObject::Update()
{
	Input* input = Input::GetInstance();

	//移動量減衰処理
	moveVec = {0,0,0};

	//リセット
	if (input->PushKey(DIK_R)) {
		Init();
	}

	//キーボード移動
	if (input->PushKey(DIK_A))	//左
	{
		moveVec.x -= moveSpead;
	}
	else if (input->PushKey(DIK_D))	//右
	{
		moveVec.x += moveSpead;
	}
	if (input->PushKey(DIK_S))	//後ろ
	{
		moveVec.z -= moveSpead;
	}
	else if (input->PushKey(DIK_W))	//前
	{
		moveVec.z += moveSpead;

	}

	//爆破威力変更
	if (input->PushKey(DIK_1)) {
		destructPow = STRONG;
	}
	if (input->PushKey(DIK_2)) {
		destructPow = WEAK;
	}

	//デバッグ用サイズ変更
	if (input->PushKey(DIK_Q)) {
		size += 0.1f;
	}
	if (input->PushKey(DIK_E)) {
		size -= 0.1f;
	}


	//自爆
	if (input->TriggerKey(DIK_SPACE) && !destructFlag)
	{
		destructFlag = true;
	}
	
	

	//爆破処理
	if (destructFlag) {
		//ショットに使う総合サイズ
		float maxSize;
		if (destructPow == WEAK) {
			maxSize = size * 0.2f;
		}
		else if (destructPow == STRONG ){
			maxSize = size * 0.4f;
		}

		//爆発威力と爆発方向から破片の数を計算
		for (int i = 0; i < destructPow; i++) {
			Vector3 startVec;		//速度*向きベクトル
			float shotRad;			//角度決定用
			float shotSpeed ;		//発射速度
			float shotSize;		//残骸のサイズ

			if (destructPow == WEAK) {
				shotSpeed = rand() % 10 + size * 0.5f + 10;
			}
			else if (destructPow == STRONG) {
				shotSpeed = rand() % 10 + size + 40;
			}
			shotSize = maxSize / destructPow;

			switch (destructType)
			{
			case CIRCLE:		//円形爆発
				shotRad = XMConvertToRadians(rand() % 360);		//360度で計算
				startVec = {
					static_cast<float>(cos(shotRad)),
					0,
					static_cast<float>(sin(shotRad))
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
			Debris::debris.push_back(new Debris(pos, startVec.Normalize() * shotSpeed, shotSize));
		}
		//爆発終了
		size -= maxSize;

		destructFlag = false;
	}

	//サイズからスケールへ変換
	scale = SizeToScaleConvert(size);
	//コライダー更新
	collider.absorbSphere.center = pos;
	collider.absorbSphere.radius = scale *1.1f * 100.0f;

	//スライムの移動適応
	pos += moveVec;
	slime->SetPosition(pos);
	slime->SetScale(scale);

	slime->Update();
}

void PlayerObject::Draw()
{
	slime->Draw(DirectXCommon::GetInstance()->GetCommandList());
}

void PlayerObject::Absorb(float size)
{
	this->size += size;
}
