Whitebox Testing
================

Whitebox text one of the following:
  * Your 2048 game
  * A classmate's 2048 game (fork their git repository and work on that).
  * Some open-source software.

Submit an issue to the 2014S3 repository with a URL to the commit log for the
repository where you have written the white-box tests.  

You will need to write 10 tests to obtain the checkpoint.

Whitebox tests concentrate on exercising as many code paths and lines of the
source code as possible.  This is in contrast to the Unit Tests you wrote as
part of our Test Driven Design work earlier in the course. 

Those tests were
blackbox tests, i.e., written without reference to the source code (in that
case the source code didn't even exist), and as such were Black Box tests.

Another difference is that Black Box tests are focused on the functionality
and specification, rather than verifying correct behaviour of each line of
code in the solution.

In other words, Black Box tests are functional, while White Box tests are
structural.

There are number of ways you can attack White Box testing, some of which we
discussed in the lectuer today.  You might want to watch the recording of
this week's lecture to get you thinking about these (especially since only
a few of you were able to attend today's lecture).

A good guide is that every if or other branch or decision point in a function
requires a test.  For complex branch expressions you may need to decompose them
to identify multiple tests to ensure all decision paths are exercised.  For
example, if two variables in the expression of an if statement can affect the
result, then two tests will be needed (and possibly more).

If we have the statement:

  if ((a>7)||(b<5))

Two tests will be needed, one with a>7 and one with b<5 to test the success path.
A third test will be needed with a<=7 and b>=5 to test the alternate path.

A further good guide is wherever there are boundary conditions, you should also test
either side of the boundary condition.  For the above example, this might mean the
following tests:

  a=6, b=10
  a=7, b=10
  a=8, b=10
  b=4, a=1
  b=5, a=1
  b=6, a=1

This helps to pick up errors where the wrong relative expression has been used, or the
logic in the expression suffers from some other flaw.

Of course, the tests should be written with regard to the expected correct behaviour of
the function, not merely making sure the function does what the function does!

Where you have multiple branch points, you will end up with many variables or conditions
to consider, and you may need to reason more deeply in order to work out which input
values will exercise all possible lines and code paths.

This deep inspection of the source code is valuable in itself, and often turns up many
bugs (for which you can then write tests that fail to hand back to the developer!). It
can also help identify code that never gets run, or where various conditions are not
properly accommodated.

These are just a few ideas of how you can go about white box testing.


