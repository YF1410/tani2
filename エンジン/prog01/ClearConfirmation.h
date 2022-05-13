#pragma once

class ClearConfirmation final
{
private:
	ClearConfirmation() = default;
	~ClearConfirmation() = default;

public:
	ClearConfirmation(const ClearConfirmation& r) = delete;
	ClearConfirmation& operator=(const ClearConfirmation& r) = delete;

	// インスタンス生成
	static ClearConfirmation* GetInstance();

	//getter
	bool GetStage1Flag() { return stage1Flag; }
	bool GetStage2Flag() { return stage2Flag; }
	bool GetStage3Flag() { return stage3Flag; }
	bool GetStage4Flag() { return stage4Flag; }
	bool GetStage5Flag() { return stage5Flag; }
	int& GetMaxUnlockStageNum() { return maxUnlockStageNum; }

	//setter
	void SetStage1Flag(bool _stage1Flag) { stage1Flag = _stage1Flag; }
	void SetStage2Flag(bool _stage2Flag) { stage2Flag = _stage2Flag; }
	void SetStage3Flag(bool _stage3Flag) { stage3Flag = _stage3Flag; }
	void SetStage4Flag(bool _stage4Flag) { stage4Flag = _stage4Flag; }
	void SetStage5Flag(bool _stage5Flag) { stage5Flag = _stage5Flag; }
	void SetMaxUnlockStageNum(int _unlockStageNum);

private: // メンバ変数
	//ステージ1フラグ
	bool stage1Flag = false;
	//ステージ2フラグ
	bool stage2Flag = false;
	//ステージ3フラグ
	bool stage3Flag = false;
	//ステージ4フラグ
	bool stage4Flag = false;
	//ステージ5フラグ
	bool stage5Flag = false;
	//前ステージの場所
	int maxUnlockStageNum = -1;
};