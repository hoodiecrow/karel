# karel
An implementation of the classic Karel the Robot educational program, based on clox from the book "Crafting Interpreters" by Robert Nystrom.

The original Karel used a programming language vaguely similar to Pascal for programming the robot. This implementation uses Lox instead, a small, dynamic language. The bytecode interpreter for Lox, clox, serves as the engine for this implementation of Karel.
The version I am using here uses code from up to and including chapter 24 of the book. That is, it doesn't have closures, garbage collection, or OOP features.

## The world
The functions `world`, `home`, `robot`, `beepers`, and `wall` have been added to set up the world. They are defined using the native function interface (such as it is).
- `world(x, y)` sets the size of the world to _x_ avenues and _y_ streets; neither can exceed 11
- `home(x, y, d)` sets the location and direction the robot is supposed to end up in to _x_ avenue, _y_ street, and _d_ direction (if not set, the robot can shut down anywhere)
- `robot(x, y, d, n)` places the robot at _x_ avenue, _y_ street, facing direction _d_ (`EAST` (0), `NORTH` (1), `WEST` (2), or `SOUTH` (3)), and carrying _n_ beepers
- `beepers(x, y, n)` places _n_ beepers at _x_ avenue, _y_ street
- `wall(x, y, d)` places a wall near _x_ avenue, _y_ street, blocking the way in direction _d_ (and also from the opposite direction)

The world karel moves in is displayed as ncurses character graphics.

## Primitive instructions
Besides the primitives of the Lox language, I have added the following to control the robot:
- `move` -- advance one corner
- `left` -- change facing 90° to the left
- `put` -- put a beeper on the corner
- `get` -- remove a beeper from the corner
- `color(n)` -- color the corner with color pair #_n_
- `done` -- shut down the robot, finishing the task

I have also added an extra iteration statement:
- `repeat(n) statement` -- repeat a statement _n_ times

## Extending the vocabulary
New pseudo-instructions, such as 'right', can be added by creating Lox functions. The grammar of Lox demands that they are executed as a function call, using the `()` operator. The body of a Lox function is always a block, so there is no limitation as in original Karel where the new instruction has a single instruction in its body.

## Conditional statements
The original Karel `IF condition THEN statement` is replaced by the Lox `if (condition) statement` statement.

### Conditions
- `facing(dir)` is true if the robot is facing in the given direction
- `seeWall(facing)` is true if there is a wall in the `FRONT` (0), `LEFT` (1), or `RIGHT` (-1) facing
- `seeColor(number)` is true if the current corner is colored with color number _number_
- `hearBeepers()` is true if there is at least one beeper at the corner where the robot is located
- `carryBeepers()` is true if there is at least one beepers in the robot's beeper bag

## Iterative statements

### Calling native functions
There are some problems with native functions, e.g. how runtime errors in one break the interpreter and cause a segfault, leaving curses unterminated. I need to propagate the error to the run function and return from that with a runtime error result. 
I could scrap the native function method and compile functions like `world` to bytecodes to run entirely inside the VM, but for now I'm trying out the option of adding a special ERR value and modifying `callValue` to examine the result of the native function before pushing it on the stack. If the return value is the ERR value, `callValue` returns false, causing `run` to end with an RTE. If the return value is any other value, it is pushed on the stack and `callValue` returns normally.

### Evaluating the outcome
This part is still being planned. One possible way is to divide the script into `initial`, `task`, and `final` sections, with beepers() and colors() (and possibly home()) calls in the `final` section setting up checks against the state of the world after performing the task. Another way is to add cases that demonstrate presence or absence of beepers, or the same for color.

## Acknowledgements
clox belongs to Robert Nystrom. Karel the Robot was designed by Richard E. Pattis. 

## Links
- https://en.wikipedia.org/wiki/Karel_(programming_language)
- https://craftinginterpreters.com/ -- home page of the book "Crafting Interpreters"
- https://ics.uci.edu/~pattis/ -- home page of Richard Pattis
