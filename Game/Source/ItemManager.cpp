#include "ItemManager.h"
#include "App.h"
#include "Module.h"

#include "Render.h"


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

	pugi::xml_node itemNode;
	pugi::xml_document itemData;
	pugi::xml_parse_result parseResult = itemData.load_file("items.xml");
	if (parseResult == NULL)
		LOG("Could not load xml file <items.xml> pugi error: %s", parseResult.description());
	else
		itemNode = itemData.child("items");
	if (itemNode == NULL)
		LOG("Could not load <itemsData> xml_document");


	itemNode = itemNode.child("item");
	while (itemNode != NULL)
	{
		Item* item = new Item();

		item->type = itemNode.attribute("type").as_int();
		item->title = itemNode.attribute("title").as_string();
		item->description = itemNode.attribute("description").as_string();
		item->objective = itemNode.attribute("objective").as_string();
		item->quantity = itemNode.attribute("quantity").as_int();
		item->rewardingNPC = itemNode.attribute("rewardingNPC").as_string();
		item->sellCost = itemNode.attribute("sellCost").as_int();
		item->buyCost = itemNode.attribute("buyCost").as_int();
		item->questItem = itemNode.attribute("questItem").as_bool();
		item->texturePath = itemNode.attribute("texturePath").as_string();
		item->itemTex = app->tex->Load(SString("Assets/Textures/Items/%s", item->texturePath.GetString()).GetString());
		itemList.Add(item);
		itemNode = itemNode.next_sibling("item");
	}

	//TEMPORAL
	playerItemList.Add(SearchForItem(SString("Wooden Sword")));
	playerItemList.Add(SearchForItem(SString("Leather Helmet")));
	playerItemList.Add(SearchForItem(SString("Magic Dust")));



	return true;
}

bool ItemManager::Update(float dt)
{

	return true;
}

bool ItemManager::PostUpdate(float dt)
{
	DrawPlayerItems();
	return true;
}

bool ItemManager::CleanUp()
{


	return true;
}

void ItemManager::DrawPlayerItems()
{
	ListItem<Item*>* item = playerItemList.start;
	y = 0;
	while (item)
	{
		//Draw Texture
		app->render->DrawTexture(item->data->itemTex, 0, 16 * y, (SDL_Rect*)(0,0,0,0), 1);
		++y;
		item = item->next;
	}


}

Item* ItemManager::SearchForItem(SString& itemTitle)
{
	ListItem<Item*>* item = itemList.start;
	while (item)
	{
		if (item->data->title == itemTitle)
			return item->data;
		item = item->next;
	}
	LOG("couldn't find %s in the item list", itemTitle.GetString());
	return nullptr;
}

