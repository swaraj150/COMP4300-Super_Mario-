#pragma once
#include "Animation.h"
#include "Assets.h"

enum eStates {
	RUNNING=0,
	JUMPING,
	SHOOTING,
	FALLING,
	STANDING
};

class Components {
public:
	bool has = false;

};

class CTransform :public Components {
public:
	Vec2 pos = { 0.0,0.0 };
	Vec2 prevPos = { 0.0,0.0 };
	Vec2 velocity = { 0.0,0.0 };
	Vec2 scale = { 1.0,1.0 };
	float angle=0;
	float aimPos=0;

	CTransform() {};
	explicit CTransform(const Vec2& p)
		: pos(p) {}
	//The use of explicit is particularly beneficial when you want to avoid accidental implicit conversions that might lead to unexpected behavior in your code. It makes the code more explicit and helps prevent potential bugs related to automatic type conversions.
	CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
		: pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}
};

class CBoundingBox :public Components {
public:
	Vec2 size = { 0.0,0.0 };


	CBoundingBox() {};
	explicit CBoundingBox(const Vec2& p)
		: size(p) {}
	//The use of explicit is particularly beneficial when you want to avoid accidental implicit conversions that might lead to unexpected behavior in your code. It makes the code more explicit and helps prevent potential bugs related to automatic type conversions.
	
};

class CLifeSpan :public Components {
public:
	sf::Clock clock;
	int lifeSpan = 0;
	CLifeSpan() {};
	explicit CLifeSpan(int l)
		: lifeSpan(l) {
		clock.restart();
	}
	//The use of explicit is particularly beneficial when you want to avoid accidental implicit conversions that might lead to unexpected behavior in your code. It makes the code more explicit and helps prevent potential bugs related to automatic type conversions.

};

class CInput :public Components {
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;
	bool jump = false; // long jump
	CInput() {};
	
};

class CAnimation :public Components {
public:
	Animation animation;
	bool repeat = false;

	CAnimation() {};
	CAnimation(const Animation& animation, bool r)
		: animation(animation), repeat(r) {}
};

class CGravity :public Components {
public:
	float gravity;
	int knockBack = 0;
	CGravity() {};
	CGravity(float gravity,int knockback)
		: gravity(gravity), knockBack(knockback) {}
};

class CState :public Components {
public:
	eStates state;
	CState() = default;

	size_t frames = 0;
	bool notGrounded = true;
	explicit CState(eStates& s) : state(s) {}
};