# Checkers!
An open source checkers engine for the console implemented in C++.

## A Few Thoughts
Don't mind the clunky controls, or messy code. This was my first real C++ project and was meant entirely to help me learn. If you end up learning something or finding my code interesting, that's great! If you find any bugs or have suggestions for improvements to my code, that's great too and I'll be glad to hear about it. Overall this was just a fun learning experience to get me using C++ and I thought I might as well put it up on Github for others to see, as Git has been something on my TODO list for quite some time and I think I could really benefit from an online prescence.

## The Program Itself
The program itself is quite simple, implementing the basic rules of checkers, as well as a simple bot to play against. The bot uses a basic Negamax algorothm, and alpha beta pruning, and uses a copy-make method to perform its searches. The bot's evaluation is equally basic; simply counting the pieces on the board and adding up the assigned values (100 for ordinary pieces, 300 for kings), or returned a hard-coded value for a win or loss. 
The move generation is rather clunky, where I prefered the simplicity of using an array of all 64 squares on the board, rather than only the 32 accessible by the pieces, as it helped me conceptualize the movements easier. For input and output, simple methods are defined to convert my derived Chess notation, say a1, to an array index, and a print method is defined on the board object.
