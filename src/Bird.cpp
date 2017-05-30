#include <iostream>
#include <algorithm>
#include <math.h>
#include "Bird.h"
#include "Game.h"

Bird::Bird(Vector2i pos)
{
    this->pos = pos;
}

void Bird::update() {

    // for all directions:
    // collision with other bird:  -1000
    // Move closer to common goal: +1 per coord nearer
    // Move at all: +1

    std::vector<int> points; // index = direction, value is points

    for (int i = 0; i < 9; i++) {
        points.push_back(0);
    }

    // determine best direction to go
    Vector2i goal = Game::instance->goal;

    Vector2f optimalDir = Vector2f((goal.x - pos.x), (goal.y - pos.y));
    float optimalDirLength = sqrt(pow(optimalDir.x, 2) + pow(optimalDir.y, 2));
    optimalDir /= optimalDirLength;

    Vector2i roundedDirection = Vector2i(round(optimalDir.x), round(optimalDir.y));

    points[dirToNum(roundedDirection)] += 100;


    /*int closestDirection = 0;
    float smallestAngle = 360;
    for (int i = 0; i < 9; i++) {
        float angle = acos(optimalDir.x * numToDir(i).x + optimalDir.y * numToDir(i).y);
        if (angle < smallestAngle) {
            smallestAngle = angle;
            closestDirection = i;
        }
    }

    points[closestDirection] = 10000;*/

    int xdist = abs(pos.x - goal.x);
    int ydist = abs(pos.y - goal.y);

    for (int i = 0; i < 9; i++) {
        if (Game::instance->getWorldTile(pos + numToDir(i)) != 0) {
            points[i] -= 1000;
        }

        if (abs(goal.x - (pos.x + numToDir(i).x)) < xdist) {
            points[i] += 1;
        }

        if (abs(goal.y - (pos.y + numToDir(i).y)) < ydist) {
            points[i] += 1;
        }

        if (i == 4) {
            points[i] -= 1;
        }

    }

    int highest = 0;
    for (int i = 0; i < 9; i++) {
        if (points[i] > points[highest]) {
            highest = i;
        }
    }
    pos += numToDir(highest);


    pos.y = pos.y <= 0 ? 0 : pos.y;
    pos.y = pos.y >= 720 ? 720 : pos.y;
    pos.x = pos.x <= 0 ? 0 : pos.x;
    pos.x = pos.x >= 1280 ? 1280 : pos.x;
}

Vector2i Bird::numToDir(int num)
{
    return Vector2i((num % 3 - 1), (num / 3 - 1));
}

int Bird::dirToNum(Vector2i pos)
{
    return (pos.x + 1) + (pos.y + 1) * 3;
}
