#pragma once

#include <vector>

#include "Vec2.h"

class Circle;

class Shape
{
public:

    Vec2 vel, pos, force, com;
    float angle, angleVel, torque, mass, moi;

    virtual ~Shape() = default;
    
    virtual std::vector<std::pair<Vec2, Vec2>> RayCast(const Vec2& v1, const Vec2& v2) const = 0; // Intersect point, normal
    virtual bool TestPoint(const Vec2& pt) const = 0;
    virtual std::vector<std::pair<Vec2, Vec2>> TestCollision(const Shape* other) const = 0;
    virtual std::vector<std::pair<Vec2, Vec2>> TestCircleCollision(const Circle* circle) const = 0; // Called when circle wants to test collision with this shape
    virtual float ComputeMass() const = 0;
    virtual Vec2 ComputeCenterOfMass() const = 0; // Local point
    virtual float ComputeMOI() const = 0;

    Vec2 GetLocalPoint(const Vec2& v) const;
    Vec2 GetWorldPoint(const Vec2& v) const;

    void ApplyForce(Vec2 force, Vec2 pos);

    

    void Update(float dt);
    virtual void Draw() const = 0;
};