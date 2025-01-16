// BBCBlackJackTask.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <string> // For std::stoi
#include <sstream> // Required for std::ostringstream
#include <iomanip> // Required for std::setw

class Card {   //creating object to store different types of cards
public:
    int value;  //this variable represents how many points a card is worth
    std::string name;

    Card(int val, const std::string& n) : value(val), name(n) {}
};

class Deck {   //An object to store the cards that a user has in their deck
private:
    std::vector<Card> cards;   //Vector data type is used as there will be multiple card objects used with potential to change
    std::mt19937 rng;  //Generating a random number

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

    // Helper function to generate ASCII art for a card
    std::string getCardArt(const Card& card) const {
        std::ostringstream oss;

        // Format card name to fit within 2 characters
        std::string displayName = card.name;
        if (card.name.length() > 2) {
            displayName = card.name.substr(0, 2);
        }

        // Build card art
        oss << "+-------+\n";
        oss << "| " << std::setw(2) << displayName << "    |\n";
        oss << "|       |\n";
        oss << "|       |\n";
        oss << "|    " << std::setw(2) << displayName << " |\n";
        oss << "+-------+";

        return oss.str();
    }

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
        // Display each card using ASCII art
        std::cout << "Your hand:\n";
        for (const auto& card : cards) {
            std::cout << getCardArt(card) << "\n";
        }
        std::cout << "Total Score: " << calculateScore() << "\n";
    }

    bool isBust() const {
        return calculateScore() > 21;
    }
};

void testBlackjack() {
    const int numPlayers = 3;  // Number of players - user input will play as player 1, players 2 and 3 are randomised
    Deck deck;

    // Initialize hands for each player
    std::vector<Hand> players(numPlayers);

    // Deal initial cards to each player
    for (int i = 0; i < numPlayers; ++i) {
        players[i].addCard(deck.deal());
        players[i].addCard(deck.deal());
    }

    // Player turns
    for (int i = 0; i < numPlayers; ++i) {
        std::cout << "\nPlayer " << i + 1 << "'s turn:\n";
        players[i].display();

        bool playerDone = false;
        while (!playerDone && !players[i].isBust()) {
            std::cout << "Hit (H) or Stand (S)? ";
            char choice;
            std::cin >> choice;

            if (choice == 'H' || choice == 'h') {
                players[i].addCard(deck.deal());
                std::cout << "After hit:\n";
                players[i].display();
            }
            else if (choice == 'S' || choice == 's') {
                playerDone = true;
            }
            else {
                std::cout << "Invalid choice. Please enter 'H' or 'S'.\n";
            }
        }

        if (players[i].isBust()) {
            std::cout << "Player " << i + 1 << " is bust!\n";
        }
        else {
            std::cout << "Player " << i + 1 << " stands with a score of " << players[i].calculateScore() << "\n";
        }
    }

    // Determine the winner
    int highestScore = 0;
    int winner = -1;
    for (int i = 0; i < numPlayers; ++i) {
        int score = players[i].calculateScore();
        if (!players[i].isBust() && score > highestScore) {
            highestScore = score;
            winner = i;
        }
    }

    std::cout << "\nGame Over!\n";
    if (winner != -1) {
        std::cout << "Player " << winner + 1 << " wins with a score of " << highestScore << "!\n";
    }
    else {
        std::cout << "All players busted. No winner.\n";
    }
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
