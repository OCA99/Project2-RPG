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

void PartyManager::RemoveParties()
{
	delete allyParty;
	delete enemyParty;
}

void PartyManager::OpenPartyInventory()
{


}

Party::Party()
{
}

Party::Party(std::string partyName) : partyName(partyName)
{
}

Party::Party(List<Member*>& list) : list(list)
{
}

Party::~Party()
{
	for (int i = 0; i < list.Count(); i++) {
		delete list.At(i)->data;
	}
}

void Party::PrintMemberDescription(std::string name)
{
	ListItem<Member*>* item = list.start;
	std::cout << "-------------MEMBER INFO " << "---------------" << std::endl;
	std::cout << std::endl;

	while (item)
	{
		if (item->data->name == name)
		{
			std::cout << "-------------MEMBER " << item->data->data.id << " : " << item->data->name << "-------------" << std::endl;
			switch (item->data->type)
			{
			case PLAYER1:
			{
				std::cout << "-------------TYPE: PLAYER1 " << "---------------" << std::endl;
				break;
			}
			case PLAYER2:
			{
				std::cout << "-------------TYPE: PLAYER2 " << "---------------" << std::endl;
				break;
			}
			case SHROOM1:
			{
				std::cout << "-------------TYPE: SKELETON1 " << "---------------" << std::endl;
				break;
			}
			case SHROOM2:
			{
				std::cout << "-------------TYPE: SKELETON2 " << "---------------" << std::endl;
				break;
			}
			case SKELETON:
			{
				std::cout << "-------------TYPE: SKELETON2 " << "---------------" << std::endl;
				break;
			}
			/* etc... */
			}

			switch (item->data->data.dead)
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

			std::cout << "-------------Health: " << item->data->data.health << "---------------" << std::endl;
			std::cout << "-------------Damage: " << item->data->data.power << "---------------" << std::endl;

			std::cout << std::endl;
		}

		item = item->next;
	}

}

void Party::PrintPartyDescription()
{
	

	ListItem<Member*>* item = list.start;

	std::cout << "-------------"<<partyName<<" MEMBERS ---------------" << std::endl;
	std::cout << std::endl;
	while (item)
	{
		std::cout << "-------------MEMBER " << item->data->data.id << " : " << item->data->name << "-------------" << std::endl;
		switch (item->data->type)
		{
		case PLAYER1:
		{
			std::cout << "-------------TYPE: PLAYER1 " << "---------------" << std::endl;
			break;
		}
		case PLAYER2:
		{
			std::cout << "-------------TYPE: PLAYER2 " << "---------------" << std::endl;
			break;
		}
		case SHROOM1:
		{
			std::cout << "-------------TYPE: SKELETON1 " << "---------------" << std::endl;
			break;
		}
		case SHROOM2:
		{
			std::cout << "-------------TYPE: SKELETON2 " << "---------------" << std::endl;
			break;
		}
		case SKELETON:
		{
			std::cout << "-------------TYPE: SKELETON2 " << "---------------" << std::endl;
			break;
		}
		/* etc... */
		}

		switch (item->data->data.dead)
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
		std::cout << "-------------Health: " << item->data->data.health << "---------------" << std::endl;
		std::cout << "-------------Damage: " << item->data->data.power << "---------------" << std::endl;
		std::cout << std::endl;
		item = item->next;
	}
}

void Party::AddMember(Member* member)
{
	if (list.start == nullptr)
		member->data.id = 0;
	else
		member->data.id = list.end->data->data.id + 1;

	list.Add(member);
}

void Party::RemoveMember(const std::string name)
{
	ListItem<Member*>* item = list.start;
	while (item)
	{
		if (item->data->name == name)
			list.Del(item);
		if (item->data->data.id <= 0) item->data->data.id = 0;
		else item->data->data.id--;

		item = item->next;
	}
}

void PartyManager::InitializeParties()
{
	allyParty = new Party("ALLY PARTY");
	Member* toisto = new Member("Toisto", PLAYER1, 10.0f, 20.0f, false);
	allyParty->AddMember(toisto);
	Member* thyma = new Member("Thyma", PLAYER2, 100.f, 15.f, false);
	allyParty->AddMember(thyma);

	Action* a = new Action("punch", toisto, Action::Filter::ENEMY, 1, 0);
	toisto->data.actions.Add(a);

	a = new Action("punch", thyma, Action::Filter::ENEMY, 1, 0);
	thyma->data.actions.Add(a);

	//allyParty->PrintPartyDescription();

	enemyParty = new Party("ENEMY PARTY");
	Member* kmush = new Member("King Mush", SHROOM1, 100.0f, 20.0f, false);
	enemyParty->AddMember(kmush);

	a = new Action("punch", kmush, Action::Filter::ENEMY, 1, 0);
	kmush->data.actions.Add(a);
}

ListItem<Member*>* Party::FindByName(const std::string name) const
{
	ListItem<Member*>* item = list.start;
	while (item)
	{

		if (item->data->name == name)
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

Member::Member(std::string name, Type type, float health, float power, bool isDead) : name(name), type(type)
{
	data.health = health;
	data.power = power;
	data.dead = isDead;
}

Member::~Member()
{
 	data.CleanUp();
}
