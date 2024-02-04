# karel
An implementation of the classic Karel the Robot educational program, based on the clox language infrastructure from the book "Crafting Interpreters" by Robert Nystrom.

## clox
clox is a dynamic, object-oriented, small bytecode-compiled language. The version I am using here uses code from up to and including chapter 24. That is, it doesn't have closures, garbage collection, or actual OOP features.

## karel
I intend to add seven statement types to clox to make controlling the robot possible.

- `move` -- advance one square
- `left` -- change facing 90° to the left
- `put` -- put a beeper on the square
- `get` -- remove a beeper from the square
- `color(n)` -- color the square with color #_n_
- `repeat(n) statement` -- repeat a (possibly compound) statemen _n_ times
- `quit` -- shut down the robot

The arena or world karel moves in will be displayed as ncurses character graphics.

## Acknowledgements
clox belongs to Robert Nystrom. Karel the Robot was designed by Richard E. Pattis. 

## Links
- https://en.wikipedia.org/wiki/Karel_(programming_language)
- https://craftinginterpreters.com/ -- home page of the book "Crafting Interpreters"
- https://ics.uci.edu/~pattis/ -- home page of Richard Pattis
