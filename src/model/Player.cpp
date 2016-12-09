#include "model/Player.h"
#include "model/cards/Card.h"
#include <string.h>
#include "functions.h"

Player::Player(const char* name) {
	strcpy(this->name, name);
}

const char* Player::getName() {
	return this->name;
}

void Player::setName(const char* name) {
	strcpy(this->name, name);
}

void Player::pushCardToHand(shared_ptr<Card> card) {
	this->hand.push_back(card);
}
shared_ptr<Card> Player::removeCardFromHand(int cardNum) {
	if(!checkRange(cardNum, 0, hand.size())) {
		return nullptr;
	}
	shared_ptr<Card> returnCard = hand[cardNum];
	hand.erase(hand.begin() + cardNum);
	return returnCard;
}
shared_ptr<Card> Player::popCardFromHand() {
	if(hand.size() == 0) {
		return nullptr;
	}
	return *(hand.erase(hand.begin()+hand.size()-1));
}
shared_ptr<Card> Player::getCardFromHand(int cardNum) {
	if(!checkRange(cardNum, 0, hand.size())) {
		return nullptr;
	}
	return hand[cardNum];
}

int Player::handSize() {
	return hand.size();
}



