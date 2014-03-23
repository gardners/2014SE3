Practical 3 - Where we begin to try to write quality, testable software
=======================================================================

The focus of this and the next few pracs is on writing quality, testable software using good practices.  The process matters more than the result.

The 2,048 Game
--------------

To keep things simple, the program you will write is a simple (but addictive) game called 2048. You can play an online version at: http://gabrielecirulli.github.io/2048/

Your version will just be a simple text-mode implementation.

The basic premise of the game is that you tilt the board left, right, up or down to move numbered tile together.  Any numbers that match, are combined into a single tile with the sum of the values of the two original tiles.  Since only pairs of tiles combine, the values will always be powers of two.  Each time you tilt the board, one new tile with value 2 or 4 is added.  The challenge is to work up to having a tile with 2,048 on it, before the board fills and no move is possible.  The score is calculated as the sum of the number on all tiles that have been combined.  If all that sounds complicated, try the on-line version of the game out for a few minutes, and it will become much clearer.  That's right, you have to play a game as part of this prac. Regrettably for you, you don't get to choose which game.

Do not write any code (yet)
---------------------------

As you begin to think about implementing this game, I want you to be thinking about how to go about it so that you can be sure that the software that you write works, works completely, and doesn't do anything that it shouldn't do.  It's a bit like in court where they get you to swear to tell the truth, the whole truth and nothing but the truth.  You should be aiming to write software the performs to specification, the whole specification, and nothing but the specification, and where you can present strong evidence in support of your claims.

We are also going to follow a development process more based on agile software development, rather than the waterfall model or some other monolithic approach to software development.

Writing (un)testable code
-------------------------

Now ignore what I just said, and just dive in and start work on this task.  I'll provide you with a skeleton for handling the keyboard input (if it isn't linked here, then poke me, and I will put it here, it's just that I haven't written it yet).  The only requirement at this time, is that you create a git repository, and do the development under source control.  I'll make a repository on github that you can clone to use as your starting point.



-------------------------

A primary consideration should be how you are going to test the resulting software.

If

