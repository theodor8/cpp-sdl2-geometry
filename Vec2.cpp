#include "Vec2.h"

#include <math.h>

Vec2::Vec2() : x(0.0f), y(0.0f) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
Vec2 Vec2::operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
Vec2 Vec2::operator*(const Vec2& other) const { return Vec2(x * other.x, y * other.y); }
Vec2 Vec2::operator/(const Vec2& other) const { return Vec2(x / other.x, y / other.y); }

Vec2 Vec2::operator+(float other) const { return Vec2(x + other, y + other); }
Vec2 Vec2::operator-(float other) const { return Vec2(x - other, y - other); }
Vec2 Vec2::operator*(float other) const { return Vec2(x * other, y * other); }
Vec2 Vec2::operator/(float other) const { return Vec2(x / other, y / other); }

void Vec2::operator+=(const Vec2& other) { x += other.x; y += other.y; }
void Vec2::operator-=(const Vec2& other) { x -= other.x; y -= other.y; }
void Vec2::operator*=(const Vec2& other) { x *= other.x; y *= other.y; }
void Vec2::operator/=(const Vec2& other) { x /= other.x; y /= other.y; }

void Vec2::operator+=(float other) { x += other; y += other; }
void Vec2::operator-=(float other) { x -= other; y -= other; }
void Vec2::operator*=(float other) { x *= other; y *= other; }
void Vec2::operator/=(float other) { x /= other; y /= other; }

Vec2 Vec2::Rotated(float angle) const { return {cosf(angle) * x - sinf(angle) * y, sinf(angle) * x + cosf(angle) * y}; }
float Vec2::Angle() const { return atanf(y / x); }
float Vec2::Length() const { return sqrtf(x * x + y * y); }
float Vec2::LengthSquared() const { return abs(x * x + y * y); }
Vec2 Vec2::Normalized() const { return *this / Length(); }
int Vec2::Quadrant() const { return x > 0 && y > 0
                             ? 1
                             : x < 0 && y > 0
                               ? 2
                               : x < 0 && y < 0
                                 ? 3
                                 : 4; }

Vec2 Vec2::FromPolar(float length, float angle) { return {cosf(angle) * length, sinf(angle) * length}; }

float Vec2::Dot(const Vec2& other) const { return x * other.x + y * other.y; }