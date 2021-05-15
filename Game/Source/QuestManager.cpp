#include "QuestManager.h"
#include "Log.h"
#include "App.h"
#include "Fonts.h"
#include "Module.h"
#include "List.h"
#include "Input.h"
#include "Render.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ItemManager.h"

#include "External/PugiXml/src/pugixml.hpp"
#include "SDL/include/SDL_scancode.h"
#include "ToUpperCase.h"

#include <string>
#include <iostream>
using namespace std;

QuestManager::QuestManager() : Module()
{
	name.Create("quests");
}

QuestManager::~QuestManager()
{
}

bool QuestManager::Start()
{
	//font = new Font("Fonts/dungeon_font3.xml", app->tex);

	// ToDo 2: Load the xml file, parse the result, and make a node pointing to quests parent
	///////////////////////////////////////////////////////////////////////////
	pugi::xml_node questNode;
	pugi::xml_document questData;
	pugi::xml_parse_result parseResult = questData.load_file("quests.xml");
	if (parseResult == NULL)
		LOG("Could not load xml file <quests.xml> pugi error: %s", parseResult.description());
	else
		questNode = questData.child("quests");
	if (questNode == NULL)
		LOG("Could not load <questsData> xml_document");
	///////////////////////////////////////////////////////////////////////////

	questNode = questNode.child("quest");
	while (questNode != NULL)
	{
		Quest* quest = new Quest(questNode);

		/*quest->id = questNode.attribute("id").as_int();
		quest->type = questNode.attribute("type").as_int();
		quest->title = questNode.attribute("title").as_string();
		quest->description = questNode.attribute("description").as_string();
		quest->objective = questNode.attribute("objective").as_string();
		quest->progress = questNode.attribute("progress").as_int();
		quest->quantity = questNode.attribute("quantity").as_int();
		quest->demandingNPC = questNode.attribute("demandingNPC").as_string();
		quest->rewardingNPC = questNode.attribute("rewardingNPC").as_string();
		quest->reward = questNode.attribute("reward").as_string();
		quest->rewardQuantity = questNode.attribute("rewardQuantity").as_string();

		quest->rewardXP = questNode.attribute("rewardXP").as_int();
		quest->rewardGold = questNode.attribute("rewardGold").as_int();
		quest->requiredId = questNode.attribute("requiredId").as_int();
		quest->isCompleted = questNode.attribute("isCompleted").as_bool();
		quest->status = questNode.attribute("status").as_int();*/

		if (quest->demandingNPC != nullptr)
		{


		}

		if (quest->status == 0) // 0 inactive but loaded
		{
			questsInactive.Add(quest);
		}
		else if (quest->status == 1) // 1 active and loaded
		{
			questsActive.Add(quest);
		}
		else if (quest->status == 2) // 2 completed
		{
			questsFinished.Add(quest);
		}

		questsList.Add(quest);
		///////////////////////////////////////////////////////////////////////////

		questNode = questNode.next_sibling("quest");
	}

	customerTex = app->tex->Load("Textures/Dialogue/blacksmith_dialogue.png");
	reaperTex = app->tex->Load("Textures/Dialogue/reaper_dialogue.png");
	tavernTex = app->tex->Load("Textures/Dialogue/tavern_lady_dialogue.png");
	thymaTex = app->tex->Load("Textures/Dialogue/thyma_good_dialogue.png");


	return true;
}

bool QuestManager::Update(float dt)
{

	CheckQuestsLogic();
	CheckObjectivesCompletion();

	if ((app->scene->currentScene->type == Scene::TYPE::MAP && app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN))
		questInvOpened = !questInvOpened;//Open or close Inv

	if (questInvOpened)
		CreateQuestButtons();

	return true;
}

bool QuestManager::PostUpdate(float dt)
{

	DrawActiveQuests();

	if (questInvOpened)
	{
		if(!app->scene->buttons) CreateQuestButtons();
		DrawQuestUi();
		ShowQuestDescription();
	}
	if (!questInvOpened)//If it close, we delete the buttons
	{
		ListItem<GuiControl*>* item = questButtons.start;
		while (item)
		{

			if (item->data->id == 18)
				app->ui->DestroyGuiControl(item->data);

			item = item->next;
		}
		questButtons.Clear();
	}

	return true;
}

bool QuestManager::CleanUp()
{
	questsList.Clear();
	questsInactive.Clear();
	questsActive.Clear();
	questsFinished.Clear();

	return true;
}

// ToDo 3: Take a look at how quests are drawn into screen. Make the description of each quest be shown if user 
// presses L button (I am giving you each x, y, size, spacing and tint in order to prevent you to waste time)
// case 1: 300, 70, 45, 0, { 200,200,200,155 }	case 2: 200, 70, 45, 0, { 200,200,200,155 }
// case 3: 245, 110, 45, 0, { 200,200,200,155 } case 4: 0, 140, 45, 0, { 200,200,200,155 }
// case 5: 0, 100, 45, 0, { 200,200,200,155 }	case 6: 0, 100, 45, 0, { 200,200,200,155 }
///////////////////////////////////////////////////////////////////////////
bool QuestManager::DrawActiveQuests()
{
	std::string text;
	string numToStr;
	const char* numToStr2;
	ListItem<Quest*>* L = questsActive.start;
	while (L != NULL)
	{
		switch (L->data->id)
		{
		case 1: // new quest chain 1
			// Title Drawing
			text = ToUpperCase(L->data->title.GetString());
			app->fonts->BlitText(0, 0, 1, text.c_str());

			// Amount of mushrooms taken
			numToStr = to_string(L->data->progress);
			app->fonts->BlitText(50, 0, 1, numToStr.c_str());

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				//app->render->DrawText(font, L->data->description.GetString(), 300, 70, 45, 0, { 200,200,200,155 });
			///////////////////////////////////////////////////////////////////////////

				break;
		case 2: // quest chain 1 (quest 2)
			// Title Drawing
			//app->render->DrawText(font, L->data->title.GetString(), 0, 60, 60, 0, { 255,255,255,255 });

			// Amount of trees taken
			//numToStr = to_string(app->player->chopTreeCount);
			numToStr2 = numToStr.c_str();
			//app->render->DrawText(font, numToStr2, 180, 63, 60, 0, { 255,255,255,200 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				//app->render->DrawText(font, L->data->description.GetString(), 200, 70, 45, 0, { 200,200,200,155 });
			///////////////////////////////////////////////////////////////////////////

				break;

		case 3: // new quest chain 2
			// Title Drawing
			//app->render->DrawText(font, L->data->title.GetString(), 0, 100, 60, 0, { 255,255,255,255 });

			// Amount of rubbish taken
			//numToStr = to_string(app->player->beachRubbish);
			numToStr2 = numToStr.c_str();
			//app->render->DrawText(font, numToStr2, 225, 103, 60, 0, { 255,255,255,200 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				//app->render->DrawText(font, L->data->description.GetString(), 245, 110, 45, 0, { 200,200,200,155 });
			///////////////////////////////////////////////////////////////////////////

				break;
		case 4: // quest chain 2 (quest 2)
			// Title Drawing
			//app->render->DrawText(font, L->data->title.GetString(), 0, 100, 60, 0, { 255,255,255,255 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				//app->render->DrawText(font, L->data->description.GetString(), 0, 140, 45, 0, { 200,200,200,155 });
			///////////////////////////////////////////////////////////////////////////

				break;
		case 5:
			// Title Drawing
			//app->render->DrawText(font, L->data->title.GetString(), 0, 60, 60, 0, { 255,255,255,255 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				//app->render->DrawText(font, L->data->description.GetString(), 0, 100, 45, 0, { 200,200,200,155 });
			///////////////////////////////////////////////////////////////////////////

				break;
		case 6:
			// Title Drawing
			//app->render->DrawText(font, L->data->title.GetString(), 0, 60, 60, 0, { 255,255,255,255 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				//app->render->DrawText(font, L->data->description.GetString(), 0, 100, 45, 0, { 200,200,200,155 });
			///////////////////////////////////////////////////////////////////////////

				break;
		default:
			break;
		}

		L = L->next;
	}
	return true;
}
void QuestManager::DrawQuestUi()
{
	ListItem<Quest*>* item = questsActive.start;

	int i = 0;
	while (item)
	{
		std::string text = ToUpperCase(item->data->title.GetString());
		app->fonts->BlitText(40, 80 + (32 * i), 0, text.c_str());
		item = item->next;
		++i;
	}

}

void QuestManager::ShowQuestDescription()
{
	ListItem<GuiControl*>* item = questButtons.start;
	y = 0;
	while (item)
	{
		//Draw Texture
		if (y > questButtons.Count()) y = 0;
		if (item->data->questCheck)
		{
			if (y < questsActive.Count())
			{
				//DRAW QUEST DESCRIPTION.
				std::string text = ToUpperCase(questsActive[y]->description.GetString());
				app->fonts->BlitText(235, 100, 0, text.c_str());

				//DRAW QEST OBJECTIVE
				text = ToUpperCase(questsActive[y]->title.GetString());
				app->fonts->BlitText(235, 300, 0, text.c_str());
				text = ToUpperCase(to_string(questsActive[y]->quantity));
				app->fonts->BlitText(450, 300, 0, text.c_str());

				//DRAW NPC TEXTURE
				if (questsActive[y]->demandingNPC == SString("customer")) app->render->DrawTexture(customerTex, 10, 15, &SDL_Rect({ 0,0,1280,720 }), 0.5f, 1, 0, 0, 0, false);
				if (questsActive[y]->demandingNPC == SString("reaper")) app->render->DrawTexture(reaperTex, 10, 18, &SDL_Rect({ 0,0,1280,720 }), 0.5f, 1, 0, 0, 0, false);
				if (questsActive[y]->demandingNPC == SString("tgirl")) app->render->DrawTexture(tavernTex, 10, 16, &SDL_Rect({ 0,0,1280,720 }), 0.5f, 1, 0, 0, 0, false);
				if (questsActive[y]->demandingNPC == SString("thyma")) app->render->DrawTexture(thymaTex, 10, 16, &SDL_Rect({ 0,0,1280,720 }), 0.5f, 1, 0, 0, 0, false);

				//DRAW REWARDS
				if (Item* tempItem = app->items->SearchForItem(questsActive[y]->reward))//If there is not a reward it will not crash with this if
				{
					app->render->DrawTexture(tempItem->itemTex, 300, 190, &SDL_Rect({ 0,0,16,16 }), 1.5f, 1, 0, 0, 0, false);

				}
	
				//Draw Reward Quantity
				text = ToUpperCase(questsActive[y]->rewardQuantity.GetString());
				app->fonts->BlitText(330, 200, 0, text.c_str());
			}

		}
		++y;
		item = item->next;
	}

}

void QuestManager::CreateQuestButtons()
{
	if (questButtons.Count() <= 0)
	{
		ListItem<Quest*>* item = questsActive.start;
		y = 0;
		while (item)
		{
			questButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 35 , 65 + 32 * y, 340 / 2, 70 / 2 }), 18)); //BUTTON TO SHOW ITEM DESCRIPTION WITH THE MOUSE
			y++;
			item = item->next;
		}
	}

}

bool QuestManager::CheckQuestsLogic()
{
	// For changing from one list to another (has finished quest)
	// ToDo 5: Implement code that passes a completed quest from the active list to the finished one
	// (remember to give rewards --> look at 2 functions of ModulePlayer)
	/////////////////////////////////////////////////////////////////////////////
	ListItem<Quest*>* activeQuestsList = questsActive.start;
	ListItem<Quest*>* inactiveQuestsList = questsInactive.start;

	while (activeQuestsList != nullptr)
	{
		if (activeQuestsList->data->isCompleted == true)
		{
			activeQuestsList->data->status = 2;
			questsActive.Del(activeQuestsList);
			questsFinished.Add(activeQuestsList->data);
			//REWARDS();
		}

		activeQuestsList = activeQuestsList->next;
	}
	/////////////////////////////////////////////////////////////////////////////

	// Quest chain logic (if required quest is completed)
	// ToDo 6: Implement the code that gives a basic chainquest logic. If an id in finished list meets the
	// requiredId from the inactive list, do the corresponding changes 
	/////////////////////////////////////////////////////////////////////////////

	while (inactiveQuestsList != NULL)
	{
		if (inactiveQuestsList->data->requiredId != 0)
		{
			ListItem<Quest*>* L2 = questsFinished.start;
			while (L2 != NULL)
			{
				if (inactiveQuestsList->data->requiredId == L2->data->id)
				{
					questsActive.Add(inactiveQuestsList->data);
					questsInactive.Del(inactiveQuestsList);
					inactiveQuestsList->data->status = 1;
				}
				L2 = L2->next;
			}
		}

		inactiveQuestsList = inactiveQuestsList->next;
	}
	/////////////////////////////////////////////////////////////////////////////

	// Complex ChainQuests Hardcoded to put it at active list
	/*if (app->player->chopTreeCount == 10 && app->player->turtleKilled)
	{
		ListItem<Quest*>* chainQuestLookingList = questsInactive.start;
		while (chainQuestLookingList != NULL)
		{
			if (chainQuestLookingList->data->id == 5)
			{
				questsActive.Add(chainQuestLookingList->data);
				questsInactive.Del(chainQuestLookingList);
				chainQuestLookingList->data->status = 1;
			}
			chainQuestLookingList = chainQuestLookingList->next;
		}
	}*/

	return true;
}

bool QuestManager::CheckObjectivesCompletion()
{
	ListItem<Quest*>* C = questsActive.start;
	while (C != nullptr)
	{
		if (C->data->progress == C->data->quantity)
		{
			CompleteQuest(C->data->id);
		}
		C = C->next;
	}
	//IDEA -> PASS THE OBJECTIVE AND THE NUMBER VIA XML AND WITH type (0 gather, 1 kill, 2 delivery, 3 dialogue) clasify in three functions for CheckingObjectiveCompletion
	//GATHER -> attach and id or name atribute to entities sort all the entities searching for the one requested and comprove the number u killed if u kill a new one change in XML
	//KILL -> attach and id or name atribute to entities sort all the entities searching for the one requested and comprove the number u killed if u kill a new one change in XML
	//DELIVERY -> button ID with CompleteQuest
	//DIALOGUE -> some random hardcoded shit
	return true;
}

// ToDo 7: Fulfill the CompleteQuest function that given a quest id parameter, changes the variable
// isCompleted to true if that id meets another same id within the whole list of quests
/////////////////////////////////////////////////////////////////////////////
bool QuestManager::CompleteQuest(int id)
{
	ListItem<Quest*>* L = questsList.start;
	while (L != nullptr)
	{
		if (id == L->data->id)
		{
			L->data->isCompleted = true;
		}
		L = L->next;
	}
	return true;
}

bool QuestManager::AcceptQuest(int id)
{
	ListItem<Quest*>* L = questsInactive.start;
	while (L != nullptr)
	{
		if (id == L->data->id)
		{
			L->data->status = 1;
			questsActive.Add(L->data);
			//find a way to only delete one quest and not the whole List 
			questsInactive.Del(L);
			//-
		}
		L = L->next;
	}
	return true;
}

bool QuestManager::AbandonQuest(int id)
{
	ListItem<Quest*>* L = questsActive.start;
	while (L != nullptr)
	{
		if (id == L->data->id)
		{
			L->data->status = 0;
			//find a way to only delete one quest and not the whole List 
			questsActive.Del(L);
			//-
			questsInactive.Add(L->data);
		}
		L = L->next;
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////