#pragma once
#include "Object3d.h"
#include "Input.h"

class Slime : public Object3d
{
public:
	// 3Dオブジェクト生成
	static Slime* Create(Model* model = nullptr);

public:
	// 初期化
	bool Initialize() override;
	// 毎フレーム処理
	void Update() override;
};