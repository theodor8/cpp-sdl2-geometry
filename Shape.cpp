#include "Shape.h"

Vec2 Shape::GetLocalPoint(const Vec2& v) const { return v.Rotated(-angle) - pos; }
Vec2 Shape::GetWorldPoint(const Vec2& v) const { return v.Rotated(angle) + pos; }

void Shape::ApplyForce(Vec2 f, Vec2 pos) // Pos = worldpos
{
    Vec2 r = pos - GetWorldPoint(com); // Center of mass --> point of force application
    force += f;
    torque += r.x * f.y - r.y * f.x;
}

void Shape::Update(float dt)
{
    if (!com.x && !com.y) com = ComputeCenterOfMass();
    if (!mass) mass = ComputeMass();
    if (!moi) moi = ComputeMOI();

    Vec2 linAcc = force / mass;
    vel += linAcc * dt;
    pos += vel * dt;

    float angAcc = torque / moi;
    angleVel += angAcc * dt;
    angle += angleVel * dt;

    force = {0, 0};
    torque = 0;
}