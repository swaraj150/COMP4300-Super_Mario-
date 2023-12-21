#pragma once
#include "Vec2.h"
#include<SFML/Graphics.hpp>

enum eAnimationTypes {
  AniBRICK = 1,
  RUN,
  JUMP,
  STAND,
  SHOT,
  BUSTER,
  EXPLOSION,
  AniBUSH = 8,
  aniSMALLBUSH = 10,
  AniCLOUDSMALL = 12,
  aniGOOMBA,
  aniBUSHER=14,
  aniPIPE=15,
  aniBCLOUD = 16,
  aniGOODEATH,
  AniGROUND=20,
  AniQUESTION,
  AniQUESTIONBLOCK,
  AniCOIN,
  ALAST
};

class Animation {
	sf::Sprite		m_sprite;
	size_t			m_frameCount = 1;
	size_t			m_AnimationSpeed=0 ;
	size_t			m_currentFrame = 0;
	Vec2			m_size = { 0.0,0.0 };
	eAnimationTypes m_name;

public:
	Animation();
	Animation(eAnimationTypes& name, const sf::Texture& t);
	Animation(eAnimationTypes& name, const sf::Texture& t, size_t frameCount,size_t speed);
	void update();
	bool hasEnded() const;
	eAnimationTypes getName() const;
	const Vec2& getSize() const;
	sf::Sprite& getSprite();
	size_t getCurrentFrame() const;
};