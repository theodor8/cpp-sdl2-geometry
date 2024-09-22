#pragma once

class Vec2
{
public:
    float x, y;

    Vec2();
    Vec2(float x, float y);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(const Vec2& other) const;
    Vec2 operator/(const Vec2& other) const;

    Vec2 operator+(float other) const;
    Vec2 operator-(float other) const;
    Vec2 operator*(float other) const;
    Vec2 operator/(float other) const;

    void operator+=(const Vec2& other);
    void operator-=(const Vec2& other);
    void operator*=(const Vec2& other);
    void operator/=(const Vec2& other);

    void operator+=(float other);
    void operator-=(float other);
    void operator*=(float other);
    void operator/=(float other);

    Vec2 Rotated(float angle) const; // CCW
    float Angle() const;
    float Length() const;
    float LengthSquared() const;
    Vec2 Normalized() const;
    int Quadrant() const;
    
    static Vec2 FromPolar(float length, float angle);

    float Dot(const Vec2& other) const;
};