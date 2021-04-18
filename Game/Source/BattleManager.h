#pragma once

#include "Module.h"
#include <string>
#include <vector>

#include "SDL/include/SDL.h"

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

	void WaitTime(float time);

	void PlayAITurn();

private:
	void Draw();
	SDL_Texture* characterBar = nullptr;
	SDL_Texture* actionBox = nullptr;
	SDL_Texture* healthBars = nullptr;
	SDL_Texture* selectionArrow = nullptr;
	SDL_Texture* selectionArrowHorizontal = nullptr;
	SDL_Texture* selectionArrowGreen = nullptr;
	SDL_Texture* selectionArrowRed = nullptr;

	char currentParty = 0;
	char currentMember = 0;
	char currentAction = 0;
	char currentTarget = 0;

	std::vector<Member*> targets;

	Selecting selecting = Selecting::MEMBER;

	bool isBattling = false;

	float pendingWaitTime = 0;

	bool end = false;
	float endWait = 0;
};