#include "ClearConfirmation.h"

ClearConfirmation* ClearConfirmation::GetInstance() {
	static ClearConfirmation instance;
	return &instance;
}

void ClearConfirmation::SetMaxUnlockStageNum(int _unlockStageNum) {
	if (maxUnlockStageNum < _unlockStageNum && _unlockStageNum <= 5) {
		maxUnlockStageNum = _unlockStageNum;
	}
}