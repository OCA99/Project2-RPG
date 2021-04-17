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
	currentParty->AddMember(Member("Oscar", ENEMY));
	currentParty->AddMember(Member("Telmo", PLAYER));
	currentParty->AddMember(Member("Paula", NONE));
	currentParty->RemoveMember("Oscar");
	//currentParty->RemoveMember("Telmo");

	//Member m = currentParty->FindByName("Telmo")->data;
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

Party::Party()
{
}

Party::Party(List<Member>& list) : list(list)
{
}

void Party::PrintMemberDescription(Member member)
{
	std::cout << "-------------MEMBER:" << member.name << "-------------" << std::endl;
	std::cout << "-------------TYPE:" << member.type << "---------------" << std::endl;
	std::cout << "-------------ISDEAD: " << member.data.dead << "-------------" << std::endl;

}

void Party::PrintPartyDescription()
{
	std::cout << "-------------PARTY MEMBERS " << "---------------" << std::endl;
	std::cout << std::endl;

	ListItem<Member>* item = list.start;
	while (item)
	{
		std::cout << "-------------MEMBER " << item->data.data.id << " : " << item->data.name << "-------------" << std::endl;
		switch (item->data.type)
		{
		case NONE:
		{
			std::cout << "-------------TYPE: NONE " << "---------------" << std::endl;
			break;
		}
		case PLAYER:
		{
			std::cout << "-------------TYPE: PLAYER " << "---------------" << std::endl;
			break;
		}
		case ENEMY:
		{
			std::cout << "-------------TYPE: ENEMY " << "---------------" << std::endl;
			break;
		}
		/* etc... */
		}
		switch (item->data.data.dead)
		{
		default:
			break;
		case 0:
		{
			std::cout << "-------------ISDEAD: FALSE " << "-------------" << std::endl;
		}
		case 1:
		{
			std::cout << "-------------ISDEAD: TRUE " <<  "-------------" << std::endl;
		}
		}
		std::cout << std::endl;
		item = item->next;
	}
}

void Party::AddMember(Member member)
{
	if (list.start == nullptr)
		member.data.id = 0;
	else
		member.data.id = list.end->data.data.id + 1;

	list.Add(member);

}

void Party::RemoveMember(const std::string name)
{
	ListItem<Member>* item = list.start;
	while (item)
	{
		if (item->data.name == name)
			list.Del(item);
		if (item->data.data.id <= 0) item->data.data.id = 0;
		else item->data.data.id--;

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
