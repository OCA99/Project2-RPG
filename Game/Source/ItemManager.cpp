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
		item->armor = itemNode.attribute("armor").as_float();
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

	GiveItemToPlayer(SString("HP Potion"));
	GiveItemToPlayer(SString("EXP Potion"));
	GiveItemToPlayer(SString("EXP Potion"));
	GiveItemToPlayer(SString("EXP Potion"));
	GiveItemToPlayer(SString("EXP Potion"));
	GiveItemToPlayer(SString("EXP Potion"));
	GiveItemToPlayer(SString("EXP Potion"));
	GiveItemToPlayer(SString("EXP Potion"));
	//GiveItemToPlayer(SString("HP Potion"));
	//GiveItemToPlayer(SString("Leather Helmet"));
	//GiveItemToPlayer(SString("Leather Chestplate"));
	//GiveItemToPlayer(SString("Leather Leggings"));
	//GiveItemToPlayer(SString("Leather Boots"));
	//GiveItemToPlayer(SString("Iron Helmet"));
	//GiveItemToPlayer(SString("Iron Chestplate"));
	//GiveItemToPlayer(SString("Magic Pendant"));
	//GiveItemToPlayer(SString("Iron Sword"));

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
		if (!invOpened)
		{
			app->scene->ResetInitialPositions();
			app->scene->sCreated = false;
		}
	}
	if ((app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || (app->input->pads[0].y && yPressed)) && app->scene->currentScene->type == Scene::TYPE::MAP)
	{
		app->party->allyParty->FindByName("Thyma")->data.Addhealth(1.0f);
	}

	if (app->input->pads[0].b == true && invOpened)
	{
		invOpened = false;
		app->scene->ResetInitialPositions();
		app->scene->sCreated = false;
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
		DrawArmor();//Draw the Armor of the player
		ShowDescription();//Show
		DrawPlayerStats();//Draw player Stats
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
			app->render->DrawTexture(item->data->itemTex, 45, (-*app->scene->pos) + 75 + 32 * y, (SDL_Rect*)(0, 0, 0, 0), 1.0f, 1, 0, 0, 0, false);

			//DRAW TEXT
			std::string text = ToUpperCase(item->data->displayTitle.GetString());
			app->fonts->BlitText(75, 75 + (32 * y) + (-*app->scene->pos), 1, text.c_str());

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
				app->render->DrawTexture(itemDescTex, item->data->bounds.x + 32, (-*app->scene->pos) + item->data->bounds.y + 30, &SDL_Rect({ 0,0,384,96 }), 0.5f, 0, 0, 0, 0, false);

				//Draw Text
				std::string text = ToUpperCase(playerItemList[b]->description.GetString());
				app->fonts->BlitText(item->data->bounds.x + 37, item->data->bounds.y + 35 + (-*app->scene->pos), 1, text.c_str());
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
		actionButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 205 , 55 + 32 * y , 100,20 }), 19, true, 55 + 32 * y, 600, SplineType::BACK, false)); //USE ITEM BUTTON
		actionButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 205 , 55 + 32 * y , 100, 20 }), 20, true, 95 + 32 * y, 600, SplineType::BACK, false)); //DISCARD ITEM BUTTON
		actionButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 205 , 55 + 32 * y , 100, 20 }), 23, true, 75 + 32 * y, 600, SplineType::BACK, false)); //REMOVE ITEM BUTTON


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
			buttons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 35 , -1000, 340 / 2, 70 / 2 }), 17, true, 65 + 32 * y, 600, SplineType::BACK, false)); //BUTTON TO SHOW ITEM DESCRIPTION WITH THE MOUSE
			y++;
			item = item->next;
		}
		buttons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 30 , -1000, 30, 30 }), 14, true, 36 / 2, 600, SplineType::BACK, false));//CREATE EXIT BUTTON
		buttons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 578, -1000, 30, 30 }), 22, true, 18, 600, SplineType::BACK, false));//CREATE PARTY BUTTON

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
	int a = 0;
	while (item)
	{
		if (item->data->itemType == armourItem->itemType)
		{
			GiveItemToPlayer(item->data->title);
			if (!partyMember)
			{
				ListItem<Item*>* c = thymaArmor.At(a);
				thymaArmor.Del(c);
				app->party->allyParty->FindByName("Thyma")->data.AddArmor(-item->data->armor);
				
			}
			if (partyMember)
			{
				ListItem<Item*>* c = toistoArmor.At(a);
				toistoArmor.Del(item);
				app->party->allyParty->FindByName("Toisto")->data.AddArmor(-item->data->armor);

			}
		}
		a++;
		item = item->next;

	}
	
	float armor = armourItem->armor;
	if (!partyMember)
	{
		thymaArmor.Add(armourItem);
		if(armourItem->itemType != ItemType::WEAPON)
			app->party->allyParty->FindByName("Thyma")->data.AddArmor(armor);
		else
			app->party->allyParty->FindByName("Thyma")->data.AddPower(armor);//Power with Armour name

	}
	if (partyMember) 
	{
		toistoArmor.Add(armourItem);
		if (armourItem->itemType != ItemType::WEAPON)
			app->party->allyParty->FindByName("Toisto")->data.AddArmor(armor);
		else
			app->party->allyParty->FindByName("Toisto")->data.AddPower(armor);

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
	float maxHp = 0;
	float pixelHp = 0;
	float resultHp = 0;

	float exp = 0;
	float resultExp = 0;
	int maxExp = 0;
	float pixelExp = 0;


	float attackPower = 0;
	int money = 0;
	float armor = 0;
	int level = 0;

	if (partyMember)
	{
		hp = app->party->allyParty->FindByName("Toisto")->data.GetHealth();
		exp = app->party->allyParty->FindByName("Toisto")->data.GetExp();

		pixelHp = (app->party->allyParty->FindByName("Toisto")->data.GetHealth() * 288) / 100;
		pixelExp = (app->party->allyParty->FindByName("Toisto")->data.GetExp() * 288) / 100;
		attackPower = (app->party->allyParty->FindByName("Toisto")->data.GetPower());
		money = app->party->allyParty->FindByName("Toisto")->data.GetMoney();
		armor = app->party->allyParty->FindByName("Toisto")->data.GetArmor();
		level = app->party->allyParty->FindByName("Toisto")->data.GetLevel();
		
		
	}

	if (!partyMember)
	{

		maxExp = app->party->allyParty->FindByName("Thyma")->data.GetMaxExp();
		exp = app->party->allyParty->FindByName("Thyma")->data.GetExp();
		resultExp = (100 * exp) / maxExp;
		pixelExp = resultExp * 288/ 100;



		maxHp = app->party->allyParty->FindByName("Thyma")->data.GetMaxHealth();
		maxHp = 100;
		hp = app->party->allyParty->FindByName("Thyma")->data.GetHealth();
		hp = 92;
		resultHp = (100 * hp) / maxHp;
		pixelHp = resultHp * 288 / 100;



		attackPower = (app->party->allyParty->FindByName("Thyma")->data.GetPower());

		money = app->party->allyParty->FindByName("Thyma")->data.GetMoney();
		armor = app->party->allyParty->FindByName("Thyma")->data.GetArmor();
		level = app->party->allyParty->FindByName("Thyma")->data.GetLevel();

	}

	int barPosX = 714;
	int barPosY = 595;

	app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),288, 18 }, 147, 147, 147, 255, true, false);//BASE COLOR
	app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),288, 18 }, 147, 147, 147, 255, true, false);//BASE COLOR

	if (resultHp > 85)
		app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),(int)pixelHp, 18 }, 50, 85, 95, 255, true, false);//BLUE

	if (resultHp <= 85)
		app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),(int)pixelHp, 18 }, 50, 89, 83, 255, true, false);// LIGHT BLUE

	if (resultHp <= 75)
		app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),(int)pixelHp, 18 }, 51, 81, 48, 255, true, false);//DARK GREEN

	if (resultHp <= 67)
		app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),(int)pixelHp, 18 }, 71, 89, 50, 255, true, false);//GREEN

	if (resultHp <= 60)
		app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),(int)pixelHp, 18 }, 100, 106, 51, 255, true, false);//LIGHT GREEN

	if (resultHp <= 50)
		app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),(int)pixelHp, 18 }, 123, 100, 51, 255, true, false);//YELLOW

	if (resultHp <= 40)
		app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),(int)pixelHp, 18 }, 123, 90, 52, 255, true, false);//LIGHT GREEN

	if (resultHp <= 30)
		app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),(int)pixelHp, 18 }, 123, 77, 52, 255, true, false);//ORANGE

	if (resultHp <= 20)
		app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),(int)pixelHp, 18 }, 102, 60, 49, 255, true, false);//DARK ORANGE

	if (resultHp <= 10)
		app->render->DrawRectangle({ 713,595 + (-*app->scene->pos),(int)pixelHp, 18 }, 102, 49, 49, 255, true, false);//DARK RED


	if (resultExp > 85)
		app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),(int)pixelExp, 18 }, 50, 85, 95, 255, true, false);//BLUE

	if (resultExp <= 85)
		app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),(int)pixelExp, 18 }, 50, 89, 83, 255, true, false);// LIGHT BLUE

	if (resultExp <= 75)
		app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),(int)pixelExp, 18 }, 51, 81, 48, 255, true, false);//DARK GREEN

	if (resultExp <= 67)
		app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),(int)pixelExp, 18 }, 71, 89, 50, 255, true, false);//GREEN

	if (resultExp <= 60)
		app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),(int)pixelExp, 18 }, 100, 106, 51, 255, true, false);//LIGHT GREEN

	if (resultExp <= 50)
		app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),(int)pixelExp, 18 }, 123, 100, 51, 255, true, false);//YELLOW

	if (resultExp <= 40)
		app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),(int)pixelExp, 18 }, 123, 90, 52, 255, true, false);//LIGHT GREEN

	if (resultExp <= 30)
		app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),(int)pixelExp, 18 }, 123, 77, 52, 255, true, false);//ORANGE

	if (resultExp <= 20)
		app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),(int)pixelExp, 18 }, 102, 60, 49, 255, true, false);//DARK ORANGE

	if (resultExp <= 10)
		app->render->DrawRectangle({ 713, 645 + (-*app->scene->pos),(int)pixelExp, 18 }, 102, 49, 49, 255, true, false);//DARK RED


	//Draw HP NUMBER
	std::string text = ToUpperCase(to_string((int)hp));
	app->fonts->BlitText(297, 297 + (-*app->scene->pos), 0, text.c_str());
	//Draw EXP
	text = ToUpperCase(to_string((int)exp));
	app->fonts->BlitText(297, 322 + (-*app->scene->pos), 0, text.c_str());
	//Draw Money
	text = ToUpperCase(to_string(money));
	app->fonts->BlitText(582, 233 + (-*app->scene->pos), 0, text.c_str());
	//Draw Attack Power
	text = ToUpperCase(to_string((int)attackPower));
	app->fonts->BlitText(560, 310 + (-*app->scene->pos), 0, text.c_str());
	//Draw Armor
	text = ToUpperCase(to_string((int)armor));
	app->fonts->BlitText(560, 325 + (-*app->scene->pos), 0, text.c_str());
	//Draw Level
	text = ToUpperCase(to_string((int)level));
	app->fonts->BlitText(250, 310 + (-*app->scene->pos), 0, text.c_str());
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
			app->render->DrawTexture(item->data->itemTex, 290, 60 + (-*app->scene->pos), (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		case ItemType::CHESTPLATE:
			app->render->DrawTexture(item->data->itemTex, 290, 110 + (-*app->scene->pos), (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		case ItemType::LEGGINGS:
			app->render->DrawTexture(item->data->itemTex, 290, 157 + (-*app->scene->pos), (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		case ItemType::BOOTS:
			app->render->DrawTexture(item->data->itemTex, 290, 207 + (-*app->scene->pos), (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		case ItemType::ACCESORY:
			app->render->DrawTexture(item->data->itemTex, 505, 180 + (-*app->scene->pos), (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		case ItemType::WEAPON:
			app->render->DrawTexture(item->data->itemTex, 505, 85 + (-*app->scene->pos), (SDL_Rect*)(0, 0, 0, 0), 1.5f, 1, 0, 0, 0, false);

			break;
		default:
			break;
		}

		item = item->next;

	}

}

