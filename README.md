# Connect4 AI
C++ implementation of Connect 4 using Minimax with Alpha-beta pruning.

## Project structure
 ```
connect4-ai/
│
├── src/
│   ├── Board.hpp               # The board  
│   ├── Engine.hpp              # Minimax implementation       
│   ├── KillerMoves.hpp         # Utility class to work with killer moves               
│
├── main.cpp                    # main
├── LICENSE
└── README.md            
 ```

## Compilation and Execution
Compilation:
</br>
</br>
 ```$ g++ main.cpp -o <name>```
</br>
</br>
Execution:
</br>
</br>
  ```$ ./<name> <depth>```
</br>
</br>
Where ```<depth>``` specifies the depth for minimax.

## Evaluation Function
To estimate the value of a particular board position, the evaluation function counts how many pieces in a row the currentPlayer has in the four directions (horizontal, vertical, diagonal, antidiagonal) and gives a score based on that.
Then it subtracts the score of the opponent (calculated in the same way) and returns the remaining value.
</br>
```
| Player pieces | Opponent Pieces | Empty Spots | Score             |
|---------------|-----------------|-------------|-------------------|
| 4             | 0               | 0           | 100000 + depth    |
| 3             | 0               | 1           | 1000              |
| 2             | 0               | 2           | 100               |
| 0             | 2               | 2           | -100              |
| 0             | 3               | 1           | -2000             |
| 0             | 4               | 0           | -(100000 + depth) |
```


 
