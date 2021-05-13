#include "ItemManager.h"
#include "App.h"
#include "Module.h"
#include "Input.h"
#include "Render.h"
#include "Fonts.h"
#include "DialogSytem.h"
#include "GuiManager.h"

#include "SDL/include/SDL_scancode.h"
#include "External/PugiXml/src/pugixml.hpp"
#include "Log.h"


#include <iostream>
#include "ToUpperCase.h"
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

	invMenu = app->tex->Load("Assets/Textures/UI/HUD/charactermenu.png");

	return true;
}

bool ItemManager::Update(float dt)
{

	return true;
}

bool ItemManager::PostUpdate(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		invOpened = !invOpened;

	if (invOpened)
	{
		app->render->DrawTexture(invMenu, 0, 0, &SDL_Rect({0,0,1080,720}), 0.5f, 1, 0, 0, 0, false);
		DrawPlayerItems();
		app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 140 / 2, 152 / 2, 340 / 2, 65 / 2 }), 15); //BUTTON TO SHOW ITEM DESCRIPTION WITH THE MOUSE
	}
	if (!invOpened)
		app->ui->DestroyAllGuiControls();
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
		app->render->DrawTexture(item->data->itemTex, 45, 75 + 32 * y, (SDL_Rect*)(0, 0, 0, 0), 1.0f, 1, 0, 0, 0, false);
		//DRAW TEXT
		std::string text = ToUpperCase(item->data->title.GetString());
		app->fonts->BlitText(75, 80 + (32 * y), 0, text.c_str());


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

