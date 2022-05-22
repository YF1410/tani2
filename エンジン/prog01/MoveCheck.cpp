#include "MoveCheck.h"

bool MoveCheck::IsMoveFlag()
{
	if (isMoveFlag)
	{
		return true;
	}
	return false;
}

MoveCheck* MoveCheck::GetInstance()
{
	static MoveCheck instance;
	return &instance;
}