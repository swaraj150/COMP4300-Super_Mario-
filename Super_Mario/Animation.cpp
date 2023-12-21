#include "Animation.h"
#include "Common.h"

Animation::Animation() {};
Animation::Animation(eAnimationTypes& name, const sf::Texture& t)
	: Animation(name, t, 1, 0) {}

Animation::Animation(eAnimationTypes& name, const sf::Texture& t, size_t frameCount, size_t speed) : m_name(name),m_sprite(t),m_frameCount(frameCount),m_AnimationSpeed(speed){
	m_size = Vec2((float)t.getSize().x / frameCount, (float)t.getSize().y);
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}

void Animation::update() {
	this->m_currentFrame++;
	int animationFrame = (int)(m_currentFrame / m_AnimationSpeed) % (int)m_frameCount;
	this->getSprite().setTextureRect(sf::IntRect(animationFrame * static_cast<int>(m_size.x), 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)));

  
}



bool Animation::hasEnded() const {
	if ((int)(m_currentFrame / m_AnimationSpeed) % (int)m_frameCount == m_frameCount-1)
		return true;
	else
		return false;
}

eAnimationTypes Animation::getName() const { return m_name; }

const Vec2& Animation::getSize() const { return m_size; }

sf::Sprite& Animation::getSprite() { return m_sprite; }

size_t Animation::getCurrentFrame() const {
	return m_currentFrame;
}


/*animation frame 0
m_currentFrame: 1
animation frame 0
m_currentFrame : 2
animation frame 0
m_currentFrame : 3
animation frame 0
m_currentFrame : 4
animation frame 0
m_currentFrame : 5
animation frame 0
m_currentFrame : 6
animation frame 0
m_currentFrame : 7
animation frame 1
m_currentFrame : 8
animation frame 1
m_currentFrame : 9
animation frame 1
m_currentFrame : 10
animation frame 1
m_currentFrame : 11
animation frame 1
m_currentFrame : 12
animation frame 1
m_currentFrame : 13
animation frame 1
m_currentFrame : 14
animation frame 1
m_currentFrame : 15
animation frame 2
m_currentFrame : 16
animation frame 2
m_currentFrame : 17
animation frame 2
m_currentFrame : 18
animation frame 2
m_currentFrame : 19
animation frame 2
m_currentFrame : 20
animation frame 2
m_currentFrame : 21
animation frame 2
m_currentFrame : 22
animation frame 2
m_currentFrame : 23
animation frame 0
m_currentFrame : 24
animation frame 0
m_currentFrame : 25
animation frame 0
m_currentFrame : 26
animation frame 0
m_currentFrame : 27
animation frame 0
m_currentFrame : 28
animation frame 0
m_currentFrame : 29
animation frame 0
m_currentFrame : 30
animation frame 0
m_currentFrame : 1
animation frame 0*/