#ifndef ktr_world_h
#define ktr_world_h

#include "common.h"

typedef struct {
  int color;
  int beepers;
  bool wallEast;
  bool wallNorth;
  bool wallWest;
  bool wallSouth;
} Corner;

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

extern Corner world[8][8];
extern Robot karel;

void initKarel(void);
void initWorld(void);
bool beeperBagEmpty(void);
bool noBeepersAtCorner(void);
void decrementBeeperBag(void);
void incrementBeeperBag(void);
void decrementBeepersAtCorner(void);
void incrementBeepersAtCorner(void);
bool frontIsBlocked(void);
bool moveOffEdge(void);
void moveToNext(void);
void turnLeft(void);

#endif
