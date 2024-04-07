# karel
An implementation of the classic Karel the Robot educational program, using clox from the book "Crafting Interpreters" by Robert Nystrom as engine.

The original Karel used a programming language vaguely similar to Pascal for programming the robot. This implementation uses Lox instead, a small, dynamic language. The bytecode interpreter for Lox, clox, serves as the engine for this implementation of Karel.
The version I am using here uses code from up to and including chapter 24 of the book. That is, it doesn't have closures, garbage collection, or OOP features.

## The world
The functions `world`, `robot`, `beepers`, and `wall` have been added to set up the world. They are defined using the native function interface (such as it is).
- `world(x, y)` sets the size of the world to _x_ avenues and _y_ streets; neither can exceed 11
- `robot(x, y, d, n)` places the robot at _x_ avenue, _y_ street, facing direction _d_ (`EAST` (0), `NORTH` (1), `WEST` (2), or `SOUTH` (3)), and carrying _n_ beepers
- `beepers(x, y, n)` places _n_ beepers at _x_ avenue, _y_ street
- `wall(x, y, d)` places a wall near _x_ avenue, _y_ street, blocking the way in direction _d_ (and also from the opposite direction)

The world Karel moves in is displayed using ncurses character graphics.

## Primitive instructions
Besides the primitives of the Lox language, I have added the following to control the robot:
- `move` -- advance one corner
- `left` -- change facing 90° to the left
- `put` -- put a beeper on the corner
- `get` -- remove a beeper from the corner
- `color(n)` -- color the corner with color pair #_n_ (0 erases color)
- `done` -- shut down the robot, finishing the task

## Extending the vocabulary
New instructions, such as 'right', can be added by creating Lox functions. 

```
fun right () {
  repeat (3) left;
}
```

The grammar of Lox demands that use of defined instructions be written as a function call, using the `()` operator (e.g. `right();`).  Primitive instructions do not need the `()` operator (e.g. `left;`)

The body of a Lox function is always a block, so there is no limitation as in original Karel where the new instruction has a single instruction in its body.

### Source code structure
The Lox version of Karel does not use `BEGINNING-OF-PROGRAM/END-OF-PROGRAM` or `BEGINNING-OF-EXECUTION/END-OF-EXECUTION` markers. Statements to be executed are placed at the top level in the script, and statements that define new instructions are placed inside Lox functions.

## Conditional statements
The original Karel `IF/THEN` instruction is replaced by the Lox `if (`_condition_`) `_statement_ statement.

### Tests
The tests reflect Karel's artificial senses and what they detect. Karel has a compass that reports facing in cardinal directions, a camera that can see blocking walls in front of, or to the right or left of, the robot. It can also see if the corner is coloured with a given colour. There is a microphone that is able to pick up the faint beeping of an active beeper, and Karel can also feel if it carries inactive beepers in its bag or if the bag is empty.

- `facing(dir)` is true if the robot is facing in the given direction
- `seeWall(facing)` is true if there is a wall in the `FRONT` (0), `LEFT` (1), or `RIGHT` (-1) facing
- `seeColor(number)` is true if the current corner is colored with color pair _number_; 0 means uncoloured
- `hearBeepers()` is true if there is at least one beeper at the corner where the robot is located
- `carryBeepers()` is true if there is at least one beeper in the robot's beeper bag

Tests don't have inverse forms: use the `!` operator for that.

The `IF/THEN/ELSE` instruction becomes `if (`_condition_`) `_statementT_` else `_statementF_.

## Iterative statements

The Karel `ITERATE` instruction is implemented as a `repeat (` _n_ `) ` _statement_ statement, which repeats its _statement_ _n_ times. It is an extension to Lox.

The `WHILE` instruction corresponds to the Lox `while (` _condition_ `) ` _statement_ statement.

## Advanced handling

This implementation is capable of recursively executing instructions.

## Calling native functions
There are some problems with native functions, e.g. how runtime errors in one break the interpreter and cause a segfault, leaving curses unterminated. I need to propagate the error to the run function and return from that with a runtime error result. 
I could scrap the native function method and compile functions like `world` to bytecodes to run entirely inside the VM, but for now I'm trying out the option of adding a special ERR value and modifying `callValue` to examine the result of the native function before pushing it on the stack. If the return value is the ERR value, `callValue` returns false, causing `run` to end with an RTE. If the return value is any other value, it is pushed on the stack and `callValue` returns normally.

## Evaluating the outcome
This part is still being planned. One possible way is to divide the script into `initial`, `task`, and `final` sections, with beepers() and colors() (and possibly home()) calls in the `final` section setting up checks against the state of the world after performing the task. Another way is to add cases that demonstrate presence or absence of beepers, or the same for color.

- `home(x, y, d)` sets the location and direction the robot is supposed to end up in to _x_ avenue, _y_ street, and _d_ direction (if not set, the robot can shut down anywhere)

## Acknowledgements
clox belongs to Robert Nystrom. Karel the Robot was designed by Richard E. Pattis. 

## Links
- https://en.wikipedia.org/wiki/Karel_(programming_language)
- https://craftinginterpreters.com/ -- home page of the book "Crafting Interpreters"
- https://ics.uci.edu/~pattis/ -- home page of Richard Pattis
