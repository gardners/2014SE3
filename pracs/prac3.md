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

Programming sushi-train style: many small bites.
------------------------------------------------

If you would like to use it, a skeleton (in C) you can use to get started is at: 

https://github.com/gardners/se3prac3 

Fork and clone this repository so that you can use it as the basis for your work.  To compile this project, type `make`, and if that succeeds, to run the game type `./2048` to run the `2048` executable from your current working directory.

I will also include a compiled executable called `sample2048` in that repository that you can use for reference to understand the functionality I will eventually expect from your programme.

If you prefer to work in another programming language, such as Java, you can create a project in whatever language you like, and work there.  However, you must still create a git repository visible on github to manage your source code and provide visibility for me into you work for marking, and be aware that the assistance I will be providing will be focussed on the C skeleton.

In either case, take a look through the commit history for the skeleton I have provided. You will see that each commit contains a very small amount of work, sometimes just a single line added or changed, or a small routine written.  Often the commits are made only a minute or two apart.  This is a good way to work.  Each change can be easily understood, tested and examined.  It also makes it easy for you to backtrack through your changes when (not if) you make a mistake.

To receive today's checkpoint you will need to make at least ten meangingful, and ideally fairly small, and well described commits to your repository.  It doesn't matter at this stage how well your program works -- what is being assessed is your ability to write software making good use of source control to "checkpoint" your work at each step.

Decomposing the problem
-----------------------

This approach is much easier if you start to decompose the problem into the smallest possible units.  

For example, you know that you will need to tilt the grid.  Tilting the grid is the same in all directions, so a single tilt routine that tilts in one particular direction is better than multiple ones for each direction.  Pair this single routine with routines to rotate the grid before and after the tilt, and you will have a much simpler solution.  

In turn, the tilting of the entire board is the tilting of each of its rows or slices.  Write a routine that is passed a single slice of the board, and implements the tilting logic on that.

Writing easily testable functions
---------------------------------

Take care to pass everything that each routine needs in as arguments, instead of relying on any global state.  This will mean that your routines are discrete, and if fed with appropriate input, can be tested independently of the entire program, which will support our goal of writing unit tests (tests of each routine) for your program.  This will allow you to fix bugs easily and early, greatly accelerating your speed of development.

Test-Driven Development
-----------------------

In fact, the best approach is to start with a small part of the program, such as tilting the board in a single direction, decompose the problem into simple parts that you believe can be written in 30 lines or less, which in this case will primarily be tilting a single row of the board, and then write a test function that passes all sorts of input into the function so that you can immediately test it.  Then you will be able to quickly identify and fix any problems with those routines, and proceed to assembling them into the next higher-level abstraction with confidence.  Using this approach I was able to write a sample solution in about 2 hours, including writing the tests. 
You have a skeleton and the information above to give you a head start, so it is possible that some of you will be able to finish the entire program during the prac, and certainly by next week.

Writing a test driver
---------------------

From a practical perspective, you will want to have a separate build target that includes the test driver.  Such a target has been setup in the build process of the skeleton for you in `test.c`.  Look through that file, and you will see that I have created a layered approach that allows vectors to the tilt left function to be easily passed in, and compared with expected output, and have any mismatches reported.  

Running `make test` will rebuild the test driver, and `./test` will run it.  Initially you will see output like:

    Empty list is empty after shift - PASSED.
    Value on left stays on left after shift - PASSED.
    Value on right shifts to left edge after shift - FAILED: {0,0,0,1} became {0,0,0,1} instead of {1,0,0,0}
    Value in middle shifts to left edge after shift - FAILED: {0,0,1,0} became {0,0,1,0} instead of {1,0,0,0}
    Distinct values don't combine - PASSED.
    Combinations don't cascade - FAILED: {1,1,1,1} became {1,1,1,1} instead of {2,2,0,0}
    Tilting {0,0,1,1} left yields {2,0,0,0} - FAILED: {0,0,1,1} became {0,0,1,1} instead of {2,0,0,0}
    Tilting {4,0,1,1} left yields {4,2,0,0} - FAILED: {4,0,1,1} became {4,0,1,1} instead of {4,2,0,0}
    Tilting {2,0,1,1} left yields {2,2,0,0} - FAILED: {2,0,1,1} became {2,0,1,1} instead of {2,2,0,0}

Running the tests regularly will allow you to focus on the next nibble of functionality to attack, as well as confirm that your progressive changes don't cause other tests to fail.  For example, from the output above, it probably makes sense to next work on sliding all non-zero values as far left as possible, and see if the third and fourth tests can be made to pass.

Where to start
--------------

The tests I have given you in the skeleton that fail are for the `tilt_line_left()` function in `tilt.c`.  So you will want to start implementing that function.

For those not familiar with C, the values passed into this function throught the `int *list` argument are available as `list[0]`, `list[1]` and so on. So to check if the first item is zero, you would use something like `if (list[0]==0) { /* leftmost value is zero */ }`

Summary
-------

So, to recap some of the above:

  * Starting with the board tilting, decompose the problem as far as practicable.
  * Write tests before writing the routine under test, beginning at the lowest level, then write the matching routines, fixing bugs until all of your tests pass.
  * Compose the next higher level functions from the lower level functions that you have just written and tested.
  * Repeat recursively until you have all the functionality you require.
  * Start with writing a routine to tilt a row of the board to the left, writing test cases for the various situations.

Now plough on ahead and write code to try to implement this game!  

To get this checkpoint.
-----------------------

To receive this checkpoint you will need to make at least ten meangingful, and ideally fairly small, and well described commits to your repository.  It doesn't matter at this stage how well your program works -- what is being assessed is your ability to write software making good use of source control to "checkpoint" your work at each step, and that you write tests before you write the body of the functions.  Commits to code that preceed the associated test will not count.

Then update the grades file for prac3 in your fork of the 2014SE3 repository, and include a link to the github URL of your repository in the comment to the pull request so that I can examine your commit log.

Checkpoint four will be a continuation of this test-driven development, i.e., the writing of tests before the functions.  The milestone will be having substantially all of the required functionality of the 2048 game complete.
