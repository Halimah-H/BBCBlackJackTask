// BBCBlackJackTask.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <string> // For std::stoi
#include <iomanip> // For formatting output

class Card {   //creating object to store different types of cards
public:
    int value;  //this variable represents how many points a card is worth
    std::string name;

    Card(int val, const std::string& n) : value(val), name(n) {}
};

class Deck {   //An object to store the cards that a user has in their deck
private:
    std::vector<Card> cards;   //Vector data type is used as there will be multiple card objects used with potential to change
    std::mt19937 rng;

public:
    Deck() {
        rng = std::mt19937(static_cast<unsigned int>(std::time(nullptr)));
        std::vector<std::string> faces = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };
        for (const auto& face : faces) {
            int value = (face == "Jack" || face == "Queen" || face == "King") ? 10 : (face == "Ace" ? 11 : std::stoi(face));
            for (int i = 0; i < 4; ++i) {
                cards.emplace_back(value, face);
            }
        }
        shuffle();
    }

    void shuffle() {
        std::shuffle(cards.begin(), cards.end(), rng);
    }

    Card deal() {
        if (cards.empty()) {
            throw std::runtime_error("The deck is empty");
        }
        Card card = cards.back();
        cards.pop_back();
        return card;
    }
};

class Hand {
private:
    std::vector<Card> cards;


public:
    void addCard(const Card& card) {
        cards.push_back(card);
    }

    int calculateScore() const {
        int score = 0;
        int aces = 0;
        for (const auto& card : cards) {
            score += card.value;
            if (card.name == "Ace") {
                ++aces;
            }
        }
        while (score > 21 && aces > 0) {
            score -= 10; // Count Ace as 1 instead of 11
            --aces;
        }
        return score;
    }

    void display() const {
        for (const auto& card : cards) {
            std::cout << card.name << " ";
        }
        std::cout << "(Score: " << calculateScore() << ")\n";
    }

    bool isBust() const {
        return calculateScore() > 21;
    }
};

void testBlackjack() {
    Deck deck;

    // Scenario 1: Initial hand has two cards
    Hand playerHand;
    playerHand.addCard(deck.deal());
    playerHand.addCard(deck.deal());
    std::cout << "Initial hand: ";
    playerHand.display();

    // Scenario 2: Player hits and receives a card
    playerHand.addCard(deck.deal());
    std::cout << "After hit: ";
    playerHand.display();

    // Scenario 3: Player stands
    std::cout << "After stand: ";
    playerHand.display();

    // Scenario 4: Valid hand under 21
    if (!playerHand.isBust()) {
        std::cout << "Hand is valid\n";
    }

    // Scenario 5: Bust if over 21
    while (!playerHand.isBust()) {
        playerHand.addCard(deck.deal());
    }
    std::cout << "After bust: ";
    playerHand.display();
    if (playerHand.isBust()) {
        std::cout << "Hand is bust\n";
    }

    // Scenario 6: Score evaluation with King and Ace
    Hand hand1;
    hand1.addCard(Card(10, "King"));
    hand1.addCard(Card(11, "Ace"));
    std::cout << "King and Ace: ";
    hand1.display();

    // Scenario 7: Score evaluation with King, Queen, and Ace
    Hand hand2;
    hand2.addCard(Card(10, "King"));
    hand2.addCard(Card(10, "Queen"));
    hand2.addCard(Card(11, "Ace"));
    std::cout << "King, Queen, and Ace: ";
    hand2.display();

    // Scenario 8: Score evaluation with two Aces and a Nine
    Hand hand3;
    hand3.addCard(Card(11, "Ace"));
    hand3.addCard(Card(11, "Ace"));
    hand3.addCard(Card(9, "9"));
    std::cout << "Two Aces and a Nine: ";
    hand3.display();
}

int main()
{
    try {
        testBlackjack();   //calling previous function to run the BlackJack simuation
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";  //error checking in case the function is not able to be called
    }
    return 0;
}
