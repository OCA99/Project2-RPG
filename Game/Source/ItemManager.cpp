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

		item->title = itemNode.attribute("title").as_string();
		item->displayTitle = itemNode.attribute("displayTitle").as_string();
		item->description = itemNode.attribute("description").as_string();
		item->quantity = itemNode.attribute("quantity").as_int();
		item->texturePath = itemNode.attribute("texturePath").as_string();
		item->itemTex = app->tex->Load(SString("Textures/Items/%s", item->texturePath.GetString()).GetString());
		SString tmp = itemNode.attribute("armorType").as_string();
		if (tmp == SString("chestplate"))
			item->itemType = ItemType::CHESTPLATE;
		if (tmp == SString("helmet"))
			item->itemType = ItemType::HELMET;
		if (tmp == SString("leggings"))
			item->itemType = ItemType::LEGGINGS;
		if (tmp == SString("boots"))
			item->itemType = ItemType::BOOTS;
		if (tmp == SString("accesory"))
			item->itemType = ItemType::ACCESORY;
		if (tmp == SString("weapon"))
			item->itemType = ItemType::WEAPON;

		itemList.Add(item);
		itemNode = itemNode.next_sibling("item");
	}

	GiveItemToPlayer(SString("EXP Potion"));
	GiveItemToPlayer(SString("HP Potion"));
	GiveItemToPlayer(SString("Leather Helmet"));
	GiveItemToPlayer(SString("Leather Chestplate"));
	GiveItemToPlayer(SString("Leather Leggings"));
	GiveItemToPlayer(SString("Leather Boots"));
	GiveItemToPlayer(SString("Iron Sword"));
	GiveItemToPlayer(SString("Magic Pendant"));

	itemDescTex = app->tex->Load("Textures/UI/OptionsMenu/item_description.png");

	return true;
}

bool ItemManager::Update(float dt)
{

	if (!app->input->pads[0].y) yPressed = true;

	if ((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || (app->input->pads[0].y && yPressed)) && app->scene->currentScene->type == Scene::TYPE::MAP && !app->scene->menu && !app->quests->questInvOpened && app->dialog->currentDialog == nullptr)
	{
		yPressed = false;
		invOpened = !invOpened;//Open or close Inv
	}


	if (invOpened)
	{
		if (createButtons)
		{
			CreateButtons();
			createButtons = false;
		}

	}

	if (!invOpened) createButtons = true;

	return true;
}

bool ItemManager::PostUpdate(float dt)
{

	if (invOpened)//If the inventory is opened bool
	{
		DrawItems();//Draw Items Image and Title
		ShowDescription();//Show
		DrawPlayerStats();
		DrawArmor();
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

void ItemManager::DrawItems()
{
	ListItem<Item*>* item = playerItemList.start;
	y = 0;
	while (item)
	{
		if (item->data->itemTex != nullptr)
		{
			//Draw Texture
			app->render->DrawTexture(item->data->itemTex, 45, 75 + 32 * y, (SDL_Rect*)(0, 0, 0, 0), 1.0f, 1, 0, 0, 0, false);

			//DRAW TEXT
			std::string text = ToUpperCase(item->data->displayTitle.GetString());
			app->fonts->BlitText(75, 75 + (32 * y), 1, text.c_str());

		}

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
		//else if (playerItemList.Add(SearchForItem(itemTitle)) == nullptr)
		//	LOG("There is no Reward for this Quest");
		else { playerItemList.Add(SearchForItem(itemTitle)); }

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
		actionButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 205 , 55 + 32 * y , 100,20 }), 19)); //USE ITEM BUTTON
		actionButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 205 , 95 + 32 * y , 100, 20 }), 20)); //DISCARD ITEM BUTTON
		actionButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 205 , 75 + 32 * y , 100, 20 }), 23)); //REMOVE ITEM BUTTON


	}
	CheckActionButtons();

}

void ItemManager::CreateButtons()
{
	if (buttons.Count() <= 0)
	{
		ListItem<Item*>* item = playerItemList.start;
		y = 0;
		while (item)
		{
			buttons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 35 , 65 + 32 * y, 340 / 2, 70 / 2 }), 17)); //BUTTON TO SHOW ITEM DESCRIPTION WITH THE MOUSE
			y++;
			item = item->next;
		}
		buttons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 30 , 15, 30, 30 }), 14));//CREATE EXIT BUTTON
		buttons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 578, 18, 30, 30 }), 22));//CREATE PARTY BUTTON

	}

}

void ItemManager::DeleteButtons()
{

	ListItem<GuiControl*>* item = buttons.start;
	while (item)
	{

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
		if (item->data->id == 19 || item->data->id == 20 || item->data->id == 23)
			app->ui->DestroyGuiControl(item->data);

		CreateButtons();
		item = item->next;
	}
	actionButtons.Clear();
}

void ItemManager::UseItem(Item* itemtoUse, int y)
{
	//EFECTO DE CADA ITEM
	if (itemtoUse->title == SString("HP Potion"))
	{
		if (partyMember) app->party->allyParty->FindByName("Toisto")->data.Addhealth(15.f);
		if (!partyMember) app->party->allyParty->FindByName("Thyma")->data.Addhealth(15.f);
	}
	if (itemtoUse->title == SString("EXP Potion"))
	{
		if (partyMember) app->party->allyParty->FindByName("Toisto")->data.AddExp(17.f);
		if (!partyMember) app->party->allyParty->FindByName("Thyma")->data.AddExp(17.f);
	}
	if (itemtoUse->title == SString("Coin"))
	{
		if (partyMember) app->party->allyParty->FindByName("Toisto")->data.AddMoney(1);
		if (!partyMember) app->party->allyParty->FindByName("Thyma")->data.AddMoney(1);
	}
	if (itemtoUse->title == SString("Coin Stack"))
	{
		if (partyMember) app->party->allyParty->FindByName("Toisto")->data.AddMoney(50.f);
		if (!partyMember) app->party->allyParty->FindByName("Thyma")->data.AddMoney(50.f);
	}
	if (itemtoUse->title == SString("Treasure Chest"))
	{
		if (partyMember) app->party->allyParty->FindByName("Toisto")->data.AddMoney(150.f);
		if (!partyMember) app->party->allyParty->FindByName("Thyma")->data.AddMoney(150.f);
	}
	if (itemtoUse->itemType == ItemType::HELMET || itemtoUse->itemType == ItemType::CHESTPLATE || itemtoUse->itemType == ItemType::LEGGINGS || 
		itemtoUse->itemType == ItemType::BOOTS || itemtoUse->itemType == ItemType::ACCESORY || itemtoUse->itemType == ItemType::WEAPON)
	{
		if (partyMember) EquipArmor(itemtoUse);
		if (!partyMember) EquipArmor(itemtoUse);
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

void ItemManager::RemoveItem(Item* itemToRemove, int y)
{
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

void ItemManager::EquipArmor(Item* armourItem)
{

	ListItem<Item*>* item = nullptr;
	if (!partyMember) item = thymaArmor.start;
	if (partyMember) item = toistoArmor.start;
	while (item)
	{
		if (item->data->itemType == armourItem->itemType)
		{
			GiveItemToPlayer(item->data->title);
		}

		item = item->next;

	}

	if (!partyMember) thymaArmor.Add(armourItem);
	if (partyMember) toistoArmor.Add(armourItem);
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

		if (actionButtons.Count() > 0 && actionButtons[0]->itemUsed && item->data->itemSingleCheck)
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

		if (actionButtons.Count() > 0 && actionButtons[2]->removeItem && item->data->itemSingleCheck)
		{
			item->data->itemSingleCheck = false;
			RemoveItem(playerItemList[y], y);
			DeleteButtons();
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
	float hp = 0;
	float exp = 0;
	float health = 0;
	float experience = 0;
	if (partyMember)
	{
		hp = app->party->allyParty->FindByName("Toisto")->data.GetHealth();
		exp = app->party->allyParty->FindByName("Toisto")->data.GetExp();

		health = (app->party->allyParty->FindByName("Toisto")->data.GetHealth() * 288) / 100;
		experience = (app->party->allyParty->FindByName("Toisto")->data.GetExp() * 288) / 100;
	}

	if (!partyMember)
	{
		hp = app->party->allyParty->FindByName("Thyma")->data.GetHealth();
		exp = app->party->allyParty->FindByName("Thyma")->data.GetExp();

		health = (app->party->allyParty->FindByName("Thyma")->data.GetHealth() * 288) / 100;
		experience = (app->party->allyParty->FindByName("Thyma")->data.GetExp() * 288) / 100;

	}

	int barPosX = 714;
	int barPosY = 595;

	app->render->DrawRectangle({ 713,595,288, 18 }, 147, 147, 147, 255, true, false);//BASE COLOR
	app->render->DrawRectangle({ 713, 645,288, 18 }, 147, 147, 147, 255, true, false);//BASE COLOR

	if (hp > 85)
		app->render->DrawRectangle({ 713,595,(int)health, 18 }, 50, 85, 95, 255, false, false);//BLUE

	if (hp <= 85)
		app->render->DrawRectangle({ 713,595,(int)health, 18 }, 50, 89, 83, 255, true, false);// LIGHT BLUE

	if (hp <= 75)
		app->render->DrawRectangle({ 713,595,(int)health, 18 }, 51, 81, 48, 255, true, false);//DARK GREEN

	if (hp <= 67)
		app->render->DrawRectangle({ 713,595,(int)health, 18 }, 71, 89, 50, 255, true, false);//GREEN

	if (hp <= 60)
		app->render->DrawRectangle({ 713,595,(int)health, 18 }, 100, 106, 51, 255, true, false);//LIGHT GREEN

	if (hp <= 50)
		app->render->DrawRectangle({ 713,595,(int)health, 18 }, 123, 100, 51, 255, true, false);//YELLOW

	if (hp <= 40)
		app->render->DrawRectangle({ 713,595,(int)health, 18 }, 123, 90, 52, 255, true, false);//LIGHT GREEN

	if (hp <= 30)
		app->render->DrawRectangle({ 713,595,(int)health, 18 }, 123, 77, 52, 255, true, false);//ORANGE

	if (hp <= 20)
		app->render->DrawRectangle({ 713,595,(int)health, 18 }, 102, 60, 49, 255, true, false);//DARK ORANGE

	if (hp <= 10)
		app->render->DrawRectangle({ 713,595,(int)health, 18 }, 102, 49, 49, 255, true, false);//DARK RED


	if (exp > 85)
		app->render->DrawRectangle({ 713, 645,(int)experience, 18 }, 50, 85, 95, 255, true, false);//BLUE

	if (exp <= 85)
		app->render->DrawRectangle({ 713, 645,(int)experience, 18 }, 50, 89, 83, 255, true, false);// LIGHT BLUE

	if (exp <= 75)
		app->render->DrawRectangle({ 713, 645,(int)experience, 18 }, 51, 81, 48, 255, true, false);//DARK GREEN

	if (exp <= 67)
		app->render->DrawRectangle({ 713, 645,(int)experience, 18 }, 71, 89, 50, 255, true, false);//GREEN

	if (exp <= 60)
		app->render->DrawRectangle({ 713, 645,(int)experience, 18 }, 100, 106, 51, 255, true, false);//LIGHT GREEN

	if (exp <= 50)
		app->render->DrawRectangle({ 713, 645,(int)experience, 18 }, 123, 100, 51, 255, true, false);//YELLOW

	if (exp <= 40)
		app->render->DrawRectangle({ 713, 645,(int)experience, 18 }, 123, 90, 52, 255, true, false);//LIGHT GREEN

	if (exp <= 30)
		app->render->DrawRectangle({ 713, 645,(int)experience, 18 }, 123, 77, 52, 255, true, false);//ORANGE

	if (exp <= 20)
		app->render->DrawRectangle({ 713, 645,(int)experience, 18 }, 102, 60, 49, 255, true, false);//DARK ORANGE

	if (exp <= 10)
		app->render->DrawRectangle({ 713, 645,(int)experience, 18 }, 102, 49, 49, 255, true, false);//DARK RED


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

void ItemManager::DrawArmor()
{

	ListItem<Item*>* item = nullptr;
	if (!partyMember) item = thymaArmor.start;
	if (partyMember) item = toistoArmor.start;
	while (item)
	{
		switch (item->data->itemType)
		{
		case ItemType::NONE:
			break;
		case ItemType::HELMET:
			app->render->DrawTexture(item->data->itemTex, 290, 60, (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		case ItemType::CHESTPLATE:
			app->render->DrawTexture(item->data->itemTex, 290, 110, (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		case ItemType::LEGGINGS:
			app->render->DrawTexture(item->data->itemTex, 290, 157, (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		case ItemType::BOOTS:
			app->render->DrawTexture(item->data->itemTex, 290, 207, (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		case ItemType::ACCESORY:
			app->render->DrawTexture(item->data->itemTex, 505, 180, (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		case ItemType::WEAPON:
			app->render->DrawTexture(item->data->itemTex, 505, 85, (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		default:
			break;
		}

		item = item->next;

	}

}

