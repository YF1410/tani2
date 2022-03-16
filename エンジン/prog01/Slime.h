#pragma once
#include "FbxObject3d.h"
#include "Input.h"

class Slime : public FbxObject3d
{
public:
	// 3Dオブジェクト生成
	static Slime* Create(FbxModel* model = nullptr);

public:
	// 初期化
	bool Initialize();
	// 毎フレーム処理
	void Update();
};