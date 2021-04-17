#include "PartyManager.h"
#include "App.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>

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
	currentParty = new Party();
	currentParty->list.Add(Member("Telmo", NONE));
	currentParty->list.Add(Member("Oscar", ENEMY));

	Member m = currentParty->FindByName("Telmo")->data;

	//currentParty->PrintMemberDescription(m);
	currentParty->PrintPartyDescription();

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
	party->list.Add(member);

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

Party::Party()
{
}

Party::Party(List<Member*>& list)
{
}

void Party::PrintMemberDescription(Member member)
{
	std::cout << "-------------MEMBER:"<< member.name << "-------------" << std::endl;
	std::cout << "-------------TYPE:" << member.type << "---------------" << std::endl;
	std::cout << "-------------ISDEAD: " << member.data.dead << "-------------" << std::endl;

}

void Party::PrintPartyDescription()
{
	ListItem<Member>* item = list.start;
	while (item)
	{
		std::cout << "-------------MEMBER:" << item->data.name << "-------------" << std::endl;
		std::cout << "-------------TYPE:" << item->data.type << "---------------" << std::endl;
		std::cout << "-------------ISDEAD: " << item->data.data.dead << "-------------" << std::endl;
		item = item->next;
	}
}

ListItem<Member>* Party::FindByName(const std::string name) const
{
	ListItem<Member>* item = list.start;
	while (item)
	{

		if (item->data.name == name)
			return item;
		item = item->next;
	}
	return nullptr;
}

Member::Member()
{
}

Member::Member(std::string name, Type type) : name(name), type(type)
{
}

Member::Member(std::string name, Type type, float health, float damage, bool dead) : name(name), type(type)
{
	data.health = health;
	data.damage = damage;
	data.dead = dead;
}
