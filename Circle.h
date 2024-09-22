#pragma once

#include "Shape.h"

class Circle : public Shape
{
public:
    float radius;
    
    Circle(float radius)
        : radius(radius) {}

    std::vector<std::pair<Vec2, Vec2>> RayCast(const Vec2& v1, const Vec2& v2) const override;
    bool TestPoint(const Vec2& pt) const override;
    std::vector<std::pair<Vec2, Vec2>> TestCollision(const Shape* other) const override;
    std::vector<std::pair<Vec2, Vec2>> TestCircleCollision(const Circle* circle) const override;
    float ComputeMass() const override;
    Vec2 ComputeCenterOfMass() const override;
    float ComputeMOI() const override;

    void Draw() const override;
};