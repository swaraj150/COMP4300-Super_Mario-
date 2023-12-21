#include "Vec2.h"
#include<cmath>

Vec2::Vec2() {

}

Vec2::Vec2(float x1, float y1) :x(x1), y(y1) {

}

bool Vec2::operator ==(const Vec2& v) const {
	return x == v.x && y == v.y;
}
bool Vec2::operator !=(const Vec2& v) const {
	return x != v.x || y != v.y;
}
Vec2 Vec2::operator +(const Vec2& v) const {
	return Vec2(x + v.x, y + v.y);
}
Vec2 Vec2::operator -(const Vec2& v) const {
	return Vec2((x - v.x), (y - v.y));
}
Vec2 Vec2::operator *(const Vec2& v) const {
	return Vec2(x * v.x, y * v.y);
}
Vec2 Vec2::operator /(const Vec2& v) const {
	return Vec2((float)(x / v.x), (float)(y / v.y));
}
void Vec2::operator +=(const Vec2& v) {
	x += v.x;
	y += v.y;
}
void Vec2::operator -=(const Vec2& v) {
	x -= v.x;
	y -= v.y;
}
void Vec2::operator *=(const Vec2& v) {
	x *= v.x;
	y *= v.y;
}
void Vec2::operator /=(const Vec2& v) {
	x = (float)(x / v.x);
	y = (float)(y / v.y);
}

float Vec2::dist(const Vec2& v) const {
	return (v.x - x) * (v.x - x) + (v.y - y) * (v.y - y);
}

Vec2 Vec2::normalize()const {
	float l = sqrtf(x * x + y * y);
	return Vec2(x / l, y / l);
}

float Vec2::length()const {
	return x * x + y * y;
}