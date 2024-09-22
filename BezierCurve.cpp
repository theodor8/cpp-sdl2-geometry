#include "BezierCurve.h"

#include <vector>

#include "Vec2.h"
#include "Renderer.h"

extern Renderer renderer;

void BezierCurve::Draw(float t) const
{
    // for (int i = 0; i < points.size() - 1; i++)
    // {
    //     Vec2 p1 = points[i], p2 = points[i + 1];
    //     renderer.DrawLine(p1, p2, {150, 150, 150});
    // }

    // for (float tt = 0; tt < t; tt += 0.01)
    // {
    //     renderer.DrawFilledCircle(ComputePoint(points, tt), 1, {255, 255, 0});
    // }

    renderer.DrawFilledCircle(ComputePoint(points, t), 5, {255, 255, 0});
}

Vec2 BezierCurve::ComputePoint(std::vector<Vec2> pts, float t) const
{
    if (pts.size() == 1) return pts[0];

    std::vector<Vec2> qs;

    for (int i = 0; i < pts.size() - 1; i++)
    {
        Vec2 p1 = pts[i], p2 = pts[i + 1];
        qs.push_back(p1 + (p2 - p1) * t);
        renderer.DrawLine(p1, p2, {150, 150, 150});
    }

    return ComputePoint(qs, t);
}