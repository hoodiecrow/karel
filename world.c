
#include "world.h"

Corner world[8][8];
Robot karel;

void initKarel(void) {
    karel.color = 0;
    karel.beepers = 0;
    karel.direction = 0;
    karel.avenue = 1;
    karel.street = 1;
}

void initWorld(void) {
    for (int a = 1; a <= 8; a++) {
        for (int s = 1; s <= 8; s++) {
            world[a][s].color = 0;
            world[a][s].beepers = 0;
        }
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
  switch (karel.direction) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
  }
}

bool moveOffEdge(void) {
  switch (karel.direction) {
    case 0: return karel.avenue == 8;
    case 1: return karel.street == 8;
    case 2: return karel.avenue == 1;
    case 3: return karel.street == 1;
  }
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
