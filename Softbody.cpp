#include "Softbody.h"

#include <math.h>

#include "Renderer.h"
#include "Shape.h"

extern std::vector<std::unique_ptr<Shape>> shapes;
extern Renderer renderer;

void Softbody::Update(float dt)
{
    for (auto& point : points)
    {
        point.vel *= 0.98;
        point.vel.y += 2;
        point.pos += point.vel * dt;
    }

    for (auto& edge : edges)
    {
        Point& p1 = points[edge.pi1];
        Point& p2 = points[edge.pi2];
        Vec2 posDif = p1.pos - p2.pos;
        float len = posDif.Length();
        float dlen = len - edge.len;
        Vec2 force = posDif.Normalized() * dlen * strength;
        p1.vel -= force;
        p2.vel += force;

        for (auto& shape : shapes)
        {
            auto pairs = shape->RayCast(p1.pos, p2.pos);
            for (auto& pair : pairs)
            {
                Vec2 intersect = pair.first, normal = pair.second;
                Point& closestPoint = (intersect - p1.pos).LengthSquared() < (intersect - p2.pos).LengthSquared() ? p1 : p2;
                closestPoint.vel += normal * (intersect - closestPoint.pos).Length();
            }
        }
    }
}

void Softbody::Draw() const
{
    // for (auto& point : points)
    // {
    //     renderer.DrawFilledCircle(point.pos, 5, {255, 255, 255});
    // }
    for (auto& edge : edges)
    {
        renderer.DrawLine(points[edge.pi1].pos, points[edge.pi2].pos, {255, 255, 0});
    }
}

void Softbody::MakeCircle(Vec2 pos, float radius, int ptCount)
{
    float da = 2 * M_PI / ptCount;
    for (float a = 0; a < 2 * M_PI; a += da)
    {
        points.push_back({{pos.x + cosf(a) * radius, pos.y + sinf(a) * radius}});
    }
    ConnectPoints();
}

void Softbody::MakeRect(Vec2 pos, Vec2 size)
{
    points.push_back({pos});
    points.push_back({{pos.x + size.x, pos.y}});
    points.push_back({pos + size});
    points.push_back({{pos.x, pos.y + size.y}});
    ConnectPoints();
}

void Softbody::ConnectPoints()
{
    for (int i = 0; i < points.size(); i++)
    {
        for (int j = i + 1; j < points.size(); j++)
        {
            float len = (points[i].pos - points[j].pos).Length();
            edges.push_back({i, j, len});
        }
    }
}