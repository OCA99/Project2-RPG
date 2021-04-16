#include "PartyManager.h"
#include "App.h"

#include "Defs.h"
#include "Log.h"

PartyManager::PartyManager() : Module()
{
	name.Create("party");
}

// Destructor
PartyManager::~PartyManager()
{}

// Called before render is available
bool PartyManager::Awake()
{
	LOG("Loading Party");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool PartyManager::Start()
{

	return true;
}

// Called each loop iteration
bool PartyManager::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool PartyManager::Update(float dt)
{


	return true;
}

// Called each loop iteration
bool PartyManager::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool PartyManager::CleanUp()
{
	return true;
}

void PartyManager::AddMember(Member& member, Party* party)
{
	//party->list.Add(member);

}

void PartyManager::RemoveMember(Member& member, Party* party)
{
	//for (ListItem<Member>* list = party->list.start; list != NULL; list = list->next)
	//{
	//	if (list->data.name == member.name)
	//	{
	//	
	//	}
	//}
}


