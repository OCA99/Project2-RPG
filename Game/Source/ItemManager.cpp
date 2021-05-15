#include "ItemManager.h"
#include "App.h"
#include "Module.h"
#include "Input.h"
#include "Render.h"
#include "Fonts.h"
#include "DialogSytem.h"
#include "SceneManager.h"
#include "QuestManager.h"
#include "Scene.h"
#include "PartyManager.h"

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
		item->itemTex = app->tex->Load(SString("Textures/Items/%s", item->texturePath.GetString()).GetString());

		itemList.Add(item);
		itemNode = itemNode.next_sibling("item");
	}

	GiveItemToPlayer(SString("Coin"));
	GiveItemToPlayer(SString("Coin"));
	GiveItemToPlayer(SString("Coin Stack"));
	GiveItemToPlayer(SString("Coin Stack"));
	GiveItemToPlayer(SString("HP Potion"));
	GiveItemToPlayer(SString("HP Potion"));
	GiveItemToPlayer(SString("EXP Potion"));
	GiveItemToPlayer(SString("Treasure Chest"));


	itemDescTex = app->tex->Load("Textures/UI/OptionsMenu/item_description.png");

	return true;
}

bool ItemManager::Update(float dt)
{



	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && app->scene->currentScene->type == Scene::TYPE::MAP && !app->scene->menu && !app->quests->questInvOpened)
		invOpened = !invOpened;//Open or close Inv

	if (invOpened)
	{
		CreateButtons();
	}

	return true;
}

bool ItemManager::PostUpdate(float dt)
{
	
	if (invOpened)//If the inventory is opened bool
	{
		DrawPlayerItems();//Draw Items Image and Title
		ShowDescription();//Show
		DrawPlayerStats();
	}

	if (!invOpened)//If it close, we delete the buttons
	{
		CleanUp();
	}

	return true;
}

bool ItemManager::CleanUp()
{
	DeleteButtons();
	DeleteActionButtons();

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
		app->fonts->BlitText(75, 80 + (32 * y), 1, text.c_str());

		++y;
		item = item->next;
	}

}

void ItemManager::GiveItemToPlayer(SString& itemTitle)
{
	{
		if (playerItemList.Count() >= MAX_ITEMS)
		{
			std::cout << "You can't add more items to your bag" << std::endl;
		}
		else
			playerItemList.Add(SearchForItem(itemTitle));

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

void ItemManager::ShowDescription()
{
	ListItem<GuiControl*>* item = buttons.start;
	int b = 0;
	while (item)
	{
		if (b > playerItemList.Count()) 
			b = 0;
		if (item->data->itemSingleCheck)
			CreateActionButtons(b);
		if (item->data->itemCheck)
		{
			
			if (b < playerItemList.Count())
			{
				//Draw Texture
				app->render->DrawTexture(itemDescTex, item->data->bounds.x + 32, item->data->bounds.y + 30, &SDL_Rect({ 0,0,384,96 }), 0.5f, 0, 0, 0, 0, false);

				//Draw Text
				std::string text = ToUpperCase(playerItemList[b]->description.GetString());
				app->fonts->BlitText(item->data->bounds.x + 37, item->data->bounds.y + 35, 1, text.c_str());
			}

		}
		++b;
		item = item->next;
	}

}

void ItemManager::CreateActionButtons(int y)
{
	if (actionButtons.Count() <= 0)
	{
		actionButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 205 , 55 + 32 * y , 100,20 }), 19)); //BUTTON TO SHOW ITEM DESCRIPTION WITH THE MOUSE
		actionButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 205 , 75 + 32 * y , 100, 20 }), 20)); //BUTTON TO SHOW ITEM DESCRIPTION WITH THE MOUSE

	}
	CheckActionButtons();

}

void ItemManager::CreateButtons()
{
	if (buttons.Count() <= 0)
	{
		exitButton = app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 30 , 15, 30, 30 }), 14);//CREATE EXIT BUTTON
		ListItem<Item*>* item = playerItemList.start;
		y = 0;
		while (item)
		{
			buttons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 35 , 65 + 32 * y, 340 / 2, 70 / 2 }), 17)); //BUTTON TO SHOW ITEM DESCRIPTION WITH THE MOUSE
			LOG("%d", y);
			y++;
			item = item->next;
		}
	}

}

void ItemManager::DeleteButtons()
{
	app->ui->DestroyGuiControl(exitButton);
	ListItem<GuiControl*>* item = buttons.start;
	while (item)
	{

		//if (item->data->id == 17 || item->data->id == 14)
		app->ui->DestroyGuiControl(item->data);

		item = item->next;
	}
	buttons.Clear();
}

void ItemManager::DeleteActionButtons()
{
	ListItem<GuiControl*>* item = actionButtons.start;
	while (item)
	{
		if (item->data->id == 19 || item->data->id == 20)
			app->ui->DestroyGuiControl(item->data);

		item = item->next;
	}
	actionButtons.Clear();
}

void ItemManager::UseItem(Item* itemtoUse, int y)
{
	//EFECTO DE CADA ITEM
	if (itemtoUse->title == SString("HP Potion"))
	{
		app->party->allyParty->FindByName("Thyma")->data.Addhealth(15.f);
	}
	if (itemtoUse->title == SString("EXP Potion"))
	{
		app->party->allyParty->FindByName("Thyma")->data.AddExp(17.f);
	}
	if (itemtoUse->title == SString("Coin"))
	{
		app->party->allyParty->FindByName("Thyma")->data.AddMoney(1);
	}
	if (itemtoUse->title == SString("Coin Stack"))
	{
		app->party->allyParty->FindByName("Thyma")->data.AddMoney(50.f);
	}
	if (itemtoUse->title == SString("Treasure Chest"))
	{
		app->party->allyParty->FindByName("Thyma")->data.AddMoney(150.f);
	}




	int a = 0;
	ListItem<Item*>* item = playerItemList.start;
	while (item)
	{
		if (a == y)
		{
			ListItem<Item*>* c = playerItemList.At(a);
			playerItemList.Del(c);
		}

		a++;
		item = item->next;
		
	}

}

void ItemManager::CheckActionButtons()
{
	y = 0;
	ListItem<GuiControl*>* item = buttons.start;
	while (item)
	{
		if (actionButtons.Count() > 0 && actionButtons[0]->itemUsed == false && actionButtons[0]->discarItem == false)
		{
			actionButtons[0]->Draw(app->render);
			actionButtons[1]->Draw(app->render);

		}
		
		if (actionButtons.Count() > 0  && actionButtons[0]->itemUsed && item->data->itemSingleCheck)
		{
			item->data->itemSingleCheck = false;

			UseItem(playerItemList[y], y);
			DeleteButtons();
			DeleteActionButtons();

		}

		if (actionButtons.Count() > 0 && actionButtons[1]->discarItem && item->data->itemSingleCheck)
		{
			item->data->itemSingleCheck = false;

			DeleteActionButtons();

		}

		++y;
		item = item->next;
	}

}

void ItemManager::DrawPlayerStats()
{
	//if(Member* m = app->party->allyParty->FindByName(std::string("Thyma")))
	//int h = app->party->allyParty->FindByName("thyma")->data.GetHealth();
	float hp = app->party->allyParty->FindByName("Thyma")->data.GetHealth();
	float exp = app->party->allyParty->FindByName("Thyma")->data.GetExp();

	float health = (app->party->allyParty->FindByName("Thyma")->data.GetHealth() * 143) / 100;
	float experience = (app->party->allyParty->FindByName("Thyma")->data.GetExp() * 143) / 100;

	app->render->DrawRectangle({ 357,367,143, 9 }, 147, 147, 147, 255);//BASE COLOR
	app->render->DrawRectangle({ 357,392,143, 9 }, 147, 147, 147, 255);//BASE COLOR

	if(hp > 85)
		app->render->DrawRectangle({ 357,367,(int)health, 9 }, 50, 85, 95, 255);//BLUE
	
	if (hp <= 85)
		app->render->DrawRectangle({ 357,367,(int)health, 9 }, 50, 89, 83, 255);// LIGHT BLUE

	if (hp <= 75)
		app->render->DrawRectangle({ 357,367,(int)health, 9 }, 51, 81, 48, 255);//DARK GREEN

	if (hp <= 67)
		app->render->DrawRectangle({ 357,367,(int)health, 9 }, 71, 89, 50, 255);//GREEN

	if (hp <= 60)
		app->render->DrawRectangle({ 357,367,(int)health, 9 }, 100, 106, 51, 255);//LIGHT GREEN

	if (hp <= 50)
		app->render->DrawRectangle({ 357,367,(int)health, 9 }, 123, 100, 51, 255);//YELLOW

	if (hp <= 40)
		app->render->DrawRectangle({ 357,367,(int)health, 9 }, 123, 90, 52, 255);//LIGHT GREEN

	if (hp <= 30)
		app->render->DrawRectangle({ 357,367,(int)health, 9 }, 123, 77, 52, 255);//ORANGE

	if (hp <= 20)
		app->render->DrawRectangle({ 357,367,(int)health, 9 }, 102, 60, 49, 255);//DARK ORANGE

	if (hp <= 10)
		app->render->DrawRectangle({ 357,367,(int)health, 9 }, 102, 49, 49, 255);//DARK RED


	if (exp > 85)
		app->render->DrawRectangle({ 357,392,(int)experience, 9 }, 50, 85, 95, 255);//BLUE

	if (exp <= 85)
		app->render->DrawRectangle({ 357,392,(int)experience, 9 }, 50, 89, 83, 255);// LIGHT BLUE

	if (exp <= 75)
		app->render->DrawRectangle({ 357,392,(int)experience, 9 }, 51, 81, 48, 255);//DARK GREEN

	if (exp <= 67)
		app->render->DrawRectangle({ 357,392,(int)experience, 9 }, 71, 89, 50, 255);//GREEN

	if (exp <= 60)
		app->render->DrawRectangle({ 357,392,(int)experience, 9 }, 100, 106, 51, 255);//LIGHT GREEN

	if (exp <= 50)
		app->render->DrawRectangle({ 357,392,(int)experience, 9 }, 123, 100, 51, 255);//YELLOW

	if (exp <= 40)
		app->render->DrawRectangle({ 357,392,(int)experience, 9 }, 123, 90, 52, 255);//LIGHT GREEN

	if (exp <= 30)
		app->render->DrawRectangle({ 357,392,(int)experience, 9 }, 123, 77, 52, 255);//ORANGE

	if (exp <= 20)
		app->render->DrawRectangle({ 357,392,(int)experience, 9 }, 102, 60, 49, 255);//DARK ORANGE

	if (exp <= 10)
		app->render->DrawRectangle({ 357,392,(int)experience, 9 }, 102, 49, 49, 255);//DARK RED


	//Draw HP NUMBER
	std::string text = ToUpperCase(to_string((int)hp));
	app->fonts->BlitText(297, 297, 0, text.c_str());
	//Draw EXP
	text = ToUpperCase(to_string((int)exp));
	app->fonts->BlitText(297, 322, 0, text.c_str());
	//Draw Money
	text = ToUpperCase(to_string(app->party->allyParty->FindByName("Thyma")->data.GetMoney()));
	app->fonts->BlitText(582, 233, 0, text.c_str());

}

