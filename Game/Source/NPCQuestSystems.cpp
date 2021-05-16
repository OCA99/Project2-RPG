#include "NPCQuestSystem.h"
#include "QuestManager.h"
#include "App.h"
#include "Textures.h"

void NPCQuestSystem::tick(ECS::World* world, float dt)
{
	ListItem<Quest*>* active = app->quests->questsActive.start;
	ListItem<Quest*>* inactive = app->quests->questsInactive.start;
	ListItem<Quest*>* finished = app->quests->questsFinished.start;

	LoadTex();

	world->each<QuestList>([&](ECS::Entity* entity, ECS::ComponentHandle<QuestList> QuestList) {
		//ACTIVE TO FINISHED
		while (active != nullptr)
		{
			if (active->data->status == 2)
			{
				QuestList->qFinished.Add(active->data);
				QuestList->qActive.Del(active);
			}
			active = active->next;
		}

		//INACTIVE TO ACTIVE
		while (inactive != nullptr)
		{
			if (inactive->data->status == 1)
			{
				QuestList->qActive.Add(inactive->data);
				QuestList->qInactive.Del(inactive);
			}
			app->render->DrawTexture(exclamation, QuestList->position.x + 8, QuestList->position.y - 14, &SDL_Rect({ 32,32,10,16 }), 0.5f, 1, 0, 0, 0, true);
			
			inactive = inactive->next;
		}

		//ACTIVE TO INACTIVE
		while (active != nullptr)
		{
			if (active->data->status == 0)
			{
				QuestList->qInactive.Add(inactive->data);
				QuestList->qActive.Del(active);
			}
			active = active->next;
		}

	});
	
	
}


void NPCQuestSystem::LoadTex()
{
	if (loaded == false && app->quests->exclamation != nullptr) 
	{
		exclamation = app->quests->exclamation;
		loaded = true;
	}
}
