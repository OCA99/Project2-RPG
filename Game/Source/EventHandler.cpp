#include "EventHandler.h"
#include "App.h"
#include "Audio.h"
#include "Components.h"
#include "ECS.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SceneTransitionSystem.h"
#include "PuzzleManager.h"

#include <string>

void EventHandler::FireEvent(MapEvent* e)
{
	std::string type = e->attributes->at("type");

	if (type == "changeArea")
	{

		if (e->attributes->at("target").c_str() == std::string("Sewers.tmx") && !app->puzzleManager->completed && app->puzzleManager->currentPuzzle == PuzzleManager::ActivePuzzle::SEWERENTRANCE)
		{
			LOG("NOPE");
		}
		else
		{
			MapScene* s = new MapScene(e->attributes->at("target").c_str());

			int playerX = std::stoi(e->attributes->at("playerX"));
			int playerY = std::stoi(e->attributes->at("playerY"));

			app->scene->sceneToBeLoaded = (Scene*)s;
			std::string str = s->filename;
			if (str == "Tavern.tmx" || str == "NPC House.tmx")
			{
				app->audio->PlayFx(4, 0);
			}
			app->scene->changingScene = true;
			app->scene->playerPositionToBeLoaded = fPoint(playerX, playerY);
		}
	}

	//PUZZLE EVENTS
	if (type == "puzzleSensor")
	{
		switch (e->eventId)
		{
		case 0:
			if (!app->puzzleManager->key1) app->puzzleManager->key1 = true;
			LOG("ONE");
			break;
		case 1:
			if (!app->puzzleManager->key2) app->puzzleManager->key2 = true;
			LOG("TWO");
			break;
		case 2:
			if (!app->puzzleManager->key3) app->puzzleManager->key3 = true;
			LOG("three");
			break;
		default:
			break;
		}
	}

}
