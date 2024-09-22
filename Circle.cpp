#include "Circle.h"

#include "Renderer.h"

extern Renderer renderer;


std::vector<std::pair<Vec2, Vec2>> Circle::RayCast(const Vec2& v1, const Vec2& v2) const
{
    // Line equation
    float k = (v1.y - v2.y) / (v1.x - v2.x);
    float m = v1.y - k * v1.x;

    float x2coef = -(k * k) - 1;
    float xcoef = (k * (2 * pos.y - 2 * m) + 2 * pos.x) / x2coef;
    float constant = (2 * m * pos.y - pos.x * pos.x - pos.y * pos.y - m * m + radius * radius) / x2coef;

    float p2 = xcoef / 2;
    float inRoot = p2 * p2 - constant;
    if (inRoot < 0) return {};
    float root = sqrtf(inRoot);
    float x1 = -p2 + root, x2 = -p2 - root;

    float minx, maxx;
    if (v1.x < v2.x) { minx = v1.x; maxx = v2.x; }
    else { minx = v2.x; maxx = v1.x; }
    if ((x1 < minx || x1 > maxx) && (x2 < minx || x2 > maxx)) return {};

    std::vector<std::pair<Vec2, Vec2>> intersections;
    if (x1 >= minx && x1 <= maxx)
    {
        Vec2 p(x1, k * x1 + m);
        Vec2 n = (p - pos).Normalized();
        intersections.emplace_back(p, n);
    }
    if (x2 >= minx && x2 <= maxx)
    {
        Vec2 p(x2, k * x2 + m);
        Vec2 n = (p - pos).Normalized();
        intersections.emplace_back(p, n);
    }
    //outPoint = abs(x1 - v1.x) < abs(x2 - v1.x) ? Vec2(x1, k * x1 + m) : Vec2(x2, k * x2 + m);
    //outNormal = (outPoint - pos).Normalized();
    
    return intersections;
}

bool Circle::TestPoint(const Vec2& pt) const
{
    return (pt - pos).LengthSquared() <= radius * radius;
}

std::vector<std::pair<Vec2, Vec2>> Circle::TestCollision(const Shape* other) const
{
    return other->TestCircleCollision(this);
}

#include <iostream>

std::vector<std::pair<Vec2, Vec2>> Circle::TestCircleCollision(const Circle* circle) const
{
    if ((pos - circle->pos).LengthSquared() > powf(radius + circle->radius, 2)) return {};

    float R = (pos - circle->pos).Length(), r1 = radius, r2 = circle->radius, x1 = pos.x, y1 = pos.y, x2 = circle->pos.x, y2 = circle->pos.y;

    Vec2 vl = (pos + circle->pos) / 2.0f + (circle->pos - pos) * ((r1 * r1 - r2 * r2) / (2.0f * R * R));
    Vec2 vr = Vec2(y2 - y1, x1 - x2) * sqrtf(2.0f * (r1 * r1 + r2 * r2) / (R * R) - powf(r1 * r1 - r2 * r2, 2) / powf(R, 4) - 1) / 2.0f;

    Vec2 p1 = vl + vr, p2 = vl - vr;
    Vec2 n1 = (p1 - pos).Normalized(), n2 = (p2 - pos).Normalized();

    return {{p1, n1}, {p2, n2}};
}

float Circle::ComputeMass() const
{
    return M_PI * radius * radius;
}

Vec2 Circle::ComputeCenterOfMass() const
{
    return {0, 0};
}

float Circle::ComputeMOI() const
{
    return M_PI_4 * powf(radius, 4);
}


void Circle::Draw() const
{
    renderer.DrawFilledCircle(pos, radius, {0, 100, 100});
    renderer.DrawCircle(pos, radius, {0, 255, 255});
    renderer.DrawLine(pos, pos + Vec2::FromPolar(radius, angle), {0, 255, 255});
}