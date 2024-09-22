#pragma once

#include <vector>

#include "Vec2.h"

class BezierCurve
{
public:
    std::vector<Vec2> points;

    template<typename... Args>
    BezierCurve(Args&&... args) : points({args...}) {}

    void Draw(float t) const;
    Vec2 ComputePoint(std::vector<Vec2> pts, float t) const;
};