#include "ItemManager.h"
#include "App.h"
#include "Module.h"


#include "SDL/include/SDL_scancode.h"
#include "External/PugiXml/src/pugixml.hpp"
#include "Log.h"

#include <string>
#include <iostream>
using namespace std;

ItemManager::ItemManager() : Module()
{
	name.Create("items");
}

ItemManager::~ItemManager()
{
}

bool ItemManager::Start()
{
	//font = new Font("Assets/Fonts/dungeon_font3.xml", app->tex);

	// ToDo 2: Load the xml file, parse the result, and make a node pointing to quests parent
	///////////////////////////////////////////////////////////////////////////
	pugi::xml_node questNode;
	pugi::xml_document questData;
	pugi::xml_parse_result parseResult = questData.load_file("items.xml");
	if (parseResult == NULL)
		LOG("Could not load xml file <quests.xml> pugi error: %s", parseResult.description());
	else
		questNode = questData.child("quests");
	if (questNode == NULL)
		LOG("Could not load <questsData> xml_document");
	///////////////////////////////////////////////////////////////////////////

	return true;
}

bool ItemManager::Update(float dt)
{

	return true;
}

bool ItemManager::PostUpdate()
{
	return true;
}

bool ItemManager::CleanUp()
{


	return true;
}

