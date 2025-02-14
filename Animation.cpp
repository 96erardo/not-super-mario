#include "Vec2.h"
#include "Animation.h"

Animation::Animation (const sf::Texture& t): m_sprite(t) {};

Animation::Animation (const std::string& name, const sf::Texture& t, size_t frameCount, size_t duration): m_sprite(t) {
  m_name = name;
  m_frameCount = frameCount;
  m_duration = duration;
  
  auto [width, height] = t.getSize();
  float tileWidth = width / frameCount;
  int x = m_currentFrame * tileWidth;
  
  m_size = Vec2(tileWidth * 4, height * 4);
  m_sprite.setTextureRect(sf::IntRect({ x, 0 }, sf::Vector2<int>(tileWidth, height)));
  m_sprite.setOrigin(sf::Vector2<float>((tileWidth / 2), (height / 2)));
  m_sprite.scale({ 4, 4 });
}

void Animation::update () {
  if (m_frameCount == 1) {
    return;
  }

  m_frames++;
  
  if (m_frames % m_duration == 0) {
    m_currentFrame++;

    m_currentFrame = m_currentFrame % m_frameCount;
    auto [width, height] = m_sprite.getTexture().getSize();
    int x = m_currentFrame * (width / m_frameCount);

    m_sprite.setTextureRect(sf::IntRect({ x, 0 }, sf::Vector2<int>(width / m_frameCount, height)));
  }
}

const std::string& Animation::getName () const {
  return m_name;
}

const Vec2& Animation::getSize() const {
  return m_size;
}

sf::Sprite& Animation::getSprite() {
  return m_sprite;
}