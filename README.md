# β-

β- is a chess bot, written entrierly algorithmically, aiming to achieve a [Chess.com](https://www.chess.com/home) elo of over 2000.

Stockfish rates β- with an elo of ~1750 at a move depth of 7. https://www.chessigma.com/games/4820e560-b9c0-43e1-b37d-ac2693c67f2d as of commit bb4bbd9.

### Rules

This chess bot was written through CUSEC's (Clarkson University Software Engineering Club) Chess Competition. The rules are below, other standard competition rules and practices are followed

1. It cannot be a already made chess bot (StockFish, Torch, etc.)
2. It can be implemented in any language, though must be able to do the 4 below, named appropriately.
  - Give Board State in [Forysth-Edwards Notation](https://en.wikipedia.org/wiki/Forsyth–Edwards_Notation).
  - Get Board State in [Forysth-Edwards Notation](https://en.wikipedia.org/wiki/Forsyth–Edwards_Notation).
  - Provide a move in [Long Algebraic Notation](https://en.wikipedia.org/wiki/Algebraic_notation_(chess)#Long_algebraic_notation).
  - Take a move in [Long Algebraic Notation](https://en.wikipedia.org/wiki/Algebraic_notation_(chess)#Long_algebraic_notation).
3. It must use [UCI](https://en.wikipedia.org/wiki/Universal_Chess_Interface).

### Actions

There is no real checking on user input. Bad inputs will lead to unintended behavior. The computer does not care whose side it is and will make a move regardless, it is up to the user to ensure the input is correct.

***Note: I recommend change the macros `CHESS_SEARCH_THREADS` and `SEARCH_DEPTH` in `src/utils/chess.h` to different values. Set `CHESS_SEARCH_THREADS` to the amount of cores you have at your disposal, and `SEARCH_DEPTH` to a value between 5-10, depending on how long you want to wait (~30-60s per move at `SEARCH_DEPTH = 7` and `CHESS_SEARCH_THREADS = 24`).***

|Action|Letter|Description|
|:--:|:--:|:---|
|Exit|q|Exits the program.|
|Start Game|s|Starts a new game with the default board state.|
|End Game|e|Ends the current game, resetting the board. Printing all moves that were made, the ending board state, and the winner (if any).|
|Give Move|g|Allows user to give a move to be completed. Moves should be input like "a1a2" or "a7a8q" if promotion is available.|
|Get Move|t|Gets a move that will be completed.|
|Give Get|m|Runs Give Move then Get Move.|
|Valid Moves|v|Gets the list of valid moves.|
|Give Board|p|Allows user to give a new board, this will reset the game.|
|Get Board|b|Gets the current board state.|
|Get Score|i|Gets the current boards score.|
|Loop|l|Loops Get move until a ending position is reached, takes a long time.|

### Viewing the Game

`convert.py` is a python script that will convert the output from End Game into technically valid PGN notation, as to be input into [Chess.com](https://www.chess.com/analysis). This file was written by [ChatGPT](https://chatgpt.com/), and requires manually input of the game notation in the code.
