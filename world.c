
#include "world.h"

// most worlds will be smaller in practice
int NUM_AVENUES = 16;
int NUM_STREETS = 16;

Corner world[16][16];

Home home;

Robot karel;

void defaultRobot(void) {
    karel.color = 0;
    karel.beepers = 0;
    karel.direction = 0;
    karel.avenue = 1;
    karel.street = 1;
}

int actualy(int y) {
    return LINES-(2+(2*y));
}

int actualx(int x) {
    return 2+(4*x);
}

int initWorld(int avenues, int streets) {
    NUM_AVENUES = avenues;
    if (NUM_AVENUES > 16)
        NUM_AVENUES = 16;
    NUM_STREETS = streets;
    if (NUM_STREETS > 16)
        NUM_STREETS = 16;
    for (int a = 1; a <= NUM_AVENUES; a++) {
        for (int s = 1; s <= NUM_STREETS; s++) {
            mvaddch(actualy(s), actualx(a), '+');
            world[a][s].color = 0;
            world[a][s].beepers = 0;
            if (a == 1)
                world[a][s].wallWest = true;
            else if (a == NUM_AVENUES)
                world[a][s].wallEast = true;
            else if (s == 1)
                world[a][s].wallSouth = true;
            else if (s == NUM_STREETS)
                world[a][s].wallNorth = true;
        }
    }
    for (int a = 1; a <= NUM_AVENUES; a++) {
        mvprintw(actualy(0), actualx(a)-1, "%2d", a);
    }
    for (int s = 1; s <= NUM_STREETS; s++) {
        mvprintw(actualy(s), actualx(0)-1, "%2d", s);
    }
    return 0;
}

int initRobot(int avenue, int street, int direction, int beepers) {
    karel.avenue = avenue;
    if (karel.avenue < 1)
        karel.avenue = 1;
    if (karel.avenue > NUM_AVENUES)
        karel.avenue = NUM_AVENUES;

    karel.street = street;
    if (karel.street < 1)
        karel.street = 1;
    if (karel.street > NUM_STREETS)
        karel.street = NUM_STREETS;

    karel.direction = direction % 4;

    karel.beepers = beepers;
    showRobot();
    getch();
    return 0;
}

int placeHome(int avenue, int street, int direction) {
    if (avenue < 1)
        avenue = 1;
    if (avenue > NUM_AVENUES)
        avenue = NUM_AVENUES;
    if (street < 1)
        street = 1;
    if (street > NUM_STREETS)
        street = NUM_STREETS;
    home.avenue = avenue;
    home.street = street;
    move(actualy(street), actualx(avenue));
    home.direction = direction;
    switch (direction) {
        case 0: addch(A_REVERSE | '>'); break;
        case 1: addch(A_REVERSE | '^'); break;
        case 2: addch(A_REVERSE | '<'); break;
        case 3: addch(A_REVERSE | 'v'); break;
        default: return 1;
    }
    refresh();
    return 0;
}

bool homeDefined(void) {
    return home.avenue != 0 && home.street != 0;
}

void showCorner(int avenue, int street);

int placeBeepers(int avenue, int street, int number) {
    if (avenue < 1)
        avenue = 1;
    if (avenue > NUM_AVENUES)
        avenue = NUM_AVENUES;
    if (street < 1)
        street = 1;
    if (street > NUM_STREETS)
        street = NUM_STREETS;
    world[avenue][street].beepers = number;
//    printf("curses: place %d beepers at %d,%d\n", number, avenue, street);
    showCorner(avenue, street);
    refresh();
    return 0;
}

int placeWall(int avenue, int street, int direction) {
    if (avenue < 1)
        avenue = 1;
    if (avenue > NUM_AVENUES)
        avenue = NUM_AVENUES;
    if (street < 1)
        street = 1;
    if (street > NUM_STREETS)
        street = NUM_STREETS;
    direction = direction % 4;
    // don't place a piece of wall at the border
    if ((direction == 0 && avenue == NUM_AVENUES) || 
        (direction == 1 && street == NUM_STREETS) || 
        (direction == 2 && avenue == 1) || 
        (direction == 3 && street == 1))
        return 0;
    
    // duplicate a wall from the opposite direction
    if (direction == 0) {
        world[avenue][street].wallEast = true;
        world[avenue + 1][street].wallWest = true;
    } else if (direction == 1) {
        world[avenue][street].wallNorth = true;
        world[avenue][street + 1].wallSouth = true;
    } else if (direction == 2) {
        world[avenue][street].wallWest = true;
        world[avenue - 1][street].wallEast = true;
    } else if (direction == 3) {
        world[avenue][street].wallSouth = true;
        world[avenue][street - 1].wallNorth = true;
    }
    printf("curses: place wall at %d,%d, direction %d\n", avenue, street, direction);
    return 0;
}

bool beeperBagEmpty(void) {
    return karel.beepers <= 0;
}

bool noBeepersAtCorner(void) {
    return world[karel.avenue][karel.street].beepers <= 0;
}

void decrementBeeperBag(void) {
    karel.beepers--;
}

void incrementBeeperBag(void) {
    karel.beepers++;
}

void decrementBeepersAtCorner(void) {
    world[karel.avenue][karel.street].beepers--;
}

void incrementBeepersAtCorner(void) {
    world[karel.avenue][karel.street].beepers++;
}

void moveToNext(void) {
  switch (karel.direction) {
    case 0:
        karel.avenue++;
        break;
    case 1:
        karel.street++;
        break;
    case 2:
        karel.avenue--;
        break;
    case 3:
        karel.street--;
        break;
  }
}

int karely(void) {
    return actualy(karel.street);
}

int karelx(void) {
    return actualx(karel.avenue);
}

void showRobot(void) {
    switch (karel.direction) {
        case 0: mvaddch(karely(), karelx(), '>'); break;
        case 1: mvaddch(karely(), karelx(), '^'); break;
        case 2: mvaddch(karely(), karelx(), '<'); break;
        case 3: mvaddch(karely(), karelx(), 'v'); break;
        default: return;
    }
    refresh();
}

void showCorner(int avenue, int street) {
    switch (world[avenue][street].beepers) {
        case 0: mvaddch(actualy(street), actualx(avenue), '+'); break;
        case 1: mvaddch(actualy(street), actualx(avenue), '1'); break;
        case 2: mvaddch(actualy(street), actualx(avenue), '2'); break;
        case 3: mvaddch(actualy(street), actualx(avenue), '3'); break;
        case 4: mvaddch(actualy(street), actualx(avenue), '4'); break;
        case 5: mvaddch(actualy(street), actualx(avenue), '5'); break;
        case 6: mvaddch(actualy(street), actualx(avenue), '6'); break;
        case 7: mvaddch(actualy(street), actualx(avenue), '7'); break;
        case 8: mvaddch(actualy(street), actualx(avenue), '8'); break;
        case 9: mvaddch(actualy(street), actualx(avenue), '9'); break;
        default: mvaddch(actualy(street), actualx(avenue), '*'); break;
    }
}

void unShowRobot(void) {
    showCorner(karel.avenue, karel.street);
}

void turnLeft(void) {
    karel.direction = (karel.direction + 1) % 4;
}

bool facingDirection(int direction) {
    return direction % 4 == karel.direction;
}

bool facingIsBlocked(int facing) {
    // 0 = front; 1 = left; -1 = right
    int dir = (karel.direction + 4 + facing) % 4;
    if (dir == 0)
        return world[karel.avenue][karel.street].wallEast;
    else if (dir == 1)
        return world[karel.avenue][karel.street].wallNorth;
    else if (dir == 2)
        return world[karel.avenue][karel.street].wallWest;
    else if (dir == 3)
        return world[karel.avenue][karel.street].wallSouth;
    else
        return false; // unreachable
}
