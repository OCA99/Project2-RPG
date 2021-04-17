#ifndef __PARTYMANAGER_H__
#define __PARTYMANAGER_H__

#include "ECS.h"
#include "Module.h"
#include "List.h"

#include <string>
#include <iostream>

struct Action;

enum Type {
	PLAYER1,
	PLAYER2,
	SKELETON,
	SHROOM1,
	SHROOM2
};

struct Data {
	//Numero en la Party
	int id;
	float health;
	float power;
	bool dead = false;
	List<Action*> actions;

	void CleanUp() {
		for (int i = 0; i < actions.Count(); i++) {
			delete actions.At(i)->data;
		}
	}
};

struct Member {
	//Sprite sprite;
	std::string name;
	Type type;
	Data data;

	Member();
	Member(std::string name, Type type);
	Member(std::string name, Type type, float health, float damage, bool isDead);
	~Member();
};

struct Action {
	enum class Filter {
		ALLY,
		ENEMY,
		BOTH
	};

	Action(std::string name, Member* _owner, Filter filter, float damage, float heal) : name(name), filter(filter), damage(damage), heal(heal)
	{
		owner = _owner;
	}

	Member* owner = nullptr;
	std::string name;
	Filter filter;
	float damage;
	float heal;

	void Apply(Member* other) {
		other->data.health += owner->data.power * heal;
		other->data.health -= owner->data.power * damage;

		if (other->data.health <= 0) other->data.dead = true;

		std::cout << owner->name << ": + -> " << owner->data.power * heal << ", - -> " << owner->data.power * damage << " to " << other->name << std::endl;
		if (other->data.dead)
			std::cout << other->name << " is dead" << std::endl;
	}
};

struct Party {

	List<Member*> list;
	std::string partyName;

	//Functions
	Party();
	Party(std::string partyName);
	Party(List<Member*>& list);
	~Party();

	void PrintMemberDescription(std::string name);
	void PrintPartyDescription();
	void AddMember(Member* member);
	void RemoveMember(const std::string name);
	//Find Member
	ListItem<Member*>* FindByName(const std::string name) const;

};

class PartyManager : public Module
{

public:

	PartyManager();
	~PartyManager();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void RemoveParties();

	void OpenPartyInventory();
	void InitializeParties();

	Party* allyParty = nullptr;
	Party* enemyParty = nullptr;
private:

};

#endif