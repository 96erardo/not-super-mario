#include "Assets.h"

Assets::Assets() {};

void Assets::addFont (const std::string& name, const std::string& path) {
  m_fonts.insert({
    name, 
    std::make_shared<sf::Font>(path)
  });
}

void Assets::addTexture (const std::string& name, const std::string& path) {
  m_textures.insert({
    name, 
    std::make_shared<sf::Texture>(path)
  });
}

void Assets::addAnimation (const std::string& name, const sf::Texture& texture, int frameCount, int duration) {
  m_animations.insert({
    name, 
    std::make_shared<Animation>(name, texture, frameCount, duration)
  });
}

std::shared_ptr<sf::Font> Assets::getFont (const std::string& name) const {
  return m_fonts.at(name);
}

std::shared_ptr<sf::Texture> Assets::getTexture (const std::string& name) const {
  return m_textures.at(name);
}

std::shared_ptr<Animation> Assets::getAnimation (const std::string& name) const {
  return m_animations.at(name);
}