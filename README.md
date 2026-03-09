# β-

β- is a chess bot, written entrierly algorithmically, aiming to achieve a [Chess.com](https://www.chess.com/home) elo of over 2000.

### Rules

This chess bot was written through CUSEC's ( Clarkson University Software Engineering Club ) Chess Competition. The rules are below, other standard competition rules and practices are followed

1. It cannot be a already made chess bot (StockFish, Torch, etc.)
2. It can be implemented in any language, though must be able to do the 4 below, named appropriately.
  - Provide board state in Forysth-Edwards Notation https://en.wikipedia.org/wiki/Forsyth–Edwards_Notation
  - Take board state in Forysth-Edwards Notation
  - Provide a move
  - Take a move
3. It must use https://en.wikipedia.org/wiki/Universal_Chess_Interface for move notation

### Actions

There is no reall checking on user input. Bad inputs will lead to unintended behavior. The computer does not care whose side it is and will make a move regardless, it is up to the user to ensure the input is correct.

|Action|Letter|Description|
|:--:|:--:|:---|
|Exit|q|Exits the program.|
|Start Game|s|Starts a new game with the default board state.|
|End Game|e|Ends the current game, reseting the board. Printing all moves that were made, the ending board state, and the winner (if any).|
|Give Move|g|Allows user to give a move to be completed.|
|Get Move|t|Gets a move that will be completed.|
|Give Get|m|Runs Give Move then Get Move.|
|Give Board|p|Allows user to give a new board, this will reset the game.|
|Get Board|b|Gets the current board state.|
