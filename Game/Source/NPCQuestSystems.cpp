#include "NPCQuestSystem.h"
#include "QuestManager.h"
#include "App.h"
#include "Textures.h"

void NPCQuestSystem::tick(ECS::World* world, float dt)
{
	ListItem<Quest*>* active = app->quests->questsActive.start;
	ListItem<Quest*>* inactive = app->quests->questsInactive.start;

	LoadTex();

	if (active != nullptr)
	{
		app->render->DrawTexture(exclamation, 10, 18, &SDL_Rect({ 32,32,10,16 }), 0.5f, 1, 0, 0, 0, false);
	}

	world->each<QuestList>([&](ECS::Entity* entity, ECS::ComponentHandle<QuestList> QuestList) {
		while (active != nullptr)
		{
			if (active->data->isCompleted == true)
			{
				active->data->status = 2;
				QuestList->qActive.Del(active);
				QuestList->qInactive.Add(active->data);
			}

			active = active->next;
		}


		while (inactive != nullptr)
		{
			if (inactive->data->requiredId != 0)
			{
				QuestList->qInactive.Del(inactive);
				QuestList->qActive.Add(inactive->data);

			}

			inactive = inactive->next;
		}

		while (active != nullptr)
		{
			if (active->data->isCompleted == true)
			{
				active->data->status = 2;
				QuestList->qActive.Del(active);
			}
			active = active->next;
		}

		while (inactive != nullptr)
		{
			if (inactive->data->isCompleted == true)
			{
				inactive->data->status = 1;
				QuestList->qInactive.Del(inactive);
			}
			inactive = inactive->next;
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
