
#include "world.h"

#define NUM_AVENUES 8
#define NUM_STREETS 8

Corner world[NUM_AVENUES][NUM_STREETS];
Robot karel;

void initKarel(void) {
    karel.color = 0;
    karel.beepers = 0;
    karel.direction = 0;
    karel.avenue = 1;
    karel.street = 1;
}

void initWorld(void) {
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

void placeWall(int avenue, int street, int direction) {
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

bool frontIsBlocked(void) {
    return (karel.direction == 0 && world[karel.avenue][karel.street].wallEast) ||
        (karel.direction == 1 && world[karel.avenue][karel.street].wallNorth) ||
        (karel.direction == 2 && world[karel.avenue][karel.street].wallWest) ||
        (karel.direction == 3 && world[karel.avenue][karel.street].wallSouth);
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
