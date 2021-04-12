#include "EventHandler.h"
#include "App.h"
#include "Scene.h"
#include "SceneManager.h"

#include <string>

void EventHandler::FireEvent(MapEvent* e)
{
	std::string type = e->attributes->at("type");

	if (type == "changeArea")
	{
		MapScene* s = new MapScene(e->attributes->at("target").c_str());

		int playerX = std::stoi(e->attributes->at("playerX"));
		int playerY = std::stoi(e->attributes->at("playerY"));

		app->scene->LoadScene(s, fPoint(playerX, playerY));
	}
}
