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
	float maxHealth;
	float power;
	bool dead = false;
	std::vector<Action*> actions;

	void CleanUp() {
		for (int i = 0; i < actions.size(); i++) {
			delete actions.at(i);
		}
	}
};


struct Member {
	//Sprite sprite;
	enum class State
	{
		IDLE,
		ATTACKING,
		HURT,
		DEAD
	};

	std::string name;
	Type type;
	Data data;
	State state;

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

	enum class Type {
		ATTACK,
		DEFENSE
	};

	Action(std::string name, Member* _owner, Filter filter, float damage, float heal, Type type) : name(name), filter(filter), damage(damage), heal(heal), type(type)
	{
		owner = _owner;
	}

	Member* owner = nullptr;
	std::string name;
	Filter filter;
	float damage;
	float heal;
	Type type;

	void Apply(Member* other) {
		other->data.health += owner->data.power * heal;
		other->data.health -= owner->data.power * damage;
		
		owner->state = Member::State::ATTACKING;
		other->state = Member::State::HURT;

		if (other->data.health <= 0) {
			other->data.dead = true;
			other->state = Member::State::DEAD;
		}

		std::cout << owner->name << ": + -> " << owner->data.power * heal << ", - -> " << owner->data.power * damage << " to " << other->name << std::endl;
		if (other->data.dead)
			std::cout << other->name << " is dead" << std::endl;


	}
};

struct Party {

	std::vector<Member*> list;
	std::string partyName;

	//Functions
	Party();
	Party(std::string partyName);
	Party(std::vector<Member*>& list);
	~Party();

	void PrintMemberDescription(std::string name);
	void PrintPartyDescription();
	void AddMember(Member* member);
	void RemoveMember(const std::string name);
	//Find Member
	Member* FindByName(const std::string name) const;

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