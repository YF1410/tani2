#pragma once

#include "CollisionInfo.h"
#include "GameObjCommon.h"


//衝突判定用関連の型定義
enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // 未設定

	SHAPE_POINT		= 0x001,	//点
	SHAPE_RAY		= 0x002,	//線（終端なし）
	SHAPE_LINE		= 0x004,	//線（終端あり）
	SHAPE_TRIANGLE	= 0x008,	//三角形
	SHAPE_SPHERE	= 0x010,	//球
	SHAPE_CYLINDER	= 0x020,	//筒
	SHAPE_AABB		= 0x040,	//AABB
	SHAPE_OBB		= 0x080,	//OBB
	SHAPE_CAPSULE	= 0x100,	//カプセル
	SHAPE_MESH		= 0x200,	//メッシュ
	// = 0x000	//コリジョンタイプ
};

class BaseCollider
{
public:
	friend class CollisionManager;
	BaseCollider() = default;
	virtual ~BaseCollider() = default;		//仮想デストラクタ

	//オブジェクトをセット
	inline void SetObject(GameObjCommon *object) { this->object = object; }
	//セットされているオブジェクトをリターン
	inline GameObjCommon *GetObject3d() { return object; }

	//更新
	virtual void Update() = 0;
	//形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType; }

	//衝突時コールバック関数
	inline void OnCollision(const CollisionInfo& info)
	{
		object->OnCollision(info);
	}

	// 当たり判定属性をセット
	inline void SetAttribute(unsigned short attribute)
	{
		this->attribute = attribute;
	}

	// 当たり判定属性を追加
	inline void AddAttribute(unsigned short attribute)
	{
		this->attribute |= attribute;
	}

	// 当たり判定属性を削除
	inline void RemoveAttribute(unsigned short attribute)
	{
		this->attribute &= !attribute;
	}

protected:
	GameObjCommon *object = nullptr;
	//形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	// 当たり判定属性
	unsigned short attribute = 0b1111111111111111;
};