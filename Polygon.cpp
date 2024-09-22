#include "Polygon.h"

#include "Vec2.h"
#include "Renderer.h"

extern Renderer renderer;

static bool LineSegmentsIntersection(const Vec2& a1, const Vec2& a2, const Vec2& b1, const Vec2& b2, Vec2& outPoint)
{
    float x1 = a1.x, y1 = a1.y, x2 = a2.x, y2 = a2.y;
    float x3 = b1.x, y3 = b1.y, x4 = b2.x, y4 = b2.y;

    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (denom == 0) return false;

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    if (t < 0 || t > 1) return false;

    float u = ((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;
    if (u < -1 || u > 0) return false;
    
    outPoint = {x1 + t * (x2 - x1), y1 + t * (y2 - y1)};

    return true;
}

static bool isLeft(const Vec2& v1, const Vec2& v2, const Vec2& pt) {
    return (v2.x - v1.x) * (pt.y - v1.y) - (v2.y - v1.y) *( pt.x - v1.x) > 0;
}



std::vector<std::pair<Vec2, Vec2>> Polygon::RayCast(const Vec2& v1, const Vec2& v2) const
{
    std::vector<std::pair<Vec2, Vec2>> intersections;

    for (int i = 0; i < vs.size(); i++)
    {
        Vec2 v3 = vs[i], v4 = i == vs.size() - 1 ? vs[0] : vs[i + 1];

        Vec2 intersection;
        if (!LineSegmentsIntersection(v1, v2, GetWorldPoint(v3), GetWorldPoint(v4), intersection)) continue;

        intersections.emplace_back(intersection, (v4 - v3).Rotated(M_PI_2 + angle).Normalized());

        // float dist = (intersection - v1).LengthSquared();
        // if (!closestDist || dist < closestDist)
        // {
        //     closestDist = dist;
        //     closestPoint = intersection;
        //     closestNormal = (v4 - v3).Rotated(M_PI_2).Normalized();
        // }
    }

    // if (!closestDist) return false;

    // outPoint = closestPoint;
    // outNormal = closestNormal;
    // return true;

    return intersections;
}

bool Polygon::TestPoint(const Vec2& pt) const
{
    Vec2 localPt = GetLocalPoint(pt);
    for (int i = 0; i < vs.size(); i++)
    {
        Vec2 v1 = vs[i], v2 = i == vs.size() - 1 ? vs[0] : vs[i + 1];
        if (isLeft(v1, v2, localPt)) return false;
    }
    return true;
}

std::vector<std::pair<Vec2, Vec2>> Polygon::TestCollision(const Shape* other) const
{
    std::vector<std::pair<Vec2, Vec2>> intersections;
    for (int i = 0; i < vs.size(); i++)
    {
        Vec2 v1 = vs[i], v2 = i == vs.size() - 1 ? vs[0] : vs[i + 1];
        auto pairs = other->RayCast(GetWorldPoint(v1), GetWorldPoint(v2));
        for (auto& p : pairs)
        {
            //intersections.push_back(p);
            intersections.push_back({p.first, (v2 - v1).Rotated(M_PI_2).Normalized()});
        }
    }
    return intersections;
}

std::vector<std::pair<Vec2, Vec2>> Polygon::TestCircleCollision(const Circle* circle) const
{
    return TestCollision((Shape*)circle);
}

float Polygon::ComputeMass() const
{
    float sum = 0.0f;
    for (int i = 0; i < vs.size(); i++)
    {
        Vec2 v1 = vs[i], v2 = i == vs.size() - 1 ? vs[0] : vs[i + 1];
        sum += v1.x * v2.y - v2.x * v1.y;
    }
    return abs(sum / 2.0f);
}

Vec2 Polygon::ComputeCenterOfMass() const
{
    Vec2 sum;
    for (auto& v : vs) sum += v;
    return sum / vs.size();
}

float Polygon::ComputeMOI() const
{
    return pow(ComputeMass(), 2);
}

void Polygon::Draw() const
{
    Vec2 v1 = GetWorldPoint(vs[0]);
    for (int i = 1; i < vs.size() - 1; i++)
    {
        Vec2 v2 = GetWorldPoint(vs[i]), v3 = GetWorldPoint(vs[i + 1]);
        renderer.DrawFilledTriangle(v1, v2, v3, {100, 0, 0}, {100, 0, 0}, {100, 0, 0});
    }
    for (int i = 0; i < vs.size(); i++)
    {
        Vec2 v1 = GetWorldPoint(vs[i]), v2 = i == vs.size() - 1 ? GetWorldPoint(vs[0]) : GetWorldPoint(vs[i + 1]);
        renderer.DrawLine(v1, v2, {255, 0, 0});
    }
}