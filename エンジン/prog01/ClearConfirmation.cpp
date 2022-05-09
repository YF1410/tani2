#include "ClearConfirmation.h"

ClearConfirmation* ClearConfirmation::GetInstance()
{
	static ClearConfirmation instance;
	return &instance;
}