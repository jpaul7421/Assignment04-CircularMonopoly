# Circular Linked List Monopoly

## Overview

This program builds a simplified Monopoly board using a circular linked list. Each board space is stored in a dynamically allocated `Property` node. Every node stores a name, cost, rent amount, owner, special-space flag, and a pointer to the next node. The last node points back to the first node so players wrap around the board naturally.

The program also includes a linked list of Chance cards. Two players move around the board for up to ten turns. Players collect $200 when passing GO, may purchase unowned properties, pay rent when landing on another player's property, draw Chance cards, and can be sent to Jail. If a player cannot pay rent, that player goes bankrupt.

Auctions, houses, hotels, trading, and mortgages are outside the scope of this simplified assignment.

## Linked-list operations

The `CircularBoard` class supports:

- Adding a property to the end of the circular list
- Searching for a property by name
- Removing a property by name
- Printing every board space once
- Accessing the first node for player movement

The `ChanceDeck` class uses a separate linked list so a random Chance card can be selected.

## Complexity

Let `n` be the number of board spaces and `k` be the number of spaces moved.

| Operation | Time | Reason |
|---|---:|---|
| Add property | O(1) | The board keeps a tail pointer. |
| Search | O(n) | The list may need to visit every node. |
| Remove | O(n) | The target and its previous node may need to be found. |
| Print board | O(n) | Every board node is visited once. |
| Move player | O(k) | The player follows `next` pointers for each space. |
| Purchase or rent | O(1) | The player already has a pointer to the current space. |

The board uses O(n) space for its property nodes. The Chance deck uses O(c) space for its card nodes, where `c` is the number of cards.

## Tests and expected results

The program performs these representative checks before starting the game:

1. A temporary property is added and successfully found with `search`.
2. The temporary property is removed with `remove`.
3. Searching for that property afterward returns `not found`.
4. The complete circular board is printed once.
5. The game simulates ten alternating turns.
6. Movement can wrap from the final board space back to GO.
7. Players cannot purchase a property that is already owned.
8. Players collect salary when passing GO.
9. Chance cards can change money or send a player to Jail.
10. A player who cannot pay rent is declared bankrupt.

The exact dice rolls and Chance cards vary because the program uses random values, but the output should always show the same operations and rules.

## How to run

Compile with C++17 or later:

```bash
g++ -std=c++17 MonopolyConsole.cpp -o MonopolyConsole
./MonopolyConsole
```

The program does not use `std::list`.
