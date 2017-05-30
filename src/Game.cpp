#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace sf;

Game* Game::instance;

Game::Game(RenderWindow* _window)
{
    Game::instance = this;
    window = _window;
}

void Game::initialize()
{
    loadAudio(audioFileNames);
    loadTextures(textureFileNames);

    for (int i = 0; i < windowHeight * windowWidth; i++) {
        world[i] = AIR;
    }

    const int NUMBIRDS = 200;
    const int SPACE = 4;

    for (int i = 640 - NUMBIRDS; i < 640 + NUMBIRDS; i += SPACE) {
        for (int j = 360 - NUMBIRDS; j < 360 + NUMBIRDS; j+= SPACE) {
            Bird bird = Bird(Vector2i(i, j));
            birds.push_back(bird);
        }
    }

    for (int i = 0; i < birds.size(); i++) {
        world[coordsToWorldIndex(birds[i].getPosition())] = BIRD;
    }

    for (int i = 0; i < 10; i++) {
        generateSquareObstacle(Vector2i(randint(0, 1280, i), randint(0, 720, i)), randint(5, 25, i));
    }

}

void Game::update()
{
    Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Escape)
            {
                window->close();
            }
            if (event.key.code == Keyboard::F1)
            {
                takeScreenshot();
            }
            if (event.key.code == Keyboard::Up)
            {
                birdsPerFrame *= 2;
                std::cout << "Birds per frame: " << birdsPerFrame << "\n";
            }
            if (event.key.code == Keyboard::Down)
            {
                birdsPerFrame /= 2;
                std::cout << "Birds per frame: " << birdsPerFrame << "\n";
            }
            if (event.key.code == Keyboard::Return)
            {
                birdsPerFrame = birds.size();
            }
            if (event.key.code == Keyboard::C)
            {
                circling = !circling;
            }
            if (event.key.code == Keyboard::H)
            {
                hideGoal = !hideGoal;
            }
            if (event.key.code == Keyboard::Num1)
            {
                goalCircleSpeed *= 2;
            }
            if (event.key.code == Keyboard::Num2)
            {
                goalCircleSpeed /= 2;
            }
            if (event.key.code == Keyboard::G)
            {
                generateSquareObstacle(Mouse::getPosition(*window), randint(5, 30));
            }
        }
        if (event.type == Event::MouseMoved)
        {
            for (int i = 0; i < birds.size(); i++) {
                goal = Vector2i(event.mouseMove.x, event.mouseMove.y);
            }
        }
        if (event.type == Event::LostFocus) {
            focus = false;
        }
        if (event.type == Event::GainedFocus) {
            focus = true;
        }

        if (event.type == Event::Resized) {
            windowWidth = event.size.width;
            windowHeight = event.size.height;
        }
    }

    dt = clock.restart();
    totalTime += dt;

    if (circling) {
        int x = 640 + cos(totalTime.asMilliseconds() / 5000.0 * goalCircleSpeed) * 300;
        int y = 360 + sin(totalTime.asMilliseconds() / 5000.0 * goalCircleSpeed) * 300;
        for (int i = 0; i < birds.size(); i++) {
            goal = Vector2i(x, y);
        }
    }


    for (int i = 0; i < birdsPerFrame; i++) {
        int b = Game::randint(0, birds.size() - 1);

        int index = coordsToWorldIndex(birds[b].getPosition());
        world[index] = AIR;
        birds[b].update();

        index = coordsToWorldIndex(birds[b].getPosition());
        world[index] = BIRD;

        cursor = cursor > birds.size() ? 0 : cursor + 1;
    }

    frame++;
}

void Game::draw()
{
    window->clear();

    Image screen;
    screen.create(windowWidth, windowHeight, Color(171, 201, 238));

    for (int i = 0; i < 720 * 1280; i++) {
        Vector2i pos = worldIndexToCoords(i);

        if (world[i] == BIRD) {
            screen.setPixel(pos.x, pos.y, Color(0, 0, 0));
        }

        if (world[i] == OBSTACLE) {
            screen.setPixel(pos.x, pos.y, Color(100, 100, 100));
        }
    }

    Texture screenTexture;
    screenTexture.loadFromImage(screen);

    Sprite screenSprite;
    screenSprite.setTexture(screenTexture);
    window->draw(screenSprite);

    if (!hideGoal) {
        CircleShape goalSprite;
        goalSprite.setPosition(Vector2f(goal.x, goal.y));
        goalSprite.setFillColor(Color::Red);
        goalSprite.setRadius(2);
        window->draw(goalSprite);
    }

    window->display();
}

bool Game::isWindowOpen()
{
    return window->isOpen();
}

void Game::loadAudio(std::vector<std::string> audioFileNames)
{
    std::cout << "Loading " << audioFileNames.size() << " audio files..." << "\n";
    for (int i = 0; i < audioFileNames.size(); i++)
    {
        sfx.insert(std::pair<std::string, Audio*>(audioFileNames[i], new Audio()));
        sfx[audioFileNames[i]]->init("audio/" + audioFileNames[i]);
        std::cout << "Loaded audio " << "audio/" + audioFileNames[i] << "\n";
    }
}

void Game::loadTextures(std::vector<std::string> textureFileNames)
{
    std::cout << "Loading " << textureFileNames.size() << " textures...\n";
    for (int i = 0; i < textureFileNames.size(); i++)
    {
        Texture texture;
        std::string path = "textures/" + textureFileNames.at(i);
        if (!texture.loadFromFile(path))
            window->close();
        textures.insert(std::pair<std::string, Texture>(textureFileNames[i], texture));
        std::cout << "Loaded " << path << "\n";
    }
}

void Game::takeScreenshot()
{
    Image screenshot = window->capture();
    screenshot.saveToFile("./screenshot" + std::to_string(randint(0, 65536)) + ".png");
}

int Game::randint(int low, int high)
{
    int value = rand() % (high + 1 - low) + low;
    srand(totalTime.asMicroseconds() * value * rand());

    return value;
}

int Game::randint(int low, int high, int seed)
{
    int value = rand() % (high + 1 - low) + low;
    srand(clock.restart().asMicroseconds() * value * rand());

    return value;
}

int Game::coordsToWorldIndex(Vector2i coords)
{
    return coords.x + coords.y * 1280;
}

Vector2i Game::worldIndexToCoords(int index)
{
    return Vector2i(index % 1280, index / 1280);
}

int Game::getWorldTile(Vector2i pos)
{
    pos.y = pos.y <= 0 ? 0 : pos.y;
    pos.y = pos.y >= 720 ? 720 : pos.y;
    pos.x = pos.x <= 0 ? 0 : pos.x;
    pos.x = pos.x >= 1280 ? 1280 : pos.x;

    return world[coordsToWorldIndex(pos)];
}

void Game::generateSquareObstacle(Vector2i pos, int size)
{
    for (int x = pos.x; x < size + pos.x; x++) {
        for (int y = pos.y; y < size + pos.y; y++) {
            int index =coordsToWorldIndex(Vector2i(x, y));
            if (index < 720 * 1280) {
                world[index] = OBSTACLE;
            }
        }
    }
}
