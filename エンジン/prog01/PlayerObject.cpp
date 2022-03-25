#include "PlayerObject.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Debris.h"
#include "SlimeMath.h"
#include "DebugText.h"

using namespace DirectX;

PlayerObject::PlayerObject():
	GameObjCommon(
		ModelManager::SLIME,	//スライムのモデルをセット
		GameObjCommon::Player,	//プレイヤーとして扱う
		false					//重力の影響を受ける
	)
{
	srand(time(NULL));			//爆破用乱数のシード値をセット

	//サイズ初期化
	size = 500.0f;
	scale = ConvertSizeToScale(size);
	objectData->SetScale(scale);
	//当たり判定初期化
	hitSphere = new SphereCollider();
	hitSphere->SetRadius(/*scale * 180.0f*/1.0f);
	hitSphere->SetOffset({ 0,hitSphere->GetRadius(),0 });
	SetCollider(hitSphere);
}


void PlayerObject::Initialize()
{
	//自爆フラグ
	destructFlag = false;
	destructType = CIRCLE;
	destructPow = WEAK;
	//吸引範囲
	suction = scale * suctionRatio;
	//ポジション初期化
	pos = { 0,100,0 };

}

void PlayerObject::Update()
{
	Input* input = Input::GetInstance();
	//スケールから移動量決定
	moveSpead = scale * 5;

	//移動量減衰処理
	VelocityReset(false,0.5f);

	//リセット
	if (input->PushKey(DIK_R)) {
		Initialize();
	}

	//キーボード移動
	if (input->PushKey(DIK_A))	//左
	{
		velocity.x -= moveSpead;
	}
	else if (input->PushKey(DIK_D))	//右
	{
		velocity.x += moveSpead;
	}
	if (input->PushKey(DIK_S))	//下
	{
		velocity.y -= moveSpead;
	}
	else if (input->PushKey(DIK_W))	//上
	{
		velocity.y += moveSpead;

	}

	velocity.x += input->PadStickGradient().x * moveSpead;
	velocity.z += -input->PadStickGradient().y * moveSpead;


	//爆破威力変更
	if (input->TriggerKey(DIK_1) || input->TriggerPadButton(BUTTON_LEFT_SHOULDER)) {
		if (destructType != STRONG) {
			destructPow = STRONG;
		}
		else {
			destructPow = WEAK;
		}
	}
	//爆破タイプ切り替え
	if (input->TriggerKey(DIK_3)||input->TriggerPadButton(BUTTON_RIGHT_SHOULDER)) {
		if (destructType != CIRCLE) {
			destructType = CIRCLE;
		}
		else {
			destructType = DIRECTIVITY;
		}
	}



	//デバッグ用サイズ変更
	if (input->PushKey(DIK_Q)) {
		size += 10.0f;
	}
	if (input->PushKey(DIK_E)) {
		size -= 10.0f;
	}


	//自爆
	if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(BUTTON_A) && !destructFlag)
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
			Debris::debris.push_back(new Debris(pos, startVec.Normalize() *shotSpeed, shotSize));
		}
		//爆発終了
		size -= maxSize;

		destructFlag = false;
	}

	
	//サイズからスケールへ変換
	scale = ConvertSizeToScale(size);

	//移動量を適応
	Move();
	
	//コライダー更新
	//ColliderUpdate();
}


void PlayerObject::Absorb(float size)
{
	this->size += size;
}


void PlayerObject::OnCollision(const CollisionInfo &info)
{
	switch (info.object->Tag)
	{
	case ENEMY:
		DebugText::GetInstance()->Print("HitEnemy", 0, 60,3);
		break;

	case DEFAULT_BLOACK:
		DebugText::GetInstance()->Print("HitBox", 0, 60, 3);
		break;
		
	default:
		break;
	}
}

void PlayerObject::HitWall(
	const XMVECTOR &hitPos,		//衝突位置
	const Vector3 &normal)
{
	//velocity = CalcWallScratchVector(velocity, normal);
	////コライダー更新
	//ColliderUpdate();
}
