# β-

β- is a chess bot, written entrierly algorithmically, aiming to achieve a [Chess.com](https://www.chess.com/home) elo of over 2000.

### Method

**All of this is guessing right now, just to have a plan**

The chess board will be represented by a 8x8 array of pieces. These pieces reside inside of an `enum class`. Moves will be represented by [Long Algebraic Notation](https://en.wikipedia.org/wiki/Algebraic_notation_(chess)#Long_algebraic_notation). Then using [Alpha-Beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) to successfully trim the resulting tree that is made. Finally implementing [IDDFS](https://en.wikipedia.org/wiki/Iterative_deepening_depth-first_search) to further improve efficiency and reliability will be added.

### Steps

- Design a Move class in conjucntion with the Game class to represent moves
- Design a Game class that stores all relevant game information.
 - A locational struct will be used as to aid in move representation
- Design an algorithm to determine all possible next moves
- Design an algorithm to determine legal next moves, if zero then checkmate
- Add the actual "bot" part of the chess bot
