#ifndef ktr_world_h
#define ktr_world_h

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "value.h"
#include "vm.h"

typedef struct {
  int color;
  int beepers;
  bool wallEast;
  bool wallNorth;
  bool wallWest;
  bool wallSouth;
} Corner;

typedef struct {
  int avenue;
  int street;
  int direction;
} Home;

typedef struct {
  int color;
  int beepers;
  int direction;
  int avenue;
  int street;
} Robot;

/*
 * direction: 0 = > / East, 1 = ^ / North, 2 = < / West, 3 = v / South
 *
 * 0 beepers: +; n beepers (1-9): n; n beepers (10+): *
 */

extern Corner world[11][11];
extern Home home;
extern Robot karel;

void defaultRobot(void);
int initWorld(int avenues, int streets);
int initRobot(int avenue, int street, int direction, int beepers);
int placeHome(int avenue, int street, int direction);
bool homeDefined(void);
int placeBeepers(int avenue, int street, int number);
int placeWall(int avenue, int street, int direction);
bool facingDirection(int direction);
bool facingIsBlocked(int facing);
bool beeperBagEmpty(void);
bool noBeepersAtCorner(void);
void decrementBeeperBag(void);
void incrementBeeperBag(void);
void decrementBeepersAtCorner(void);
void incrementBeepersAtCorner(void);
void moveToNext(void);
void showCorner(int avenue, int street);
void showRobot(void);
void turnLeft(void);

#endif
