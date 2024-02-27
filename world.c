
#include "world.h"

// most worlds will be smaller in practice
int NUM_AVENUES = 11;
int NUM_STREETS = 11;

Corner world[11][11];

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
    return LINES-(2+(4*y));
}

int actualx(int x) {
    return 2+(6*x);
}

void showWall(int avenue, int street, int direction);

int initWorld(int avenues, int streets) {
    NUM_AVENUES = avenues;
    if (NUM_AVENUES > 11)
        NUM_AVENUES = 11;
    NUM_STREETS = streets;
    if (NUM_STREETS > 11)
        NUM_STREETS = 11;
    for (int a = 1; a <= NUM_AVENUES; a++) {
        for (int s = 1; s <= NUM_STREETS; s++) {
            mvaddch(actualy(s), actualx(a), '+');
            world[a][s].color = 0;
            world[a][s].beepers = 0;
        }
    }
    for (int a = 1, s = 1; s <= NUM_STREETS; s++) {
        world[a][s].wallWest = true;
        showWall(a, s, 2);
    }
    for (int a = NUM_AVENUES, s = 1; s <= NUM_STREETS; s++) {
        world[a][s].wallEast = true;
        showWall(a, s, 0);
    }
    for (int a = 1, s = 1; a <= NUM_AVENUES; a++) {
        world[a][s].wallSouth = true;
        showWall(a, s, 3);
    }
    for (int a = 1, s = NUM_STREETS; a <= NUM_AVENUES; a++) {
        world[a][s].wallNorth = true;
        showWall(a, s, 1);
    }
    for (int a = 1; a <= NUM_AVENUES; a++) {
        mvprintw(actualy(0), actualx(a)-1, "%2d", a);
    }
    for (int s = 1; s <= NUM_STREETS; s++) {
        mvprintw(actualy(s), actualx(0)-1, "%2d", s);
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
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

    karel.direction = abs(direction) % 4;

    karel.beepers = beepers;
    showRobot();
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

void showWall(int avenue, int street, int direction) {
    int x = actualx(avenue);
    int y = actualy(street);
    if (direction == 0) {
        x += 3;
        for (int y_ = y - 1; y_ <= y + 1; y_++) {
            mvaddch(y_, x, '|');
        }
    } else if (direction == 1) {
        y -= 2;
        x -= 1;
        mvaddstr(y, x, "---");
    } else if (direction == 2) {
        x -= 3;
        for (int y_ = y - 1; y_ <= y + 1; y_++) {
            mvaddch(y_, x, '|');
        }
    } else if (direction == 3) {
        y += 2;
        x -= 1;
        mvaddstr(y, x, "---");
    }
    refresh();
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
    direction = abs(direction) % 4;
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
    showWall(avenue, street, direction);

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
  world[karel.avenue][karel.street].color = karel.color;
}

void setColor(int color) {
    karel.color = color;
    world[karel.avenue][karel.street].color = color;
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
    if (world[avenue][street].color != 0) {
        attrset(COLOR_PAIR(world[avenue][street].color));
    }
    if (world[avenue][street].beepers == 0) {
        mvaddch(actualy(street), actualx(avenue), '+');
    } else {
        mvprintw(actualy(street), actualx(avenue)-1, "%2d", world[avenue][street].beepers);
    }
    attrset(A_NORMAL);
}

void turnLeft(void) {
    karel.direction = (karel.direction + 1) % 4;
}

bool facingDirection(int direction) {
    return abs(direction) % 4 == karel.direction;
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
