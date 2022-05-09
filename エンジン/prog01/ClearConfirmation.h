#pragma once

class ClearConfirmation final
{
private:
	ClearConfirmation() = default;
	~ClearConfirmation() = default;

public:
	ClearConfirmation(const ClearConfirmation& r) = delete;
	ClearConfirmation& operator=(const ClearConfirmation& r) = delete;

	// �C���X�^���X����
	static ClearConfirmation* GetInstance();

	//getter
	bool isTutorialFlag() { return tutorialFlag; }
	bool isStage1Flag() { return stage1Flag; }
	bool isStage2Flag() { return stage2Flag; }
	bool isStage3Flag() { return stage3Flag; }
	bool isStage4Flag() { return stage4Flag; }
	bool isStage5Flag() { return stage5Flag; }
	int GetStageNum() { return stageNum; }

	//setter
	void SetTutorialFlag(bool _tutorialFlag) { tutorialFlag = _tutorialFlag; }
	void SetStage1Flag(bool _stage1Flag) { stage1Flag = _stage1Flag; }
	void SetStage2Flag(bool _stage2Flag) { stage2Flag = _stage2Flag; }
	void SetStage3Flag(bool _stage3Flag) { stage3Flag = _stage3Flag; }
	void SetStage4Flag(bool _stage4Flag) { stage4Flag = _stage4Flag; }
	void SetStage5Flag(bool _stage5Flag) { stage5Flag = _stage5Flag; }
	void SetStageNum(int _stageNum) { stageNum = _stageNum; }

private: // �����o�ϐ�
	//�`���[�g���A���t���O
	bool tutorialFlag = false;
	//�X�e�[�W1�t���O
	bool stage1Flag = false;
	//�X�e�[�W2�t���O
	bool stage2Flag = false;
	//�X�e�[�W3�t���O
	bool stage3Flag = false;
	//�X�e�[�W4�t���O
	bool stage4Flag = false;
	//�X�e�[�W5�t���O
	bool stage5Flag = false;
	//�O�X�e�[�W�̏ꏊ
	int stageNum = -1;
};