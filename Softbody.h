#pragma once

#include <vector>

#include "Vec2.h"

struct Point
{
    Vec2 pos, vel;
};

struct Edge
{
    int pi1, pi2;
    float len;
};



class Softbody
{
public:
    float strength;
    std::vector<Point> points;
    std::vector<Edge> edges;

    Softbody(float strength)
        : strength(strength) {}

    void Update(float dt);
    void Draw() const;

    void MakeCircle(Vec2 pos, float radius, int ptCount);
    void MakeRect(Vec2 pos, Vec2 size);
    void ConnectPoints();
};