
#include "Card.h"

//Indivudal card

Card::Card() {
    std::cerr << "Undeclared Card Exception:            Exiting...";
    abort();
}
Card::Card(Suit suit, Color color, Value value) {
    this->suit = suit;
    this->color = color;
    this->value = value;
}

Suit Card::getSuit() {
    return suit;
}
Color Card::getColor() {
    return color;
}
Value Card::getValue() {
    return value;
}
    

//Operator overloads

//Maps for printing
std::map<Color, std::string> colMap = { {Red, "Red"}, {Black, "Black"}, };
std::map<Suit, std::string> suitMap = { {Spade, "Spade"}, {Diamond, "Diamond"}, {Heart, "Heart"}, {Club, "Club"}, };
std::map<Value, std::string> valMap = { {One, "One"}, {Two, "Two"}, {Three, "Three"}, {Four, "Four"}, {Five, "Five"}, {Six, "Six"}, {Seven, "Seven"}, {Eight, "Eight"}, {Nine, "Nine"}, {Ten, "Ten"}, {Jack, "Jack"}, {Queen, "Queen"}, {King, "King"}, {Ace, "Ace"} };

std::ostream& operator<<(std::ostream& out, const Card& card)
{
    //Idk how to not make these public atm
    out << colMap[card.color] << " " << suitMap[card.suit] << " " << valMap[card.value] << "\n";
    return out;
}

bool operator==(const Card& card1, const Card& card2) {
    return (card1.color == card2.color && card1.suit == card2.suit && card1.value == card2.value);
}

bool operator<(const Card& card1, const Card& card2) {
    return card1.value < card2.value;
}
