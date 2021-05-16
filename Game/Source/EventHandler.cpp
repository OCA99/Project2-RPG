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
	if (type == "graveyardPuzzle")
	{
		switch (e->eventId)
		{
		case 0:
			if (app->puzzleManager->currentPuzzle == PuzzleManager::ActivePuzzle::SEWERENTRANCE)
			{
				if (!app->puzzleManager->key1) app->puzzleManager->key1 = true;

			}
			break;
		case 1:
			if (app->puzzleManager->currentPuzzle == PuzzleManager::ActivePuzzle::SEWERENTRANCE)
			{
				if (!app->puzzleManager->key2) app->puzzleManager->key2 = true;


			}

			break;
		case 2:
			if (app->puzzleManager->currentPuzzle == PuzzleManager::ActivePuzzle::SEWERENTRANCE)
			{
				if (!app->puzzleManager->key3) app->puzzleManager->key3 = true;
			}
			break;
		default:
			break;
		}
	}

	if (type == "CagePuzzle")
	{
		switch (e->eventId)
		{
		case 5:
			if (app->puzzleManager->currentPuzzle == PuzzleManager::ActivePuzzle::CAGEPUZZLE)
			{
				if (app->puzzleManager->key1) app->puzzleManager->key1 = false;
				if (app->puzzleManager->key2) app->puzzleManager->key2 = false;
				if (app->puzzleManager->key3) app->puzzleManager->key3 = false;
				if (app->puzzleManager->key4) app->puzzleManager->key4 = false;
				if (app->puzzleManager->key5) app->puzzleManager->key5 = false;

				app->puzzleManager->ClearButtons();
			}
			break;
		case 6:
			if (app->puzzleManager->currentPuzzle == PuzzleManager::ActivePuzzle::CAGEPUZZLE)
			{
				if (!app->puzzleManager->key2) app->puzzleManager->key2 = true;
				if (app->puzzleManager->key1) app->puzzleManager->key1 = false;
			}
			break;
		case 7:
			if (app->puzzleManager->currentPuzzle == PuzzleManager::ActivePuzzle::CAGEPUZZLE)
			{
				if (!app->puzzleManager->key1) app->puzzleManager->key1 = true;
				if (app->puzzleManager->key2) app->puzzleManager->key2 = false;
			}
			break;
		default:
			break;
		}
	}

}
