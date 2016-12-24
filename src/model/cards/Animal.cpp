#include "model/cards/Animal.h"
#include "model/cards/abilities/Fat.h"
#include "model/cards/abilities/Ability.h"
#include "model/cards/interfaces/FoodModification.h"
#include "exceptions/Exception.h"
#include <memory>
#include <string>
#include <typeinfo>

using namespace std;

Animal::Animal(shared_ptr<AbilityCard> createdFrom, Player* owner) {
	this->createdFrom = createdFrom;
	this->_isHungry = true;
	this->_needFood = true;
	this->owner = owner;
}
vector<shared_ptr<AbilityCard>>* Animal::getAbilities() {
	return &abilities;
}
Player* Animal::getOwner() {
	return owner;
}
shared_ptr<AbilityCard> Animal::getCreatedFrom() {
	return createdFrom;
}

void Animal::pushAbility(shared_ptr<AbilityCard> newAbility) {
	newAbility->setOwner(this);
	abilities.push_back(newAbility);
}
shared_ptr<AbilityCard> Animal::popAbility() {
	if (abilities.size() == 0) {
		throw Exception("Cannot pop animal ability because there is no one");
	}
	shared_ptr<AbilityCard> abilityToReturn = abilities[abilities.size()-1];
	abilities.pop_back();
	return abilityToReturn;
}

AbilityCard* Animal::getAbility(int index) {
	if (index >= abilities.size() || index < 0) {
		throw Exception("Cannot get ability on illegal index");
	}
	return abilities[index].get();
}

string Animal::getStatus() {
	if (abilities.size() == 0) {
		return "Empty animal";
	}
	string status;
	for (auto ibegin = abilities.begin(); ibegin != abilities.end(); ibegin++) {
		AbilityCard* ability = (*ibegin).get();
		status += ability->getStatus();
		status += " ";
	}
	return status;
}
int Animal::getCost() {
	int totalCost = 3;
	for (auto ibegin = abilities.begin(); ibegin != abilities.end(); ibegin++) {
		totalCost += (*ibegin)->getCost();
	}
	return totalCost;
}

bool Animal::needFood() {
	return _needFood;
}
bool Animal::isHungry() {
	return _isHungry;
}
void Animal::setHungry(bool hungry) {
	this->_isHungry = hungry;
	if (hungry) {
		this->_needFood = true;
	}	
}
void Animal::setNeedFood(bool needFood) {
	this->_needFood = needFood;
}

int Animal::feed() {
	bool abilitiesNeedFood = false;
	int abilityToFeed = -1;
	for (int i = 0; i < abilities.size(); i++) {
		AbilityCard* ability = abilities[i].get();
		// 2) check if ability needs food
		switch (ability->getAbility()) {
			case (Ability::FAT) : {
				// feed fat only if animal is not hungry				
				Fat* fatCard = (Fat*)ability;
				// feed only if fat is empty
				if (fatCard->needFood()) {
					if (!this->_isHungry) {	// if animal is hungry, you cannot fill fat
						if (abilityToFeed != -1) {	// some fat has been filled earlier
							abilitiesNeedFood = true;	// mark that there is empty fat
							break;
						}
						fatCard->giveFood();
						abilityToFeed = i;
					} else {
						abilitiesNeedFood = true;//mark that there is empty fat
					}		
				}
				break;
			}
		}		
	}
	// no ability needs food
	if (abilityToFeed == -1) {
		_isHungry = false;
	} 
	if (!abilitiesNeedFood) {
		_needFood = false;
	}
	return abilityToFeed;
}