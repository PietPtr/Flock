#ifndef BIRD_H
#define BIRD_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Bird
{
    public:
        Bird(Vector2i pos);

        void update();

        Vector2i getPosition() { return pos; }
        Vector2i numToDir(int num);
        int dirToNum(Vector2i pos);
    protected:
    private:
        Vector2i pos;
        int previousDirection = -1;
};

#endif // BIRD_H
