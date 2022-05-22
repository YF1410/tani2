#include "MoveCheck.h"

MoveCheck* MoveCheck::GetInstance()
{
	static MoveCheck instance;
	return &instance;
}