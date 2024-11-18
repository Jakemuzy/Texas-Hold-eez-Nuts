// Poker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <random>
#include <stdlib.h>
#include "Deck.h"
#include <map>

int potValue = 0, betPrice = 1;
enum playerChoice {
    RAISE, MATCH, FOLD
};

//Struct player
struct Hand {
    //How much moeny they have left
    int moneyLeft = 1000;
    //How much each person put up to gamble, buy in of 1
    int chips = 1;
    //What the player wants to do for the round
    playerChoice choice = FOLD;
    //Which type of hand they have
    int handValue = 0;

    std::vector<Card> knownCards;
    std::vector<Card> unknownCards;
};

//Sort by values, then by suit, then by color
//Very inefficient atm, fix later
void sortHand(Hand& hand) {

    Hand copyHand;
    std::vector<int> values;
    std::vector<int> suits;
    std::vector<int> colors;
    
    //Sort by value

    for (Card card : hand.knownCards) {
        values.push_back(card.getValue());
        suits.push_back(card.getSuit());
        colors.push_back(card.getColor());
    }
    //Needs to put suits and colors in the same order
    std::sort(values.begin(), values.end());

    for (int it = 0; it < values.size(); it++) {
        for (Card card : hand.knownCards) {
            if (values[it] == card.getValue() && std::find(copyHand.knownCards.begin(), copyHand.knownCards.end(), card) == copyHand.knownCards.end()) {
                copyHand.knownCards.push_back(card);
                //hand.knownCards.erase(hand.knownCards.begin() + it);
            }
        }
    }

    hand.knownCards.clear();
    hand.knownCards.assign(copyHand.knownCards.begin(), copyHand.knownCards.end());

    return;
}

//Very inefficient but whatevs, will fix later
//Make it so handValue is a double, and it considers multiple different types of wins (eg. flush and pair > flush)
int determineHandValue(Hand player, Hand dealer) {

    //Combined dealer cards with player cards
    Hand combinedHand = Hand();
    int totalValue, totalColor, totalSuit;
    potValue += player.chips;

    for (Card card : player.knownCards)
        combinedHand.knownCards.push_back(card);
    for (Card card : dealer.knownCards) 
        combinedHand.knownCards.push_back(card);
    

    //Sort hand
    sortHand(combinedHand);
    /*
    for (Card card : combinedHand.knownCards)
        std::cout << card;
        */
    
    //Occurences for value, suit and color
    std::map<int, int> valueOccurences;
    std::map<int, int> suitOccurences;
    std::map<int, int> colorOccurences;


    for (Card card : combinedHand.knownCards) {

        //Value
        if (valueOccurences.find(card.value) != valueOccurences.end())
            valueOccurences[card.value]++;
        else
            valueOccurences[card.value] = 1;

        //Suit
        if (suitOccurences.find(card.suit) != suitOccurences.end())
            suitOccurences[card.suit]++;
        else
            suitOccurences[card.suit] = 1;

        //Color
        if (colorOccurences.find(card.color) != colorOccurences.end())
            colorOccurences[card.color]++;
        else
            colorOccurences[card.color] = 1;
    }

    //Determine which hand you have, using  Deterministic Finite Automata
    bool royalFlush = 0, straightFlush = 0, fourKind = 0, fullHouse = 0, flush = 0, straight = 0, threeKind = 0, twoPair = 0, pair = 0, highCard = 1;

    //Flush checking
    for (auto& suit : suitOccurences) {
        if (suit.second >= 5) {
            //Royal and straight
            int incrementCount = 0;
            int prevNum = valueOccurences.begin()->first;
 
            for (auto& value : valueOccurences) {

                //Counts how many times in a row it occurs
                if (value.first == prevNum + 1 ) {      //Technically wrong because need to consider the suit too
                    incrementCount++;
                }
                else {
                    incrementCount = 0;
                }
                prevNum = value.first;
                
                
                if(incrementCount >= 4){
                    straightFlush = true;
                }
            }
            //I am aware how lazy this is LMAO
            if (valueOccurences[valueOccurences.size()] == 13 && valueOccurences[valueOccurences.size() - 1] == 12 && valueOccurences[valueOccurences.size() - 2] == 11 && valueOccurences[valueOccurences.size() - 3] == 10 && valueOccurences[valueOccurences.size() - 4] == 9) {
                royalFlush = true;
            }
            flush = 1; 
        }
    }
    //Pair, three, four checking, full house, 
    bool threePair = 0;
    for (auto& suit : valueOccurences) {
        if (suit.second == 2) {
            if (pair)
                twoPair = true;
            pair = true;
        }
        else if (suit.second == 3) {
            threePair = true;
        } 
        else if (suit.second == 4) {
            fourKind = true;
        }

        //Full house
        if (threePair && twoPair)
            fullHouse = true;

    }

    //Printing which it is
    int handValue = 0;
    if (royalFlush) {
        //std::cout << "ROYAL";
        handValue = 10;
    } else if (straightFlush) {
        //std::cout << "STRAIGHTFLUSH";
        handValue = 9;
    } else if (fourKind) {
        //std::cout << "FOUR";
        handValue = 8;
    } else if (fullHouse) {
        //std::cout << "FULLHOUSE";
        handValue = 7;
    } else if (flush) {
        //std::cout << "FLUSH";
        handValue = 6;
    } else if (straight) {
        //std::cout << "STRAIGHT";
        handValue = 5;
    } else if (threeKind) {
        //std::cout << "THREE";
        handValue = 4;
    } else if (twoPair) {
        //std::cout << "TWOPAIR";
        handValue = 3;
    } else if (pair) {
        //std::cout << "PAIR";
        handValue = 2;
    } else if (highCard) {
        //std::cout << "HIGHCARD";
        handValue = 1;
    }
    //std::cout << "\n\n";

    return handValue;
}

void determineWinner(std::vector<Hand>& players, Hand dealer) {
    std::map<int, std::string> winningHand{ {1, "Highcard"}, {2, "Pair"}, {3, "Two Pairs"}, {4, "Three of a Kind"}, {5, "Straight"}, {6, "Flush"}, {7, "Full House"}, {8, "Four of a Kind"}, {9, "Straight Flush"}, {10, "Royal Flush"} };

    int currentWinningValue = 0;
    std::vector<int> winnerPositions;

    int currentPlayer = 0;
    for (Hand& player : players) {
        player.handValue = determineHandValue(player, dealer);

        if (player.handValue > currentWinningValue) {
            winnerPositions.clear();
            winnerPositions.push_back(currentPlayer);
            currentWinningValue = player.handValue;
        }
        else if (player.handValue == currentWinningValue) {
            winnerPositions.push_back(currentPlayer);
        }

        currentPlayer++;
    }

    //Multiple winners
    if (winnerPositions.size() > 1) {
        std::cout << "Multiple winners: \n"; 
        for (int pos : winnerPositions) {
            std::cout << "Player " << pos + 1 << " won with a value of " << winningHand[currentWinningValue] << "\n";
        }

    }
    //Single winner
    else {
        if (winnerPositions[0] + 1 == 1) {
            std::cout << "You have won the round with a " << winningHand[currentWinningValue] << ".\nMoney Won " << potValue << "\nCurrent Balance " << players[winnerPositions[0]].moneyLeft << " \n";
        }
        else {
            std::cout << "Player " << winnerPositions[0] + 1 << " won the round with a " << winningHand[currentWinningValue] << "\n";
        }

        betPrice = 1;
        potValue = 0;
    }

    return;
}


void dealCards(int playerCount, std::vector<Hand> & players, Deck deck) {
    Hand dealer = Hand();
    dealer.knownCards.push_back(deck.m_getTop());
    dealer.knownCards.push_back(deck.m_getTop());
    dealer.knownCards.push_back(deck.m_getTop()); 
    dealer.unknownCards.push_back(deck.m_getTop());
    dealer.unknownCards.push_back(deck.m_getTop());

    for (int i = 0; i < playerCount; i++) {
        Hand newHand = Hand();
        newHand.knownCards.push_back(deck.m_getTop());
        newHand.knownCards.push_back(deck.m_getTop());
        
        players.push_back(newHand);
    }

    players.push_back(dealer);

    return;
}

//Check whether raise, match, or fold, if fold return 0
bool handleInput(Hand& player, const Hand& dealer) {

    //Print current hand
    std::cout << "Current Hand: \n";
    for (Card card : player.knownCards) {
        std::cout << card;
    }
    std::cout << "\nShared Cards: \n";
    for (Card card : dealer.knownCards) {
        std::cout << card;
    }
    std::cout << "\n\nCurrent Bid " << betPrice;
    std::cout << "\n\nCurrent Balance " << player.moneyLeft << "\n\n";
    
    int check;
    do {
        std::cout << "Would you like to Raise Match or Fold (0-2) ";
        std::cin >> check;
    } while (check < 0 && check > 2);

    switch (check) {
    case 0:
        //No money left, fold
        if (player.moneyLeft < betPrice) {
            return 0;
        }
        else if (player.moneyLeft == betPrice) {
            std::cout << "You do not have enough money to raise the bet, you only have enough to match. Matching the bet... \n";
        }

        //
        int raise;
        do  {
            std::cout << "What would you like to raise the bet to: ";
            std::cin >> raise;

            if (raise <= betPrice)
                std::cout << "Price must be more than current bet price of " << betPrice << "\n";
            if (player.moneyLeft < raise) {
                std::cout << "You do not have enough money to raise the price to " << raise << ". Your current balance is " << player.moneyLeft << "\n";
                raise = betPrice;
                return 0;
            }
            std::cout << "\n";
        } while (raise <= betPrice);

        betPrice = raise;
        player.chips = betPrice;
        player.moneyLeft -= betPrice;
        potValue += player.chips;
        break;
    case 1:
        //No money left, fold
        if (player.moneyLeft < betPrice) {
            return 0;
        }
        player.chips = betPrice;
        player.moneyLeft - betPrice;
        potValue += player.chips;
        break;
    case 2: 
        potValue += player.chips;
        player.chips = 0; 
        return 0;
        break;
    default:
        std::cout << "Please enter 0-2 for options\n";
        return 0;
        break;
    }

    return 1;
}

//Main game loop
int main()
{
    std::srand(std::time(0));
    Deck d1 = Deck();
    std::vector<Hand> players;      //11 is the dealer
    Hand dealer;    //Shared cards


    //Up to ten players
    int playerCount;
    std::cout << "How many players are playing (1-10): ";
    std::cin >> playerCount;
    dealCards(playerCount, players, d1);

    //Erase dealer from players
    dealer = players.back();
    players.pop_back();
    
    //Input handling
    std::cout << "\n";
    while (!dealer.unknownCards.empty()) {
        if (!handleInput(players.front(), dealer)) {
            std::cout << "Game over, current balance left " << players.front().moneyLeft << "\n";
            return 0;
        }
        dealer.knownCards.push_back(dealer.unknownCards.back());
        dealer.unknownCards.pop_back();
    }
    std::cout << "\nGame over determining winner... \n";
    determineWinner(players, dealer);

    
    

    /*
    for (Card card : players[check - 1].knownCards)
        std::cout << card;
    */
 
}

