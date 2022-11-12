#include<iostream>

struct ViewportSpace
{
    ViewportSpace() : x(0), y(0), width(1), height(1) {}
    ViewportSpace(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}
    int x;
    int y;
    int width;
    int height;
};

struct LogicSpace
{
    LogicSpace() : x(0), y(0), width(1), height(1) {}
    LogicSpace(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {}
    float x;
    float y;
    float width;
    float height;
};

// projectile structure
struct Feather
{
    std::string pName;
    float x, y;
    float angle;
    bool spawned;
    int lifespan;
    float size;

};

// enemy structure
struct Enemy
{
    std::string name;
    float x, y;
    float angle;
    bool spawned;
    bool collided;
    float speed;
};

// obstacle structure
struct Obstacle
{
    std::string name;
    float x, y;
    float sizeX, sizeY;
};

// healthbar structure
struct Healthbar {
    std::string name;
    float posX, posY;
    float sizeX, sizeY;
    float value;
};

struct HealthPickup {
    float posX, posY;
    float sizeX, sizeY;
    float value;
};