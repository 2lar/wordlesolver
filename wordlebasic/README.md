This is a basic wordle solver.
Takes in the word and considers possibilities based on the progression.

this means that the advisor doesn't neccessarily make the BEST choice but rather offers the possible choices;
  this is to imitate the naive approach to solving wordle.
  
implemenetations of this advisor can be used to find a more effective algorithm
- using the letter frequency to an advantage in the next advisor.
- instead of simply nailing down the possible candidates, could try to find as many greens/yellows in second try
  while also avoiding letters used in first attempt to construct a more solidified answer. This might rail the 
  average attempts to about 4, maybe even 3. but this is better than the (wordlebasic) narrowing candidate selection
