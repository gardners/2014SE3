Pair Programming
================

This prac is where we explore pair programming.

See below for the resources we used to explore pair programming
during the lecture:

  http://www.slideshare.net/rogercafe/pair-programming-presentation
  http://www.slideshare.net/MrSMAk/extreme-pair-programming-slides-ppt
  http://www.slideshare.net/thekua/pair-programming-good-bad-and-ugly-11289903
  http://en.wikipedia.org/wiki/Pair_programming

These sources reflect the diverse views and practices around Pair Programming
and the lively ongoing debate about the net value (or liability) of pair programming.

Note the practices which are encouraged and discouraged by these sources.  At a minimum in this
prac, you should:
  Swap developer/navigator roles at least every 10 minutes.
  The navigator should be thinking about big-picture issues, like how do we design this to accommodate the next few devious dates?
  The navigator and developer should be conversing fairly continuously.
  The navigator should follow the 10 second rule before jumping in to point out syntax and other trivial errors.

The purpose of this prac is for you to try pair programming, and then reflect on your experience.

The subject exercise
====================

Your task to perform using Pair Programming is to write a robust time and date parsing routine, 
using any programming language of your choice.  

You are to begin by trying to parse the more common date formats, and then as time permits,
to progress towards being able to handle increasingly informal, ambiguous, vague and esoteric
representations.  

The returned result should be a normalised
time and date expressed as seconds since 1970 UTC, i.e., a standard UNIX epoch time.  This
also allows you to make use of whatever date and time parsing routines you can find.  However,
be assured that it is extremely unlikely that you will find existing routines to reliabily parse
many of the examples provided.

For some formats and values, it may not be possible to return a single precise
date. You may elect to return a set of dates with probabilities of correctness.

For any date you cannot parse, you should return a syntax error.

For any date that can be parsed, but cannot correspond to a real time should return an error.

You should spend only a couple of hours working on this -- it is just to give you material to work
with as you explore pair programming.

Assessment
==========

You will work in groups of 2 or 3, most likely with members of the US study group who will be joining us for this prac.

Although you will work with others, your submission must be your own.

After having experienced a couple of hours of pair programming, reflect on your own experience, covering such matters as:
  What benefits did you experience.  Were you faster? was your code more correct?
  What difficulties did you experience?
  What would worked well, and what would you change next time?
  Would you use pair programming in the future given the chance?

Submit a ~500 word summary of your reflection as the text of a pull request to add your FAN to the prac8 grade file.
