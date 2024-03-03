# karel
An implementation of the classic Karel the Robot educational program, based on clox from the book "Crafting Interpreters" by Robert Nystrom.

## clox
Lox is a small, dynamic, object-oriented language, and clox is a bytecode interpreter for Lox. The version I am using here uses code from up to and including chapter 24 of the book. That is, it doesn't have closures, garbage collection, or actual OOP features.

## karel
I have added seven statement types to clox to make controlling the robot possible.

- `move` -- advance one corner
- `left` -- change facing 90° to the left
- `put` -- put a beeper on the corner
- `get` -- remove a beeper from the corner
- `color(n)` -- color the corner with color pair #_n_
- `repeat(n) statement` -- repeat a statement _n_ times
- `done` -- shut down the robot

The arena or world karel moves in will be displayed as ncurses character graphics.

### Initialization
The functions `world`, `home`, `robot`, `beepers`, and `wall` have been added to initialize the world parts. They are defined using the native function interface (such as it is).
- `world(x, y)` sets the size of the world to _x_ avenues and _y_ streets; neither can exceed 16
- `home(x, y, d)` sets the location and direction the robot is supposed to end up in to _x_ avenue, _y_ street, and _d_ direction (if not set, the robot can shut down anywhere)
- `robot(x, y, d, n)` sets the robot at _x_ avenue, _y_ street, facing direction _d_ (`EAST` (0), `NORTH` (1), `WEST` (2), or `SOUTH` (3)), and carrying _n_ beepers
- `beepers(x, y, n)` sets down _n_ beepers at _x_ avenue, _y_ street
- `wall(x, y, d)` sets down a wall near _x_ avenue, _y_ street, blocking the way in direction _d_ (and also from the opposite direction)

### Conditions
- `facing(dir)` is true if the robot is facing in the given direction
- `blocked(facing)` is true if there is a wall in the `FRONT` (0), `LEFT` (1), or `RIGHT` (-1) facing
- `beepersAtCorner()` is true if there is at least one beeper at the corner where the robot is located
- `beeperBagEmpty()` is true if there are no beepers in the robot's beeper bag

### Calling native functions
There are some problems with native functions, e.g. how runtime errors in one break the interpreter and cause a segfault, leaving curses unterminated. I need to propagate the error to the run function and return from that with a runtime error result. 
I could scrap the native function method and compile functions like `world` to bytecodes to run entirely inside the VM, but for now I'm trying out the option of adding a special ERR value and modifying `callValue` to examine the result of the native function before pushing it on the stack. If the return value is the ERR value, `callValue` returns false, causing `run` to end with an RTE. If the return value is any other value, it is pushed on the stack and `callValue` returns normally.

## Extending the vocabulary
New pseudo-instructions, such as 'right', can be added by creating Lox functions. The grammar of Lox demands that they are executed as a function call, using the `()` operator. The body of a Lox function is always a block, so there is no limitation as in original Karel where the new instruction has a single instruction in its body.

## Acknowledgements
clox belongs to Robert Nystrom. Karel the Robot was designed by Richard E. Pattis. 

## Links
- https://en.wikipedia.org/wiki/Karel_(programming_language)
- https://craftinginterpreters.com/ -- home page of the book "Crafting Interpreters"
- https://ics.uci.edu/~pattis/ -- home page of Richard Pattis
