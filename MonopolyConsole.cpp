#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

struct Property {
    string name;
    int cost;
    int rent;
    string owner;
    bool special;
    Property* next;

    Property(string propertyName, int propertyCost,
             int propertyRent, bool isSpecial = false) {
        name = propertyName;
        cost = propertyCost;
        rent = propertyRent;
        owner = "Unowned";
        special = isSpecial;
        next = nullptr;
    }
};

class CircularBoard {
private:
    Property* head;
    Property* tail;

public:
    CircularBoard() {
        head = nullptr;
        tail = nullptr;
    }

    void add(string name, int cost, int rent,
             bool special = false) {
        Property* newProperty =
            new Property(name, cost, rent, special);

        if (head == nullptr) {
            head = newProperty;
            tail = newProperty;
            newProperty->next = head;
        } else {
            tail->next = newProperty;
            tail = newProperty;
            tail->next = head;
        }
    }

    Property* search(string name) const {
        if (head == nullptr) {
            return nullptr;
        }

        Property* current = head;

        do {
            if (current->name == name) {
                return current;
            }

            current = current->next;
        } while (current != head);

        return nullptr;
    }

    bool remove(string name) {
        if (head == nullptr) {
            return false;
        }

        Property* current = head;
        Property* previous = tail;

        do {
            if (current->name == name) {
                if (current == head && current == tail) {
                    head = nullptr;
                    tail = nullptr;
                } else {
                    previous->next = current->next;

                    if (current == head) {
                        head = current->next;
                    }

                    if (current == tail) {
                        tail = previous;
                    }

                    tail->next = head;
                }

                delete current;
                return true;
            }

            previous = current;
            current = current->next;
        } while (current != head);

        return false;
    }

    void print() const {
        if (head == nullptr) {
            cout << "The board is empty.\n";
            return;
        }

        Property* current = head;

        do {
            cout << current->name;

            if (!current->special) {
                cout << " | Cost: $" << current->cost
                     << " | Rent: $" << current->rent
                     << " | Owner: " << current->owner;
            }

            cout << '\n';
            current = current->next;
        } while (current != head);
    }

    Property* first() const {
        return head;
    }

    ~CircularBoard() {
        if (head == nullptr) {
            return;
        }

        tail->next = nullptr;
        Property* current = head;

        while (current != nullptr) {
            Property* next = current->next;
            delete current;
            current = next;
        }
    }
};

struct ChanceCard {
    string description;
    int moneyChange;
    bool sendsToJail;
    ChanceCard* next;

    ChanceCard(string cardDescription, int change,
               bool jail) {
        description = cardDescription;
        moneyChange = change;
        sendsToJail = jail;
        next = nullptr;
    }
};

class ChanceDeck {
private:
    ChanceCard* head;
    ChanceCard* tail;
    int cardCount;

public:
    ChanceDeck() {
        head = nullptr;
        tail = nullptr;
        cardCount = 0;
    }

    void add(string description, int moneyChange,
             bool sendsToJail) {
        ChanceCard* newCard =
            new ChanceCard(description,
                           moneyChange,
                           sendsToJail);

        if (head == nullptr) {
            head = newCard;
            tail = newCard;
        } else {
            tail->next = newCard;
            tail = newCard;
        }

        cardCount++;
    }

    ChanceCard* draw() const {
        int selected = rand() % cardCount;
        ChanceCard* current = head;

        for (int i = 0; i < selected; i++) {
            current = current->next;
        }

        return current;
    }

    ~ChanceDeck() {
        ChanceCard* current = head;

        while (current != nullptr) {
            ChanceCard* next = current->next;
            delete current;
            current = next;
        }
    }
};

struct Player {
    string name;
    int money;
    int jailTurns;
    Property* position;

    Player(string playerName, Property* startingPosition) {
        name = playerName;
        money = 1500;
        jailTurns = 0;
        position = startingPosition;
    }
};

int rollDice() {
    int firstDie = rand() % 6 + 1;
    int secondDie = rand() % 6 + 1;
    return firstDie + secondDie;
}

void movePlayer(Player& player, Property* first,
                int spaces) {
    for (int i = 0; i < spaces; i++) {
        if (player.position->next == first) {
            player.money += 200;
            cout << player.name
                 << " passed GO and collected $200.\n";
        }

        player.position = player.position->next;
    }
}

void playTurn(Player& player, Player& opponent,
              Property* first, Property* jail,
              ChanceDeck& chanceDeck) {
    cout << player.name << "'s turn:\n";

    if (player.jailTurns > 0) {
        cout << player.name
             << " is in Jail and skips this turn.\n";
        player.jailTurns--;
        return;
    }

    int roll = rollDice();
    cout << player.name << " rolled " << roll << ".\n";

    movePlayer(player, first, roll);

    cout << player.name << " landed on "
         << player.position->name << ".\n";

    if (!player.position->special) {
        if (player.position->owner == "Unowned") {
            if (player.money >= player.position->cost) {
                player.money -= player.position->cost;
                player.position->owner = player.name;

                cout << player.name << " bought the property for $"
                     << player.position->cost << ".\n";
            } else {
                cout << player.name
                     << " cannot afford the property.\n";
            }
        } else if (player.position->owner == player.name) {
            cout << player.name
                 << " already owns this property.\n";
        } else if (player.position->owner == opponent.name) {
            int rent = player.position->rent;

            if (player.money >= rent) {
                player.money -= rent;
                opponent.money += rent;

                cout << player.name << " paid $"
                     << rent << " in rent.\n";
            } else {
                player.money = 0;
                cout << player.name
                     << " could not pay rent and went bankrupt.\n";
            }
        }
    } else if (player.position->name == "Chance") {
        ChanceCard* card = chanceDeck.draw();

        player.money += card->moneyChange;

        cout << "Chance card: "
             << card->description << '\n';

        if (card->sendsToJail) {
            player.position = jail;
            player.jailTurns = 2;
            cout << player.name
                 << " was sent to Jail.\n";
        }
    } else if (player.position->name == "Go To Jail") {
        player.position = jail;
        player.jailTurns = 2;
        cout << player.name
             << " was sent to Jail.\n";
    } else if (player.position->name == "Jail") {
        cout << player.name
             << " is just visiting Jail.\n";
    }

    cout << "Money remaining: $"
         << player.money << "\n\n";
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    CircularBoard board;

    board.add("GO", 0, 0, true);
    board.add("Mediterranean Avenue", 60, 30);
    board.add("Baltic Avenue", 60, 30);
    board.add("Chance", 0, 0, true);
    board.add("Oriental Avenue", 100, 50);
    board.add("Vermont Avenue", 100, 50);
    board.add("Jail", 0, 0, true);
    board.add("Connecticut Avenue", 120, 60);
    board.add("St. Charles Place", 140, 70);
    board.add("Chance", 0, 0, true);
    board.add("States Avenue", 140, 70);
    board.add("Virginia Avenue", 160, 80);
    board.add("St. James Place", 180, 90);
    board.add("Tennessee Avenue", 180, 90);
    board.add("Go To Jail", 0, 0, true);

    cout << "Testing linked-list operations:\n";
    board.add("Temporary Test Property", 50, 25);

    cout << "Search test: ";
    cout << (board.search("Temporary Test Property")
                 ? "found\n"
                 : "not found\n");

    cout << "Remove test: "
         << (board.remove("Temporary Test Property")
                 ? "removed\n"
                 : "not removed\n");

    cout << "Search after removal: ";
    cout << (board.search("Temporary Test Property")
                 ? "found\n\n"
                 : "not found\n\n");

    cout << "Initial board:\n";
    board.print();
    cout << '\n';

    ChanceDeck chanceDeck;
    chanceDeck.add("Bank error: collect $200", 200, false);
    chanceDeck.add("Pay a repair bill: $100", -100, false);
    chanceDeck.add("Contest prize: collect $150", 150, false);
    chanceDeck.add("Go directly to Jail", 0, true);

    Property* jail = board.search("Jail");
    Player player1("Player 1", board.first());
    Player player2("Player 2", board.first());

    cout << "Starting 10-turn Monopoly simulation:\n\n";

    for (int turn = 1; turn <= 10; turn++) {
        cout << "========== Turn " << turn
             << " ==========\n";

        playTurn(player1, player2, board.first(),
                 jail, chanceDeck);

        if (player1.money > 0) {
            playTurn(player2, player1, board.first(),
                     jail, chanceDeck);
        }

        if (player1.money <= 0 || player2.money <= 0) {
            break;
        }
    }

    cout << "Final board:\n";
    board.print();

    return 0;
}
