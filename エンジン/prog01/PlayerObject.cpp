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
	slimeObj = FbxObject3d::Create(ModelManager::GetIns()->GetModel(SLIME));
	


	srand(time(NULL));
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::Init()
{
	//自爆フラグ
	destructFlag = false;
	destructType = CIRCLE;
	destructPow = WEAK;
	//吸引範囲
	suction = scale * suctionRatio;
	//ポジション初期化
	pos = { 0,100,0 };
	//サイズ初期化
	size = 500.0f;
	scale = ConvertSizeToScale(size);
	slimeObj->SetScale(scale);
	//当たり判定初期化
	collider.absorbSphere.radius = 100.0f;
}

void PlayerObject::Update()
{
	Input* input = Input::GetInstance();
	//スケールから移動量決定
	moveSpead = scale * 20;

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
	//debug用飛翔
	if (input->PushKey(DIK_Z))	//上
	{
		moveVec.y -= moveSpead;
	}
	else if (input->PushKey(DIK_X))	//下
	{
		moveVec.y+= moveSpead;

	}

	//爆破威力変更
	if (input->PushKey(DIK_1)) {
		destructPow = STRONG;
	}
	if (input->PushKey(DIK_2)) {
		destructPow = WEAK;
	}
	//爆破タイプ変更
	if (input->PushKey(DIK_3)) {
		destructType = CIRCLE;
	}
	if (input->PushKey(DIK_4)) {
		destructType = DIRECTIVITY;
	}



	//デバッグ用サイズ変更
	if (input->PushKey(DIK_Q)) {
		size += 10.0f;
	}
	if (input->PushKey(DIK_E)) {
		size -= 10.0f;
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
			maxSize = size * 0.5f;
		}

		//爆発威力と爆発方向から破片の数を計算
		for (int i = 0; i < destructPow * destructType; i++) {
			Vector3 startVec;		//速度*向きベクトル
			float shotRad;			//角度決定用
			float shotSpeed ;		//発射速度
			//残骸のサイズ
			float shotSize = maxSize / (destructPow * destructType);

			switch (destructType)
			{
			case CIRCLE:		//円形爆発
				//サイズとスピード
				if (destructPow == WEAK) {
					shotSpeed = rand() % 4 + scale * 10;
				}
				else if (destructPow == STRONG) {
					shotSpeed = rand() % 12 + scale * 100;
				}
				

				shotRad = XMConvertToRadians(rand() % 360);		//360度で計算
				startVec = {
					static_cast<float>(cos(shotRad)),
					0,
					static_cast<float>(sin(shotRad))
				};
				break;

			case DIRECTIVITY:	//指向性爆発
				//サイズとスピード
				if (destructPow == WEAK) {
					shotSpeed = rand() % 4 + scale * 10;
				}
				else if (destructPow == STRONG) {
					shotSpeed = rand() % 12 + scale * 100;
				}


				shotRad = XMConvertToRadians(rand() % 15 - 15);		//360度で計算
				startVec = {
					static_cast<float>(cos(shotRad)),
					0,
					static_cast<float>(sin(shotRad))
				};
				break;
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
	scale = ConvertSizeToScale(size);

	//コライダー更新
	UpdateCollider();
}

void PlayerObject::Adaptation()
{
	//描画位置決定
	pos = afterPos;

	slimeObj->SetPosition(pos);
	slimeObj->SetScale(scale);

	slimeObj->Update();
}

void PlayerObject::Draw()
{
	slimeObj->Draw(DirectXCommon::GetInstance()->GetCommandList());
}

void PlayerObject::Absorb(float size)
{
	this->size += size;
}

void PlayerObject::UpdateCollider()
{
	//移動後の位置予測
	afterPos = pos + moveVec;

	//移動後の吸収範囲
	suction = scale * suctionRatio;
	
	//見た目に近い判定
	collider.realSphere.center = afterPos;
	collider.realSphere.radius = scale * 150.0f;
	//吸い寄せ用
	collider.suctionSphere.center = afterPos;
	collider.suctionSphere.radius = suction;
	//吸収用
	collider.absorbSphere.center = afterPos;
	collider.absorbSphere.radius = scale * 100.0f;
}

void PlayerObject::HitWall(
	const XMVECTOR &hitPos,		//衝突位置
	const Vector3 &normal)
{
	pos = hitPos + normal * collider.realSphere.radius;
	moveVec = CalcWallScratchVector(moveVec, normal);
	//コライダー更新
	UpdateCollider();
}
