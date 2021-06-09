#ifndef __PARTYMANAGER_H__
#define __PARTYMANAGER_H__

#include "ECS.h"
#include "Module.h"
#include "List.h"
#include "Log.h"

#include <string>
#include <iostream>
#include <math.h>

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
	float health = 0;
	int maxHealth = 100;
	float exp = 0;
	float maxExp = 15;
	float armor = 0;
	float power = 0;
	bool dead = false;
	int money = 0;
	int level = 1;
	std::vector<Action*> actions;

	int GetMoney() { return money; }
	int GetLevel() { return level; }
	float GetHealth() { return health; }
	float GetMaxHealth() { return maxHealth; }
	
	float GetExp() { return exp; }
	int GetMaxExp() { return maxExp; }
	float GetArmor() { return armor; }
	float GetPower() { return power; }
	
	void Addhealth(float hp) 
	{
		health += hp;
		if (health > 100) health = 100;
	}
	void AddMoney(int m) { money = money + m; }
	void AddExp(float ex) 
	{
		exp = exp + ex; 
		while (exp >= maxExp)
		{
			level++;
			exp = exp - maxExp;
			if (exp < 0) exp = 0;
			maxExp = ((50 / 3) * (level * level) - (6 * (level * level) + 17 * level - 12));
			
		}
			
		
	}
	void AddArmor(int ar) { armor = armor + ar; }
	void AddPower(float pw) { power = power + pw; }
	void AddLevel(int lvl) { level = level + lvl; }

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
		HEAL,
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

		if (other->data.health > other->data.maxHealth)
			other->data.health = other->data.maxHealth;
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
	bool PostUpdate(float dt);
	bool CleanUp();

	void RemoveParties();

	void OpenPartyInventory();
	void InitializeParties();
	void InitializeEnemyParties();

	Party* allyParty = nullptr;
	Party* enemyParty = nullptr;
private:

};

#endif