#include "PlayerObject.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Debris.h"
#include "SlimeMath.h"
#include "DebugText.h"
#include "MapChip.h"


using namespace DirectX;

PlayerObject::PlayerObject(XMFLOAT3 startPos):
	GameObjCommon(
		ModelManager::SLIME,	//スライムのモデルをセット
		GameObjCommon::PLAYER,	//プレイヤーとして扱う
		false					//重力の影響を受ける
	)
{
	srand(time(NULL));			//爆破用乱数のシード値をセット
	//初期位置に設定
	this->startPos = startPos;
	pos = startPos;

	//サイズ初期化
	size = 500.0f;

	scalef = ConvertSizeToScale(size);
	objectData->SetScale(scalef);
	//当たり判定
	hitCollider = new SphereCollider("hitCollider", { 0,scalef * 180.0f,0 }, scalef * 180.0f);
	SetCollider(hitCollider);
	absorptionCollider = new SphereCollider("absorptionCollider", { 0,scalef * 180.0f,0 }, (scalef + 1) * 180.0f);
	SetCollider(absorptionCollider);
}


void PlayerObject::Initialize()
{
	//サイズ初期化
	size = 500.0f;
	//自爆フラグ
	destructType = CIRCLE;
	//吸引範囲
	suction = scalef * suctionRatio;
	//ポジション初期化
	pos = startPos;

}

void PlayerObject::Update()
{
	Input* input = Input::GetInstance();
	//スケールから移動量決定
	moveSpead = scalef * 5;

	//移動量減衰処理
	VelocityReset(false,0.8f);

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
	if (input->PushKey(DIK_S))	//後ろ
	{
		velocity.z -= moveSpead;
	}
	else if (input->PushKey(DIK_W))	//前
	{
		velocity.z += moveSpead;

	}

	velocity.x += input->PadStickGradient().x * moveSpead;
	velocity.z += -input->PadStickGradient().y * moveSpead;


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
	if (input->PushKey(DIK_UP)) {
		size += 10.0f;
	}
	if (input->PushKey(DIK_DOWN)) {
		size -= 10.0f;
	}

	//自爆
	if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(BUTTON_A))
	{
		//ショットに使う総合サイズ
		float maxSize = size * 0.5f;
		//爆発威力と爆発方向から破片の数を計算
		for (int i = 0; i < destructPow * destructType; i++) {
			Vector3 startVec;		//速度*向きベクトル
			float shotRad;			//角度決定用
			//発射スピード
			float shotSpeed = rand() % 12 + scalef * 20;
			//残骸のサイズ
			float shotSize = maxSize / (destructPow * destructType);

			switch (destructType)
			{
			case CIRCLE:		//円形爆発
				//放射角度を360度で計算
				shotRad = XMConvertToRadians(rand() % 360);
				startVec = {
					static_cast<float>(cos(shotRad)),
					0,
					static_cast<float>(sin(shotRad))
				};
				break;

			case DIRECTIVITY:	//指向性爆発
				//-15~15度で計算
				shotRad = XMConvertToRadians(rand() % 30 - 15);
			
				startVec = velocity.Normalize();

				startVec.AddRotationY(shotRad);
				//startVec = startVec + offset;
				break;
			default:
				break;
			}


			//Debrisのコンテナに追加
			Debris::debris.push_back(new Debris(pos, startVec * shotSpeed, shotSize,&pos));
		}
		//爆発終了
		size -= maxSize;

	}

	//回収
	if (input->TriggerKey(DIK_1)) {
		for (int i = 0; i < Debris::debris.size(); i++) {
			Debris::debris[i]->ReturnStart();
		}
	}
	

	
	//サイズからスケールへ変換
	scalef = ConvertSizeToScale(size);
	scale = scalef;
	//移動量を適応
	Move();

	//マップチップ用の判定を移動
	rect2d.Top		= -(int)((pos.z + scalef * 100.0f));
	rect2d.Bottom	= -(int)((pos.z - scalef * 100.0f));
	rect2d.Right	= (int)((pos.x + scalef * 100.0f) );
	rect2d.Left		= (int)((pos.x - scalef * 100.0f) );


	//マップチップとの当たり判定
	EdgeType contact_edge = EdgeType::EdgeTypeNon;
	float contact_pos = 0.0f;
	//X軸
	if (MapChip::GetInstance()->CollisionRectAndMapchipEdgeVersion(
		rect2d,
		Vector3(velocity.x, 0, 0),
		contact_edge,
		contact_pos,
		MapChip::TEST_MAP
	)) {
		Vector3 hitPos = {
			contact_pos,
			0,
			pos.z,
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
		HitWall(hitPos, normal);
	}
	//Y軸
	if (MapChip::GetInstance()->CollisionRectAndMapchipEdgeVersion(
		rect2d,
		Vector3(0, 0, velocity.z),
		contact_edge,
		contact_pos,
		MapChip::TEST_MAP
	)) {
		Vector3 hitPos = {
			pos.x,
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
		HitWall(hitPos, normal);
	}
}



void PlayerObject::OnCollision(const CollisionInfo &info)
{
	Debris *debri;
	switch (info.object->Tag)
	{
	case DEBRIS:
		//破片が吸収範囲と衝突したら
		debri = dynamic_cast<Debris *>(info.object);
		if (info.myName == "hitCollider" &&
			info.collider->GetCollisionName() == "hitCollider" &&
			!debri->isFirstAttack) {
			//吸収
			size += debri->GetSize();
		}
		break;
	case ENEMY:
		DebugText::GetInstance()->Print("HitEnemy", 0, 60,3);
		break;

	case DEFAULT_BLOACK:
		DebugText::GetInstance()->Print("HitBox", 0, 60, 3);
		/*
		
		pos = (info.object->pos );
		velocity = CalcWallScratchVector(velocity, info);*/

		break;
		
	default:
		break;
	}
}

void PlayerObject::HitWall(
	const XMVECTOR &hitPos,		//衝突位置
	const Vector3 &normal)
{
	Vector3 HitPos = hitPos;
	pos = HitPos + normal * (rect2d.Bottom - rect2d.Top);
	velocity = CalcWallScratchVector(velocity, normal);


	//マップチップ用の判定を移動
	rect2d.Top = -(int)((pos.z + scalef * 100.0f));
	rect2d.Bottom = -(int)((pos.z - scalef * 100.0f));
	rect2d.Right = (int)((pos.x + scalef * 100.0f));
	rect2d.Left = (int)((pos.x - scalef * 100.0f));
}

void PlayerObject::AdjustToMapchipEdgePosition(EdgeType contact_edge, float contact_pos)
{
	struct Vec2 {
		float x;
		float y;
		Vec2(float x, float y) {
			this->x = x;
			this->y = y;
		}
	};
		
	Vec2 offset = Vec2(rect2d.Bottom - rect2d.Top, rect2d.Bottom - rect2d.Top);

	switch (contact_edge)
	{
	case EdgeTypeLeft:
		pos.x = contact_pos - offset.x/2;
		break;
	case EdgeTypeRight:
		pos.x = contact_pos + offset.x / 2;
		break;
	case EdgeTypeTop:
		pos.z = contact_pos - offset.y/2;
		break;
	case EdgeTypeBottom:
		pos.z = contact_pos + offset.y /2;
		break;
	}
}
