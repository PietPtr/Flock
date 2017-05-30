#include <SFML/Graphics.hpp>
#include <iostream>
#include "Audio.h"
#include "Bird.h"

using namespace sf;

enum Tile {
    AIR,
    BIRD,
    OBSTACLE
};

class Game
{
    public:
        Game(RenderWindow* window);
        static Game* instance;
        void initialize();
        void update();
        void draw();
        bool isWindowOpen();
        void generateSquareObstacle(Vector2i pos, int size);

        void loadAudio(std::vector<std::string> fileNames);
        void loadTextures(std::vector<std::string> fileNames);

        int randint(int low, int high, int seed);
        int randint(int low, int high);

        void takeScreenshot();

        int coordsToWorldIndex(Vector2i coords);
        Vector2i worldIndexToCoords(int index);
        int getWorldTile(Vector2i pos);

        Vector2i goal;
    protected:
    private:
        RenderWindow* window;

        Time dt;
        Time totalTime;
        Clock clock;
        int frame = 0;
        bool focus = false;

        int windowWidth = 1280;
        int windowHeight = 720;

        std::vector<std::string> audioFileNames { };
        std::vector<std::string> textureFileNames { "test.png" };

        std::map<std::string, Audio*> sfx;
        std::map<std::string, Texture> textures;

        int world[1280 * 720];

        std::vector<Bird> birds;
        int cursor = 0;
        int birdsPerFrame = 10;

        bool circling = false;
        bool hideGoal = false;
        double goalCircleSpeed = 1;
};
