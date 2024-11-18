#pragma once

#include <iostream>
#include <map>
#include <string>

enum Suit {
    Spade, Diamond, Heart, Club
};
enum Color {
    Red, Black
};
enum Value {
    One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace
};

class Card {
    
public:

    Suit suit;
    Color color;
    Value value;

    Card();
    Card(Suit suit, Color color, Value value);

    Suit getSuit();
    Color getColor();
    Value getValue();
    
};

extern std::ostream& operator<<(std::ostream& out, const Card& card);
extern bool operator==(const Card& card1, const Card& card2);
extern bool operator<(const Card& card1, const Card& card2);