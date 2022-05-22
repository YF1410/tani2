#pragma once

class MoveCheck final
{
private:
	MoveCheck() = default;
	~MoveCheck() = default;

public:
	MoveCheck(const MoveCheck& r) = delete;
	MoveCheck& operator=(const MoveCheck& r) = delete;

	bool IsMoveFlag();
	void SetMoveFlag(bool _isMoveFlag) { isMoveFlag = _isMoveFlag; }

	// �C���X�^���X����
	static MoveCheck* GetInstance();

private:
	bool isMoveFlag = true;
};