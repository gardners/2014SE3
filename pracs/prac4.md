Practical 4 - Where we continue to try to write quality, testable software
==========================================================================

Continue working on the task from prac 3, with the goal of completing
the core functionality (see the `sample2048` program that I have added
to the se3prac3 repo.  You may need to run `git pull` in your
`se3prac3` directory to make it appear.

Some help with progressing from line to board based tests and functions
-----------------------------------------------------------------------

If you are not familiar with C, moving to handling 2-dimensional arrays for the board tests might be a bit to get your head around. To make life easier, see the following for a test template for testing a 2x2 board.  The main differences are the board arrangement, and placing and retrieving the input and output values:

    int board_vector_test(int i1,int i2,int i3,int i4,char *msg,
                          int o1,int o2,int o3,int o4, int (*func)(int,int **))
    {
      int **board=alloca(2*sizeof(int*));
      board[0]=alloca(sizeof(int)*2);
      board[1]=alloca(sizeof(int)*2);
      board[0][0]=i1; board[1][0]=i2; board[0][1]=i3; board[1][1]=i4;
    
      if (msg) printf("%s - ",msg); 
      else {
        printf("Board operation on {{%d,%d},{%d,%d}} yields {{%d,%d},{%d,%d}} - ",
                 i1,i2,i3,i4,o1,o2,o3,o4);
      }
      fflush(stdout);
      func(2,board);
      if ((board[0][0]!=o1)||(board[1][0]!=o2)
          ||(board[0][1]!=o3)||(board[1][1]!=o4))
        {
          printf("FAILED: {{%d,%d},{%d,%d}} became {{%d,%d},{%d,%d}} instead of"
                 " {{%d,%d},{%d,%d}}\n",
                 i1,i2,i3,i4,
                 board[0][0],board[1][0],board[0][1],board[1][1],
                 o1,o2,o3,o4);
          return -1;
        } 
      printf("PASSED.\n");
      return 0;
    }

Then calling a test would use something like:

    board_vector_test(1,2,3,4,"Rotate 2x2 board right (+90)",3,1,4,2,board_rotate_90);

This lets you write and run tests for very simple boards first.  
You can then generalised your tests and routines to larger boards, 
such as 3x3, as below:

    int board3_vector_test(int i1,int i2,int i3,int i4,int i5,
                           int i6,int i7,int i8,int i9,char *msg,
                           int o1,int o2,int o3,int o4,int o5,
                           int o6,int o7,int o8,int o9,
                           int (*func)(int,int **))
    {
      int **board=alloca(3*sizeof(int*));
      board[0]=alloca(sizeof(int)*3);
      board[1]=alloca(sizeof(int)*3);
      board[2]=alloca(sizeof(int)*3);
      board[0][0]=i1; board[1][0]=i2; board[2][0]=i3;
      board[0][1]=i4; board[1][1]=i5; board[2][1]=i6;
      board[0][2]=i7; board[1][2]=i8; board[2][2]=i9;
    
      if (msg) printf("%s - ",msg); 
      else {
        printf("Board operation on {{%d,%d,%d},...} yields {{%d,%d,%d}...} - ",
                 i1,i2,i3,o1,o2,o3);
      }
      fflush(stdout);
      func(3,board);
      if (
          (board[0][0]!=o1)||(board[1][0]!=o2)||(board[2][0]!=o3)||
          (board[0][1]!=o4)||(board[1][1]!=o5)||(board[2][1]!=o6)||
          (board[0][2]!=o7)||(board[1][2]!=o8)||(board[2][2]!=o9)
          )
        {
	     printf("FAILED: {{%d,%d,%d},{%d,%d,%d},{%d,%d,%d}} became {{%d,%d,%d},{%d,%d,%d},{%d,%d,%d}} instead of"
                 " {{%d,%d,%d},{%d,%d,%d},{%d,%d,%d}}\n",
                 i1,i2,i3,i4,i5,i6,i7,i8,i9,
                 board[0][0],board[1][0],board[2][0],
                 board[0][1],board[1][1],board[2][1],
                 board[0][2],board[1][2],board[2][2],
                 o1,o2,o3,o4,o5,o6,o7,o8,o9);
          return -1;
        } 
      printf("PASSED.\n");
      return 0;
    }

This should help you write your own similar routine for the 4x4 board required in the game.  Eventually you might have tests that look like:

    board4_vector_test(1,0,0,0,
                         0,0,0,0,
                         0,0,0,0,
                         0,0,0,0,
                         "Tilt board right",
                         0,0,0,1,
                         0,0,0,0,
                         0,0,0,0,
                         0,0,0,0,
                         tilt_board_right);


To get this checkpoint.
-----------------------

To receive this checkpoint you will need to make at least ten *more*
meangingful, and ideally fairly small, and well described commits to
your repository.  You must also get the core functionality working, or
at least have tests covering the missing functionality.

Then update the grades file for prac4 in your fork of the 2014SE3 repository, and include a link to the github URL of your repository in the comment to the pull request so that I can examine your commit log.
