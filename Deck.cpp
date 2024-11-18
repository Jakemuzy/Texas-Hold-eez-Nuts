
#include "Deck.h"

//Deck

//Really only for debugging purposes
void Deck::m_printDeck() {
    while (!cards.empty()) {
        std::cout << cards.top();
        cards.pop();
    }
}

//Initializes and shuffles deck, kinda messy
Deck::Deck() {

    for (int suit = 0; suit < 4; suit++) {
        for (int col = 0; col < 2; col = col++) {
            for (int val = 0; val < 14; val++) {
                Card c1(static_cast<Suit>(suit), static_cast<Color>(col), static_cast<Value>(val));
                cards.push(c1);

            }
        }
    }

    Deck::m_shuffleDeck();
    //Deck::m_printDeck();
}

//I mean yes inefficient, but not gonna be using it much
void Deck::m_shuffleDeck() {
    std::vector<Card> tempDeck;

    //Swaps stack into vector
    while (!cards.empty()) {
        tempDeck.push_back(cards.top());
        cards.pop();
    }

    //Shuffles
    std::random_shuffle(tempDeck.begin(), tempDeck.end());

    //Swaps vector back into satck
    while (!tempDeck.empty()) {
        cards.push(tempDeck.back());
        tempDeck.pop_back();
    }

}

Card Deck::m_getTop() {
    Card top = cards.top();
    cards.pop();
    numCards--;
    return top;
}

int Deck::m_getCardCount() {
    return numCards;
}

