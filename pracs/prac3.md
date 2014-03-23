Practical 3 - Where we begin to try to write quality, testable software
=======================================================================

The focus of this and the next few pracs is on writing quality, testable software using good practices.  The process matters more than the result.

The 2,048 Game
--------------

To keep things simple, the program you will write is a simple (but addictive) game called 2048. You can play an online version at gabrielecirulli.github.io/2048/

The basic premise of the game is that you tilt the board left, right, up or down to move numbered tile together.  Any numbers that match, are combined into a single tile with the sum of the values of the two original tiles.  Since only pairs of tiles combine, the values will always be powers of two.  Each time you tilt the board, one new tile with value 2 or 4 is added.  The challenge is to work up to having a tile with 2,048 on it, before the board fills and no move is possible.  The score is calculated as the sum of the number on all tiles that have been combined.  If all that sounds complicated, try the on-line version of the game out for a few minutes, and it will become much clearer.

