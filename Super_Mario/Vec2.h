#pragma once
class Vec2 {
public:

	float x = 0;
	float y = 0;

	Vec2();
	Vec2(float x1, float y1);


	bool operator == (const Vec2& v) const;
	bool operator != (const Vec2& v) const;

	Vec2 operator + (const Vec2& v) const;//const at the end of the function declaration means that this member function does not modify the state of the object it is called on. This is particularly useful when you have a const object of type Vec2, and you want to ensure that calling this function won't modify the object.
	Vec2 operator - (const Vec2& v) const;
	Vec2 operator * (const Vec2& v) const;
	Vec2 operator / (const Vec2& v) const;

	void operator += (const Vec2& v);// here we want to modify the the object ,thus no const
	void operator -= (const Vec2& v);// here we want to modify the the object ,thus no const
	void operator *= (const Vec2& v);// here we want to modify the the object ,thus no const
	void operator /= (const Vec2& v);// here we want to modify the the object ,thus no const

	float dist(const Vec2& v) const;
	Vec2 normalize() const;

	float length()const;

};
