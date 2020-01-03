
#include "micromouseserver.h"
#include <iostream>
using namespace std;

int graph[21][21] = {0}; //I put it as 21 * 21, just so when I fill the dirarray, if the mouse is at the border it will say 0 and not give an error
int dirarr[3];
int dirX = 1;
int dirY = 1;
int direction = 1;
int leftTurnCounter = 0;
int rightTurnCounter = 0;

int getDirection(int curDirection, bool turnRight, bool turnLeft) {
    if (!turnRight && !turnLeft) {
        return curDirection;
    }

// Up = 1, Right = 2, Down = 3, Left = 4
    switch(curDirection) {
        case 1:
            if (turnRight) {
                return 2;
            }
            if (turnLeft) {
                return 4;
            }

        case 2:
            if(turnRight) {
                return 3;
            } else if (turnLeft) {
                return 1;
            }

        case 3:
            if(turnRight) {
                return 4;
            } else if (turnLeft) {
                return 2;
            }
        case 4:
            if(turnRight) {
                return 1;
            } else if (turnLeft) {
                return 3;
            }
    }
}

void getDirectionArray(int direction, bool wallLeft, bool wallRight, bool wallForward) {
    if(direction == 1) {
        dirarr[0] = graph[dirX-1][dirY];
        dirarr[1] = graph[dirX][dirY+1];
        dirarr[2] = graph[dirX+1][dirY];
    }
    if(direction == 2) {
        dirarr[0] = graph[dirX][dirY+1];
        dirarr[1] = graph[dirX+1][dirY];
        dirarr[2] = graph[dirX][dirY-1];
    }
    if(direction == 3) {
        dirarr[0] = graph[dirX+1][dirY];
        dirarr[1] = graph[dirX][dirY-1];
        dirarr[2] = graph[dirX-1][dirY];
    }
    if(direction == 4) {
        dirarr[0] = graph[dirX][dirY-1];
        dirarr[1] = graph[dirX-1][dirY];
        dirarr[2] = graph[dirX][dirY+1];
    }

    if(wallLeft) {
        dirarr[0] = 1000;
    }
    if(wallRight) {
        dirarr[2] = 1000;
    }
    if(wallForward) {
        dirarr[1] = 1000;
    }
}

int chooseDirection(int minValue) {
   //1 go left, 2 go forward, 3 go right

   if(dirarr[2] == minValue) {
       return 3;
   } else if(dirarr[1] == minValue) {
       return 2;
   } else {
       return 1;
   }
}

int findMinValue() {
    int crown = dirarr[0];

    if (crown <= dirarr[1] && crown <= dirarr[2]) {
        crown = dirarr[0];
    } else if (dirarr[1] <= dirarr[0] && dirarr[1] <= dirarr[2]) {
        crown = dirarr[1];
    } else if (dirarr[2] <= dirarr[0] && dirarr[2] <= dirarr[1]) {
        crown = dirarr[2];
    }
    return crown;
}

void updateXY(int direction, bool f, bool r, bool l) {
    if(direction == 1 && f) {
        dirY++;
    } else if (direction == 1 && r) {
        dirX++;
    } else if(direction == 1 && l) {
        dirX--;
    }

    if(direction == 2 && f) {
        dirX++;
    } else if (direction == 2 && r) {
        dirY--;
    } else if(direction == 2 && l) {
        dirY++;
    }

    if(direction == 3 && f) {
        dirY--;
    } else if (direction == 3 && r) {
        dirX--;
    } else if(direction == 3 && l) {
        dirX++;
    }

    if(direction == 4 && f) {
        dirX--;
    } else if (direction == 4 && r) {
        dirY++;
    } else if(direction == 4 && l) {
        dirY--;
    }
}

void microMouseServer::studentAI()
{
/*
 * The following are the eight functions that you can call. Feel free to create your own fuctions as well.
 * Remember that any solution that calls moveForward more than once per call of studentAI() will have points deducted.
 *
 *The following functions return if there is a wall in their respective directions
 *bool isWallLeft();
 *bool isWallRight();
 *bool isWallForward();
 *
 *The following functions move the mouse. Move forward returns if the mouse was able to move forward and can be used for error checking
 *bool moveForward();
 *void turnLeft();
 *void turnRight();
 *
 * The following functions are called when you need to output something to the UI or when you have finished the maze
 * void foundFinish();
 * void printUI(const char *mesg);
*/


    bool isTurningRight;
    bool isTurningLeft;
    int count;
    int turnValue;
    int minValue;
    int readValue;


    count = graph[dirX][dirY];
    count++;
    graph[dirX][dirY] = count;

    getDirectionArray(direction, isWallLeft(), isWallRight(), isWallForward());
    //If dead-end
    if(isWallLeft() && isWallRight() && isWallForward()) {
            turnRight();
            direction = getDirection(direction, true, false);
            turnRight();
            direction = getDirection(direction, true, false);

    } else {

    minValue = findMinValue();
    turnValue = chooseDirection(minValue);
    if(turnValue == 1) {
        updateXY(direction, false, false, true);
        turnLeft();
        rightTurnCounter = 0;
        leftTurnCounter++;
        moveForward();
        direction = getDirection(direction, false, true);

    } else if(turnValue == 2) {
        updateXY(direction, true, false, false);
        moveForward();
        leftTurnCounter = 0;
        rightTurnCounter = 0;
        direction = getDirection(direction, false, false);
    } else if(turnValue == 3) {
        updateXY(direction, false, true, false);
        turnRight();
        leftTurnCounter = 0;
        rightTurnCounter++;
        moveForward();
        direction = getDirection(direction, true, false);
    }
    }
    if (leftTurnCounter >=3 || rightTurnCounter >= 3) {
        cout << "Found it";
        foundFinish();
         printUI("Done!");
    }
}
