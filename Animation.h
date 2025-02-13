#ifndef ANIMATION_H
#define ANIMATION_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Vec2.h"

class Animation {
  std::string m_name;
  sf::Sprite  m_sprite;
  size_t      m_frameCount = 0;
  size_t      m_animFrame = 0;
  size_t      m_duration = 0;
  Vec2        m_size;

  public:
    Animation();
    Animation(const std::string& name, const sf::Texture& t);
    Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t duration);

    void update ();
    bool hasEnded () const;
    const std::string& getName() const;
    const Vec2& getSize() const;
    sf::Sprite& getSprite();
};

#endif