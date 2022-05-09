#pragma once
#include "EnemyHelper.h"
class EnemyHelperManager
{
private:
	EnemyHelperManager() = default;
	~EnemyHelperManager() = default;
	EnemyHelperManager(const EnemyHelperManager & r) = default;
	EnemyHelperManager &operator= (const EnemyHelperManager & r) = default;

public:
	static EnemyHelperManager *GetIns();		//Žæ“¾—p
	void Update();
	void LustUpdate();
	void Draw() const;
	static std::vector<EnemyHelper *> enemyHelpers;
};

