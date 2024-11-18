#pragma once
#include <stack>
#include <algorithm>
#include <vector>
#include "Card.h"

class Deck {
	std::stack<Card> cards;
	int numCards;

	void m_printDeck();

public:
	Deck();
	void m_shuffleDeck();
	Card m_getTop();
	int m_getCardCount();
	
};




