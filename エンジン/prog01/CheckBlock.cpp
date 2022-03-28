#include "CheckBlock.h"
#include "AABBCollider.h"

CheckBlock::CheckBlock(Vector3 pos) :
	BaseBlock(
		pos
	)
{
	SetCollider(new AABBCollider({ -100,-100,-100 }, { 200,200,200}));
	exclusionList.push_back(DEFAULT_BLOACK);
}