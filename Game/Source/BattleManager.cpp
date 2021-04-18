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

	return true;
}

bool BattleManager::PreUpdate()
{
	return true;
}

bool BattleManager::Update(float dt)
{
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

	while (currentMember < p->list.Count() && p->list.At(currentMember)->data->data.dead)
		currentMember += 1;

	if (currentMember == p->list.Count())
		EndBattle();

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (selecting == Selecting::MEMBER)
			selecting = Selecting::ACTION;
		else if (selecting == Selecting::ACTION)
		{
			selecting = Selecting::TARGET;

			Member* m = p->list.At(currentMember)->data;
			Action* a = m->data.actions.At(currentAction)->data;

			if (a->filter == Action::Filter::ENEMY)
			{
				for (int i = 0; i < o->list.Count(); i++)
				{
					if (!o->list.At(i)->data->data.dead)
						targets.push_back(o->list.At(i)->data);
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
		if (currentAction == p->list.At(currentMember)->data->data.actions.Count()) currentAction -= 1;
		if (currentTarget == o->list.Count()) currentTarget -= 1;
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
}

void BattleManager::EndBattle()
{
	isBattling = false;
	MapScene* s = new MapScene("Forest.tmx");
	app->scene->sceneToBeLoaded = (Scene*)s;
	app->scene->playerPositionToBeLoaded.x = 944;
	app->scene->playerPositionToBeLoaded.y = 240;

	app->party->CleanUp();
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

	Member* m = p->list.At(currentMember)->data;
	Action* a = m->data.actions.At(currentAction)->data;

	Member* t = targets.at(currentTarget);

	a->Apply(t);

	currentMember += 1;
	currentAction = 0;
	currentTarget = 0;

	targets.clear();

	selecting = Selecting::ACTION;

	if (currentMember == p->list.Count())
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

	for (int i = 0; i < p->list.Count(); i++)
	{
		if (!p->list.At(i)->data->data.dead) A = false;
	}

	bool B = true;

	for (int i = 0; i < o->list.Count(); i++)
	{
		if (!o->list.At(i)->data->data.dead) B = false;
	}

	if (A || B) EndBattle();
}

void BattleManager::Draw()
{
	if (!isBattling) return;

	Party* ally = app->party->allyParty;

	for (int i = 0; i < ally->list.Count(); i++) {
		int x = 70;
		uint w, h;
		app->win->GetWindowSize(w, h);
		int y = h / 2 - 45 - 38 * i;
		app->render->DrawTexture(characterBar, x, y, NULL, 0.5f);
		std::string name = ally->list.At(i)->data->name;
		std::transform(name.begin(), name.end(), name.begin(), ::toupper);
		app->fonts->BlitText(x + 30, y + 20, 1, name.c_str());
		int p = 10 - std::floor(ally->list.At(i)->data->data.health / 100.0f * 11.0f);
		if (p == -1) p = 0;
		if (p > 10) p = 10;
		SDL_Rect section = SDL_Rect({ 0, 30 * p, 300, 30 });
		app->render->DrawTexture(healthBars, x + 250, y + 15, &section, 0.5f);

		if (i == currentMember && currentParty == 0)
		{
			app->render->DrawTexture(selectionArrow, 75, 30 + i * 80.5f, NULL, .5f);
		}

		if (currentParty == 0)
		{
			List<Action*> actions = ally->list.At(i)->data->data.actions;
			for (int j = 0; j < actions.Count(); j++) {
				Action* a = actions.At(j)->data;

				x = 90;
				y = h / 2 - 45 - 38 * i - 32 - j * 25;
				app->render->DrawTexture(actionBox, x, y, NULL, .5f);
			}
		}
	}
}
