#pragma once

#include "Module.h"
#include <string>
#include <vector>

struct Member;

class BattleManager : public Module
{

public:

	enum class Selecting {
		MEMBER,
		ACTION,
		TARGET
	};

	BattleManager();
	~BattleManager();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void StartBattle();
	void EndBattle();

	void DoAction();
	void CheckBattleEnd();

private:
	char currentParty = 0;
	char currentMember = 0;
	char currentAction = 0;
	char currentTarget = 0;

	std::vector<Member*> targets;

	Selecting selecting = Selecting::MEMBER;

	bool isBattling = false;
};