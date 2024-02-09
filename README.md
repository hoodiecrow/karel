# karel
An implementation of the classic Karel the Robot educational program, based on the clox language infrastructure from the book "Crafting Interpreters" by Robert Nystrom.

## clox
clox is a dynamic, object-oriented, small bytecode-compiled language. The version I am using here uses code from up to and including chapter 24. That is, it doesn't have closures, garbage collection, or actual OOP features.

## karel
I intend to add seven statement types to clox to make controlling the robot possible.

- `move` -- advance one corner (done)
- `left` -- change facing 90° to the left (done)
- `put` -- put a beeper on the corner (done)
- `get` -- remove a beeper from the corner (done)
- `color(n)` -- color the corner with color #_n_
- `repeat(n) statement` -- repeat a (possibly compound) statement _n_ times (done)
- `quit` -- shut down the robot

The arena or world karel moves in will be displayed as ncurses character graphics.

### Initialization
The functions `world`, `home`, `robot`, `beepers`, and `wall` have been added to initialize the world parts. They are defined using the native function interface (such as it is).
- world(av, st) sets the size of the world to 1..av x 1..st
- home(av, st) sets the location where the robot is supposed to end up (if not set, the robot can shut down anywhere)
- robot(av, st, d, n) sets the robot at avenue _av_, street _st_, facing direction _d_, and carrying _n_ beepers
- beepers(av, st, n) sets down _n_ beepers at avenue _av_, street _st_
- wall(av, st, d) sets down a wall near avenue _av_, street _st_, blocking the way in direction d (and also from the opposite direction)

### Conditions
- facing(dir) verifies that the robot is facing in the given direction
- blocked(facing) is true if there is a wall in the front (0), left (1), or right (-1) facing
- beepersAtCorner() is true if there is at least one beeper on the corner where the robot is located
- beeperBagEmpty() is true if there are no beepers in the robot's beeper bag

## Acknowledgements
clox belongs to Robert Nystrom. Karel the Robot was designed by Richard E. Pattis. 

## Links
- https://en.wikipedia.org/wiki/Karel_(programming_language)
- https://craftinginterpreters.com/ -- home page of the book "Crafting Interpreters"
- https://ics.uci.edu/~pattis/ -- home page of Richard Pattis
