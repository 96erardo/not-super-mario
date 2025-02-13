#include "Vec2.h"
#include "Animation.h"

Animation::Animation (const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed): m_sprite(sf::Sprite(t)) {
  m_name = name;
  m_frameCount = frameCount;
  m_duration = speed;
  
  auto [width, height] = t.getSize();
  float tileWidth = width / frameCount;
  int x = m_animFrame * tileWidth;
  
  m_size = Vec2(tileWidth * 4, height * 4);
  m_sprite.setTextureRect(sf::IntRect({ x, 0 }, sf::Vector2<int>(tileWidth, height)));
  m_sprite.setOrigin(sf::Vector2<float>((tileWidth / 2), (height / 2)));
  m_sprite.scale({ 4, 4 });
}

const Vec2& Animation::getSize() const {
  return m_size;
}

sf::Sprite& Animation::getSprite() {
  return m_sprite;
}