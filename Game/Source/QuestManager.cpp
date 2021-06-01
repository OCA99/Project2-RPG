#include "QuestManager.h"
#include "Log.h"
#include "Fonts.h"
#include "Input.h"
#include "Render.h"
#include "SceneManager.h"
#include "DialogSytem.h"
#include "Input.h"
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
	
	pugi::xml_node questNode;
	pugi::xml_document questData;
	pugi::xml_parse_result parseResult = questData.load_file("quests.xml");
	if (parseResult == NULL)
		LOG("Could not load xml file <quests.xml> pugi error: %s", parseResult.description());
	else
		questNode = questData.child("quests");
	if (questNode == NULL)
		LOG("Could not load <questsData> xml_document");

	questNode = questNode.child("quest");
	while (questNode != NULL)
	{
		Quest* quest = new Quest(questNode);

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

		questNode = questNode.next_sibling("quest");
	}

	customerTex = app->tex->Load("Textures/Dialogue/blacksmith_dialogue.png");
	reaperTex = app->tex->Load("Textures/Dialogue/reaper_dialogue.png");
	tavernTex = app->tex->Load("Textures/Dialogue/tavern_lady_dialogue.png");
	thymaTex = app->tex->Load("Textures/Dialogue/thyma_good_dialogue.png");
	exclamation = app->tex->Load("Textures/UI/HUD/alert_marks_try.png");
	return true;
}

bool QuestManager::Update(float dt)
{

	CheckQuestsLogic();

	if (!app->input->pads[0].back) selectPressed = true;

	if ((app->scene->currentScene->type == Scene::TYPE::MAP && app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN || (app->input->pads[0].back == true && selectPressed)) && !app->scene->menu && !app->items->invOpened && app->dialog->currentDialog == nullptr)
	{
		selectPressed = false;
		if (questInvOpened) app->ui->DestroyAllGuiControls();
		questInvOpened = !questInvOpened;//Open or close Inv
	}

	if (questInvOpened)
	{
		if (buttons)
		{
			CreateQuestButtons();
			buttons = false;
		}
	}

	if (!questInvOpened) buttons = true;

	return true;
}

bool QuestManager::PostUpdate(float dt)
{

	DrawActiveQuests();

	if (questInvOpened)
	{
		if(!app->scene->buttons && !app->scene->menu && !app->items) CreateQuestButtons();
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

	if (app->scene->currentScene->type == Scene::TYPE::MAP && !questInvOpened && !app->scene->menu && !app->items->invOpened)
	{
		ExclamationDraw();
		InterrogantDraw();
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
		case 1: 
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
			

				break;
		case 2: 
			numToStr2 = numToStr.c_str();
			
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
			
				break;

		case 3: 
			numToStr2 = numToStr.c_str();
		
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
			
				break;
		case 4:
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
			

				break;
		case 5:
		
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
			

				break;
		case 6:
		
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				
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
		app->fonts->BlitText(40, 80 + (32 * i), 1, text.c_str());
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
				text = ToUpperCase(questsActive[y]->objective.GetString());
				app->fonts->BlitText(235, 300, 0, text.c_str());
				text = ToUpperCase(to_string(questsActive[y]->progress));
				app->fonts->BlitText(450, 300, 0, text.c_str());
				text = ToUpperCase(to_string(questsActive[y]->quantity));
				app->fonts->BlitText(470, 300, 0, text.c_str());
				app->fonts->BlitText(460, 300, 0, "/");
				

				//DRAW NPC TEXTURE
				if (questsActive[y]->demandingNPC == SString("customer")) app->render->DrawTexture(customerTex, 10, 15, &SDL_Rect({ 0,0,1280,720 }), 0.5f, 1, 0, 0, 0, false);
				if (questsActive[y]->demandingNPC == SString("reaper")) app->render->DrawTexture(reaperTex, 10, 18, &SDL_Rect({ 0,0,1280,720 }), 0.5f, 1, 0, 0, 0, false);
				if (questsActive[y]->demandingNPC == SString("tlady")) app->render->DrawTexture(tavernTex, 10, 16, &SDL_Rect({ 0,0,1280,720 }), 0.5f, 1, 0, 0, 0, false);
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
		app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 59 / 2 , 36 / 2, 30, 30 }), 14);//Back Button 14
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

	ListItem<Quest*>* activeQuestsList = questsActive.start;
	ListItem<Quest*>* inactiveQuestsList = questsInactive.start;

	while (activeQuestsList != nullptr)
	{
		if (activeQuestsList->data->isCompleted == true)
		{
			activeQuestsList->data->status = 2;
			questsFinished.Add(activeQuestsList->data);
			questsActive.Del(activeQuestsList);
			//REWARDS();
			app->items->GiveItemToPlayer(activeQuestsList->data->reward);
		}

		activeQuestsList = activeQuestsList->next;
	}
	

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


void QuestManager::ExclamationDraw()
{
	ListItem<Quest*>* inactiveQuestsList = app->quests->questsInactive.start;
	while (inactiveQuestsList != nullptr)
	{
		if (inactiveQuestsList->data->demandingNPC == "thyma" && app->map->currentMapName == "Town.tmx")
		{
			app->render->DrawTexture(exclamation, 84, 212, &SDL_Rect({ 0,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
		}

		if (inactiveQuestsList->data->demandingNPC == "tlady" && app->map->currentMapName == "Tavern.tmx")
		{
			app->render->DrawTexture(exclamation, 397, 212, &SDL_Rect({ 0,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
		}

		if (inactiveQuestsList->data->demandingNPC == "reaper" && app->map->currentMapName == "Forest.tmx")
		{
			app->render->DrawTexture(exclamation, 1508, 823, &SDL_Rect({ 0,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
		}

		if (inactiveQuestsList->data->demandingNPC == "customer" && app->map->currentMapName == "NPC House.tmx")
		{
			app->render->DrawTexture(exclamation, 404, 246, &SDL_Rect({ 0,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
		}

		inactiveQuestsList = inactiveQuestsList->next;
	}
}


void QuestManager::InterrogantDraw()
{
	ListItem<Quest*>* activeQuestsList = app->quests->questsActive.start;
	while (activeQuestsList != nullptr)
	{
		if (activeQuestsList->data->progress == activeQuestsList->data->quantity)
		{
			if (activeQuestsList->data->rewardingNPC == "thyma" && app->map->currentMapName == "Town.tmx")
			{
				app->render->DrawTexture(exclamation, 84, 212, &SDL_Rect({ 25,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
			}

			if (activeQuestsList->data->rewardingNPC == "tlady" && app->map->currentMapName == "Tavern.tmx")
			{
				app->render->DrawTexture(exclamation, 397, 212, &SDL_Rect({ 25,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
			}

			if (activeQuestsList->data->rewardingNPC == "reaper" && app->map->currentMapName == "Forest.tmx")
			{
				app->render->DrawTexture(exclamation, 1509, 823, &SDL_Rect({ 25,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
			}

			if (activeQuestsList->data->rewardingNPC == "customer" && app->map->currentMapName == "NPC House.tmx")
			{
				app->render->DrawTexture(exclamation, 405, 246, &SDL_Rect({ 25,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
			}
		}
		if (activeQuestsList->data->type == 3)
		{
			if (activeQuestsList->data->rewardingNPC == "thyma" && app->map->currentMapName == "Town.tmx")
			{
				app->render->DrawTexture(exclamation, 84, 212, &SDL_Rect({ 25,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
			}

			if (activeQuestsList->data->rewardingNPC == "tlady" && app->map->currentMapName == "Tavern.tmx")
			{
				app->render->DrawTexture(exclamation, 397, 212, &SDL_Rect({ 24,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
			}

			if (activeQuestsList->data->rewardingNPC == "reaper" && app->map->currentMapName == "Forest.tmx")
			{
				app->render->DrawTexture(exclamation, 1509, 823, &SDL_Rect({ 25,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
			}

			if (activeQuestsList->data->rewardingNPC == "customer" && app->map->currentMapName == "NPC House.tmx")
			{
				app->render->DrawTexture(exclamation, 405, 246, &SDL_Rect({ 25,0,24,24 }), 0.5f, 1, 0, 0, 0, true);
			}



		}


		activeQuestsList = activeQuestsList->next;
	}
}