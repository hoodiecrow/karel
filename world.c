
#include "world.h"

// most worlds will be smaller in practice
int NUM_AVENUES = 16;
int NUM_STREETS = 16;

Corner world[16][16];

Robot karel;

void defaultRobot(void) {
    karel.color = 0;
    karel.beepers = 0;
    karel.direction = 0;
    karel.avenue = 1;
    karel.street = 1;
}

void initWorld(Value avenues, Value streets) {
    if (!IS_NUMBER(avenues)) {
        runtimeError("avenues is not a number.");
        return;
    }
    NUM_AVENUES = AS_NUMBER(avenues);
    if (NUM_AVENUES > 16)
        NUM_AVENUES = 16;
    if (!IS_NUMBER(streets)) {
        runtimeError("streets is not a number.");
        return;
    }
    NUM_STREETS = AS_NUMBER(streets);
    if (NUM_STREETS > 16)
        NUM_STREETS = 16;
    for (int a = 1; a <= NUM_AVENUES; a++) {
        for (int s = 1; s <= NUM_STREETS; s++) {
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
}

void initRobot(Value avenue, Value street, Value direction, Value beepers) {
    if (!IS_NUMBER(avenue)) {
        runtimeError("avenue is not a number.");
        return;
    }
    karel.avenue = AS_NUMBER(avenue);
    if (karel.avenue < 1)
        karel.avenue = 1;
    if (karel.avenue > NUM_AVENUES)
        karel.avenue = NUM_AVENUES;

    if (!IS_NUMBER(street)) {
        runtimeError("street is not a number.");
        return;
    }
    karel.street = AS_NUMBER(street);
    if (karel.street < 1)
        karel.street = 1;
    if (karel.street > NUM_STREETS)
        karel.street = NUM_STREETS;

    if (!IS_NUMBER(direction)) {
        runtimeError("direction is not a number.");
        return;
    }
    karel.direction = (int)AS_NUMBER(direction) % 4;

    if (!IS_NUMBER(beepers)) {
        runtimeError("beepers is not a number.");
        return;
    }
    karel.beepers = AS_NUMBER(beepers);
}

void placeHome(Value avenue, Value street) {
    if (!IS_NUMBER(avenue)) {
        runtimeError("avenue is not a number.");
        return;
    }
    int havenue = AS_NUMBER(avenue);
    if (havenue < 1)
        havenue = 1;
    if (havenue > NUM_AVENUES)
        havenue = NUM_AVENUES;

    if (!IS_NUMBER(street)) {
        runtimeError("street is not a number.");
        return;
    }
    int hstreet = AS_NUMBER(street);
    if (hstreet < 1)
        hstreet = 1;
    if (hstreet > NUM_STREETS)
        hstreet = NUM_STREETS;
    world[havenue][hstreet].home = true;
}

void placeBeepers(Value avenue, Value street, Value number) {
    if (!IS_NUMBER(avenue)) {
        runtimeError("avenue is not a number.");
        return;
    }
    int bavenue = AS_NUMBER(avenue);
    if (bavenue < 1)
        bavenue = 1;
    if (bavenue > NUM_AVENUES)
        bavenue = NUM_AVENUES;

    if (!IS_NUMBER(street)) {
        runtimeError("street is not a number.");
        return;
    }
    int bstreet = AS_NUMBER(street);
    if (bstreet < 1)
        bstreet = 1;
    if (bstreet > NUM_STREETS)
        bstreet = NUM_STREETS;

    if (!IS_NUMBER(number)) {
        runtimeError("number is not a number."); // oh yes
        return;
    }
    int bnumber = AS_NUMBER(number);
    world[bavenue][bstreet].beepers = bnumber;
}

void placeWall(Value avenue, Value street, Value direction) {
    if (!IS_NUMBER(avenue)) {
        runtimeError("avenue is not a number.");
        return;
    }
    int wavenue = AS_NUMBER(avenue);
    if (wavenue < 1)
        wavenue = 1;
    if (wavenue > NUM_AVENUES)
        wavenue = NUM_AVENUES;

    if (!IS_NUMBER(street)) {
        runtimeError("street is not a number.");
        return;
    }
    int wstreet = AS_NUMBER(street);
    if (wstreet < 1)
        wstreet = 1;
    if (wstreet > NUM_STREETS)
        wstreet = NUM_STREETS;

    if (!IS_NUMBER(direction)) {
        runtimeError("direction is not a number.");
        return;
    }
    int wdirection = (int)AS_NUMBER(direction) % 4;
    // don't place a piece of wall at the border
    if ((wdirection == 0 && wavenue == NUM_AVENUES) || 
        (wdirection == 1 && wstreet == NUM_STREETS) || 
        (wdirection == 2 && wavenue == 1) || 
        (wdirection == 3 && wstreet == 1))
        return;
    
    // duplicate a wall from the opposite direction
    if (wdirection == 0) {
        world[wavenue][wstreet].wallEast = true;
        world[wavenue + 1][wstreet].wallWest = true;
    } else if (wdirection == 1) {
        world[wavenue][wstreet].wallNorth = true;
        world[wavenue][wstreet + 1].wallSouth = true;
    } else if (wdirection == 2) {
        world[wavenue][wstreet].wallWest = true;
        world[wavenue - 1][wstreet].wallEast = true;
    } else if (wdirection == 3) {
        world[wavenue][wstreet].wallSouth = true;
        world[wavenue][wstreet - 1].wallNorth = true;
    }
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

void turnLeft(void) {
    karel.direction = (karel.direction + 1) % 4;
}

bool facingDirection(Value direction) {
    if (!IS_NUMBER(direction)) {
        runtimeError("direction is not a number.");
        return false;
    }
    int dir = (int)AS_NUMBER(direction) % 4;
    return dir == karel.direction;
}

bool facingIsBlocked(Value facing) {
    // 0 = front; 1 = left; -1 = right
    if (!IS_NUMBER(facing)) {
        runtimeError("facing is not a number.");
        return false;
    }
    int f = AS_NUMBER(facing);
    if (!(f == 0 || f == 1 || f == -1)) {
        runtimeError("invalid facing.");
        return false;
    }
    int dir = (karel.direction + 4 + f) % 4;
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
