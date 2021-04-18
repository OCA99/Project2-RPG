#include "BattleManager.h"

#include "App.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PartyManager.h"
#include "Render.h"
#include "Window.h"
#include "Input.h"
#include "Fonts.h"
#include <cmath>

#include <algorithm>
#include <string>

BattleManager::BattleManager()
{
}

BattleManager::~BattleManager()
{
}

bool BattleManager::Awake()
{
	return true;
}

bool BattleManager::Start()
{
	characterBar = app->tex->Load("Assets/Textures/UI/BattleMenu/character_bar.png");
	actionBox = app->tex->Load("Assets/Textures/UI/BattleMenu/action_box.png");
	healthBars = app->tex->Load("Assets/Textures/UI/BattleMenu/health_bars.png");
	selectionArrow = app->tex->Load("Assets/Textures/UI/BattleMenu/selection_arrow.png");
	selectionArrowHorizontal = app->tex->Load("Assets/Textures/UI/BattleMenu/selection_arrow_horizontal.png");
	selectionArrowGreen = app->tex->Load("Assets/Textures/UI/BattleMenu/selection_arrow_green.png");
	selectionArrowRed = app->tex->Load("Assets/Textures/UI/BattleMenu/selection_arrow_red.png");
	

	return true;
}

bool BattleManager::PreUpdate()
{
	return true;
}

bool BattleManager::Update(float dt)
{
	if (end)
	{
		endWait -= dt;

		if (endWait <= 0)
			EndBattle();

		return true;
	}

	if (pendingWaitFrames > 0)
	{
		pendingWaitFrames -= 1;
		return true;
	}

	if (!isBattling) return true;

	Party* p;
	Party* o;

	if (currentParty == 0)
	{
		p = app->party->allyParty;
		o = app->party->enemyParty;
	}
	else
	{
		p = app->party->enemyParty;
		o = app->party->allyParty;
	}

	while (currentMember < p->list.size() && p->list.at(currentMember)->data.dead)
		currentMember += 1;

	if (currentMember == p->list.size())
		end = true;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (selecting == Selecting::MEMBER)
			selecting = Selecting::ACTION;
		else if (selecting == Selecting::ACTION)
		{
			selecting = Selecting::TARGET;

			Member* m = p->list.at(currentMember);
			Action* a = m->data.actions.at(currentAction);

			if (a->filter == Action::Filter::ENEMY)
			{
				for (int i = 0; i < o->list.size(); i++)
				{
					if (!o->list.at(i)->data.dead)
						targets.push_back(o->list.at(i));
				}
			}
		}
		else if (selecting == Selecting::TARGET)
			DoAction();
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		if (selecting == Selecting::ACTION)
			currentAction += 1;
		if (selecting == Selecting::TARGET)
			currentTarget += 1;
		if (currentAction == p->list.at(currentMember)->data.actions.size()) currentAction -= 1;
		if (currentTarget == o->list.size()) currentTarget -= 1;
		LOG("%d, %d", currentAction, currentTarget);
	}

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		if (selecting == Selecting::ACTION)
			currentAction -= 1;
		if (selecting == Selecting::TARGET)
			currentTarget -= 1;
		if (currentAction < 0) currentAction = 0;
		if (currentTarget < 0) currentTarget = 0;
		LOG("%d, %d", currentAction, currentTarget);
	}

	return true;
}

bool BattleManager::PostUpdate(float dt)
{
	Draw();
	return true;
}

bool BattleManager::CleanUp()
{
	return true;
}

void BattleManager::StartBattle()
{
	app->party->InitializeParties();

	isBattling = true;
	currentParty = 0;
	currentMember = 0;
	selecting = Selecting::ACTION;

	end = false;
	endWait = 4.0f;
}

void BattleManager::EndBattle()
{
	isBattling = false;
	MapScene* s = new MapScene("Forest.tmx");
	app->scene->sceneToBeLoaded = (Scene*)s;
	app->scene->playerPositionToBeLoaded.x = 944;
	app->scene->playerPositionToBeLoaded.y = 240;

	app->party->CleanUp();

	end = false;
}

void BattleManager::DoAction()
{
	Party* p;
	Party* o;

	if (currentParty == 0)
	{
		p = app->party->allyParty;
		o = app->party->enemyParty;
	}
	else
	{
		p = app->party->enemyParty;
		o = app->party->allyParty;
	}

	Member* m = p->list.at(currentMember);
	Action* a = m->data.actions.at(currentAction);

	Member* t = targets.at(currentTarget);

	a->Apply(t);

	currentMember += 1;
	currentAction = 0;
	currentTarget = 0;

	targets.clear();

	selecting = Selecting::ACTION;

	if (currentMember == p->list.size())
	{
		if (currentParty == 0) currentParty = 1;
		else currentParty = 0;
		currentMember = 0;
	}

	CheckBattleEnd();
}

void BattleManager::CheckBattleEnd()
{
	Party* p;
	Party* o;

	if (currentParty == 0)
	{
		p = app->party->allyParty;
		o = app->party->enemyParty;
	}
	else
	{
		p = app->party->enemyParty;
		o = app->party->allyParty;
	}

	bool A = true;

	for (int i = 0; i < p->list.size(); i++)
	{
		if (!p->list.at(i)->data.dead) A = false;
	}

	bool B = true;

	for (int i = 0; i < o->list.size(); i++)
	{
		if (!o->list.at(i)->data.dead) B = false;
	}

	if (A || B) end = true;
}

void BattleManager::WaitFrames(int frames)
{
	pendingWaitFrames = frames;
}

void BattleManager::Draw()
{
	if (!isBattling) return;

	Party* party = app->party->allyParty;

	for (int i = 0; i < party->list.size(); i++) {
		int x = 70;
		uint w, h;
		app->win->GetWindowSize(w, h);
		int y = h / 2 - 45 - 38 * i;
		app->render->DrawTexture(characterBar, x, y, NULL, 0.5f);
		std::string name = party->list.at(i)->name;
		std::transform(name.begin(), name.end(), name.begin(), ::toupper);
		app->fonts->BlitText(x + 30, y + 20, 1, name.c_str());
		int p = 10 - std::floor(party->list.at(i)->data.health / party->list.at(i)->data.maxHealth * 11.0f);
		if (p == -1) p = 0;
		if (p > 10) p = 10;
		SDL_Rect section = SDL_Rect({ 0, 30 * p, 300, 30 });
		app->render->DrawTexture(healthBars, x + 250, y + 15, &section, 0.5f);

		if (i == currentMember && currentParty == 0)
		{
			app->render->DrawTexture(selectionArrow, 75, 30 + i * 80.5f, NULL, .5f);

			std::vector<Action*>* actions = &party->list.at(i)->data.actions;
			for (int j = 0; j < actions->size(); j++) {
				Action* a = actions->at(j);

				x = 90;
				y = h / 2 - 45 - 38 * (party->list.size() - 1) - 32 - j * 25;
				app->render->DrawTexture(actionBox, x, y, NULL, .5f);
				std::transform(a->name.begin(), a->name.end(), a->name.begin(), ::toupper);
				app->fonts->BlitText(x + 25, y + 13, 1, a->name.c_str());

				if (j == currentAction) {
					app->render->DrawTexture(selectionArrowHorizontal, x + 80, y + 12, NULL, 0.3f);
				}
			}
		}
	}

	if (selecting == Selecting::TARGET && currentParty == 0) {
		for (int i = 0; i < party->list.size(); i++) {
			for (int j = 0; j < targets.size(); j++) {
				if (j == currentTarget) {
					if (party->list.at(i) == targets.at(j)) {
						app->render->DrawTexture(selectionArrowGreen, 75, 30 + i * 80.5f, NULL, .5f);
					}
				}
			}
		}

		party = app->party->enemyParty;

		for (int i = 0; i < party->list.size(); i++) {
			for (int j = 0; j < targets.size(); j++) {
				if (j == currentTarget) {
					if (party->list.at(i) == targets.at(j)) {
						app->render->DrawTexture(selectionArrowRed, 495, 10 + i * 80.5f, NULL, .5f);
					}
				}
			}
		}
	}

	party = app->party->enemyParty;

	for (int i = 0; i < party->list.size(); i++) {
		Member* m = party->list.at(i);

		int p = 10 - std::floor(m->data.health / m->data.maxHealth * 11.0f);
		if (p == -1) p = 0;
		if (p > 10) p = 10;
		SDL_Rect section = SDL_Rect({ 0, 30 * p, 300, 30 });

		app->render->DrawTexture(healthBars, 480.0f, 20 + i * 80.5f, &section, 0.25f);
	}
}
