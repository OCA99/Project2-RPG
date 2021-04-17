#include "PartyManager.h"
#include "App.h"
#include "Input.h"
#include "GuiManager.h"
#include "Textures.h"
#include "Audio.h"

#include "SpriteSystem.h"

#include "Defs.h"
#include "Log.h"
#include "SDL/include/SDL_scancode.h"

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
	currentParty->AddMember(Member("Eduardo", NONE, 10.0f, 20.0f, true));
	currentParty->AddMember(Member("Telmo", PLAYER, 100.f, 15.f, false));
	currentParty->AddMember(Member("Oscar", PLAYER, -10.f, 0.f, false));
	currentParty->AddMember(Member("Paula", NONE, 70.0f, 30.f, true));
	//currentParty->RemoveMember("Oscar");
	//currentParty->RemoveMember("Telmo");

	currentParty->PrintMemberDescription("Telmo");
	currentParty->PrintPartyDescription();


	//CARGAR GUI


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
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		//ABRIR PARTY
	}

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

void PartyManager::OpenPartyInventory()
{


}

Party::Party()
{
}

Party::Party(List<Member>& list) : list(list)
{
}

Party::~Party()
{
}

void Party::PrintMemberDescription(std::string name)
{
	ListItem<Member>* item = list.start;
	std::cout << "-------------MEMBER INFO " << "---------------" << std::endl;
	std::cout << std::endl;

	while (item)
	{
		if (item->data.name == name)
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
				break;
			}
			case 1:
			{
				std::cout << "-------------ISDEAD: TRUE " << "-------------" << std::endl;
				break;
			}
			}

			std::cout << "-------------Health: " << item->data.data.health << "---------------" << std::endl;
			std::cout << "-------------Damage: " << item->data.data.damage << "---------------" << std::endl;

			std::cout << std::endl;
		}

		item = item->next;
	}

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
			break;
		}
		case 1:
		{
			std::cout << "-------------ISDEAD: TRUE " << "-------------" << std::endl;
			break;
		}
		}
		std::cout << "-------------Health: " << item->data.data.health << "---------------" << std::endl;
		std::cout << "-------------Damage: " << item->data.data.damage << "---------------" << std::endl;
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

Member::Member(std::string name, Type type, float health, float damage, bool isDead) : name(name), type(type)
{
	data.health = health;
	data.damage = damage;
	data.dead = isDead;
}

Member::~Member()
{
}
