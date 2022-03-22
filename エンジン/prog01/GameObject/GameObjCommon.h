#pragma once
#include "Vector3.h"
#include "ModelManager.h"
#include "FbxObject3d.h"


class GameObjCommon
{
public://サブクラス
	enum Tag {
		Notag,
		MapChip,
		Player,
		Enemy,

		MAX
	};
public:
	GameObjCommon(
		ModelManager::ModelName modelName,	//モデルの名前
		Tag tag = Notag,					//当たり判定用タグ
		bool isGravity = false,				//重力処理の有無
		Vector3 pos = { 0,0,0 },			//初期位置
		Vector3 scale = { 1,1,1 },			//初期サイズ
		Vector3 rotate = { 0,0,0 }			//初期回転
	) :
		Tag(tag),
		isGravity(isGravity),
		pos(pos),
		scale(scale),
		rotate(rotate),
		velocity(0, 0, 0),
		isActive(true),
		isInvisible(false),
		isDelete(false),
		gravityPow(0.4f)
	{
		object = nullptr;
		object = FbxObject3d::Create(ModelManager::GetIns()->GetModel(modelName));
		object->SetModel(ModelManager::GetIns()->GetModel(modelName));
		object->SetPosition(pos);
		object->SetRotation(rotate);
		object->SetScale(scale);
		Initialize();
	}

	~GameObjCommon()
	{
	}

	virtual void Initialize();
	virtual void Update();
	//重力計算と移動適応
	void MovePos();
	//変更を適応
	void Reflection();
	//アニメーション再生
	void PlayAnimation(bool isLoop = false) { object->PlayAnimation(isLoop); };
	virtual void Draw() const;
	
	////正面ベクトル取得
	//Vector3 GetFrontVector() { return Vector3(object->matWorldf._31, object->matWorldf._32, object->matWorldf._33).Normalize(); }
	////右ベクトル取得
	//Vector3 GetRightVector() { return Vector3(object->matWorldf._11, object->matWorldf._12, object->matWorldf._13).Normalize(); }
	////上ベクトル取得
	//Vector3 GetUpVector() { return Vector3(object->matWorldf._21, object->matWorldf._22, object->matWorldf._23).Normalize(); }

	//オブジェクト取得
	//std::unique_ptr<FbxObject3d> GetObjectDate() { return object; }
 
public:


	Vector3 pos;	//座標
	Vector3 rotate;//向き
	float gravityPow;
	Vector3 velocity;	//移動量
	Vector3 scale;	//サイズ

	bool isGravity;	//重力の影響を受けるかどうか
	bool isActive;	//更新フラグ
	bool isInvisible;//透明化フラグ
	bool isDelete;	//消去フラグ

	Tag Tag;		//オブジェクトタグ


protected:
	std::unique_ptr<FbxObject3d> object;

};

