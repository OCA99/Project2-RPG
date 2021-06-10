#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#include "Module.h"
#include "App.h"
#include "Textures.h"
#include "GuiManager.h"

#include "External/PugiXml/src/pugixml.hpp"
#include "SString.h"
#include "List.h"

#include <string>

#define MAX_ITEMS 8

using namespace std;

class Font;
enum class ItemType {
	NONE,
	HELMET,
	CHESTPLATE,
	LEGGINGS,
	BOOTS,
	ACCESORY,
	WEAPON,
};
class Item
{
public:
	Item() {};
	~Item() {};
public:

	int quantity; // Amount of items in possession
	float armor;
	SString title; // Item title
	SString displayTitle; // Displayed Title
	SString description; // Item description
	SString texturePath; //Texture Path
	SDL_Texture** itemTex = nullptr;
	ItemType itemType = ItemType::NONE;
};

class ItemManager : public Module
{
public:
	ItemManager();

	~ItemManager();

	bool Start();

	bool Update(float dt);

	bool PostUpdate(float dt);

	bool CleanUp();

	//Draw Items Icon & Title
	void DrawItems();

	//Give item to a player, USE TITLE AS PARAMETER
	void GiveItemToPlayer(SString& itemTitle, int quantity = 1);

	//Search for an item BY ITS TITLE(NAME)
	Item* SearchForItem(SString& itemTitle); 

	//Draw the box with te Description Text
	void ShowDescription(); 

	//Show Item Actions
	void CreateActionButtons(int y);

	//Create Description & Exit Buttons
	void CreateButtons();

	void DeleteButtons();

	void DeleteActionButtons();

	void UseItem(Item* itemToUse, int y);

	void RemoveItem(Item* itemToRemove, int y);

	void EquipArmor(Item* armourItem);

	void CheckActionButtons();

	void DrawPlayerStats();

	void DrawArmor();


public:
	SDL_Texture** itemDescTex;

	List<Item*> itemList;//ALL Items
	List<Item*> playerItemList;//Player Items
	List<Item*> thymaArmor;//Armor list
	List<Item*> toistoArmor;//Armor list
	List<GuiControl*> buttons;//Buttons List
	List<GuiControl*> actionButtons;//Buttons List

	int y = 0;//Logistic Tool

	bool invOpened = false;//True = Opened, False = closed.
	bool createButtons = true;
	bool partyMember = false; //false == toisto ;; true == thyma

	bool useItem = false;
	bool removeItem = false;
	bool yPressed = true;

	bool hemosAbiertoElInvenLoko = true;

	ItemType currentAmor = ItemType::NONE;


};


#endif
