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
	app->party->InitializeParties();
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
bool PartyManager::PostUpdate(float dt)
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

Party::Party(std::vector<Member*>& list) : list(list)
{
}

Party::~Party()
{
	for (int i = 0; i < list.size(); i++) {
		delete list.at(i);
	}
}

void Party::PrintMemberDescription(std::string name)
{
	/*ListItem<Member*>* item = list.start;
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
			/*}

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
	}*/

}

void Party::PrintPartyDescription()
{
	

	/*ListItem<Member*>* item = list.start;

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
		/*}

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
	}*/
}

void Party::AddMember(Member* member)
{
	member->data.id = list.size();
	list.push_back(member);
}

void Party::RemoveMember(const std::string name)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list.at(i)->name == name)
			list.erase(list.begin() + i);
		if (list.at(i)->data.id <= 0) list.at(i)->data.id = 0;
		else list.at(i)->data.id -= 1;
	}
}

void PartyManager::InitializeParties()
{
	allyParty = new Party("ALLY PARTY");
	Member* toisto = new Member("Toisto", PLAYER1, 100.0f, 20.0f, false);
 	allyParty->AddMember(toisto);
	Member* thyma = new Member("Thyma", PLAYER2, 100.f, 25.f, false);
	allyParty->AddMember(thyma);

	Action* a = new Action("Punch", toisto, Action::Filter::ENEMY, 1, 0, Action::Type::ATTACK);
	toisto->data.actions.push_back(a);
	a = new Action("Medkit", toisto, Action::Filter::ALLY, 0, 2, Action::Type::DEFENSE);
	toisto->data.actions.push_back(a);

	a = new Action("Slash", thyma, Action::Filter::ENEMY, 1.2f, 0, Action::Type::ATTACK);
	thyma->data.actions.push_back(a);

	//allyParty->PrintPartyDescription();

	enemyParty = new Party("ENEMY PARTY");
	Member* kmush = new Member("KingMush", SHROOM1, 60.0f, 18.0f, false);
	Member* mush = new Member("LilMush", SHROOM2, 40.0f, 15.0f, false);
	Member* bones = new Member("Mr.Bones", SKELETON, 50.0f, 12.0f, false);
	enemyParty->AddMember(kmush);
	enemyParty->AddMember(bones);
	enemyParty->AddMember(mush);

	a = new Action("punch", kmush, Action::Filter::ENEMY, 1, 0, Action::Type::ATTACK);
	kmush->data.actions.push_back(a);
	a = new Action("punch", mush, Action::Filter::ALLY, 0, 1, Action::Type::DEFENSE);
	mush->data.actions.push_back(a);
	a = new Action("cure", mush, Action::Filter::ENEMY, 1, 0, Action::Type::ATTACK);
	mush->data.actions.push_back(a);
	a = new Action("punch", bones, Action::Filter::ENEMY, 1.3f, 0, Action::Type::ATTACK);
	bones->data.actions.push_back(a);




}

void PartyManager::InitializeEnemyParties()
{
	enemyParty = new Party("ENEMY PARTY");
	Member* kmush = new Member("KingMush", SHROOM1, 60.0f, 18.0f, false);
	Member* mush = new Member("LilMush", SHROOM2, 40.0f, 15.0f, false);
	Member* bones = new Member("Mr.Bones", SKELETON, 50.0f, 12.0f, false);
	enemyParty->AddMember(kmush);
	enemyParty->AddMember(bones);
	enemyParty->AddMember(mush);

	Action* a = new Action("punch", kmush, Action::Filter::ENEMY, 1, 0, Action::Type::ATTACK);
	kmush->data.actions.push_back(a);
	a = new Action("punch", mush, Action::Filter::ALLY, 0, 1, Action::Type::DEFENSE);
	mush->data.actions.push_back(a);
	a = new Action("cure", mush, Action::Filter::ENEMY, 1, 0, Action::Type::ATTACK);
	mush->data.actions.push_back(a);
	a = new Action("punch", bones, Action::Filter::ENEMY, 1.3f, 0, Action::Type::ATTACK);
	bones->data.actions.push_back(a);
}

Member* Party::FindByName(const std::string name) const
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list.at(i)->name == name)
			return list.at(i);
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
	data.maxHealth = health;
	data.power = power;
	data.dead = isDead;
}

Member::~Member()
{
 	data.CleanUp();
}
