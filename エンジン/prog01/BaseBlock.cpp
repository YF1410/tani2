#include "BaseBlock.h"
#include "AABBCollider.h"

BaseBlock::BaseBlock(Vector3 pos) :
	GameObjCommon(
		ModelManager::DEFAULT_BLOCK,
		DEFAULT_BLOACK,
		false,
		pos
	) 
{
	SetCollider(new AABBCollider({ -100,-100,-100 }, { 200,200,200}));
}