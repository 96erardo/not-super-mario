#ifndef ASSETS_H
#define ASSETS_H
#include <SFML/Graphics.hpp>
#include <map>
#include "Animation.h"

class Assets {
  std::map<std::string, std::shared_ptr<Animation>> m_animations;
  std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
  std::map<std::string, std::shared_ptr<sf::Font>> m_fonts;

  public:
    Assets ();

    void addFont (const std::string& name, const std::string& path);
    void addTexture (const std::string& name, const std::string& path);
    void addAnimation (const std::string& name, const sf::Texture& texture, int frameCount, int duration);

    std::shared_ptr<sf::Font> getFont (const std::string& name) const;
    std::shared_ptr<sf::Texture> getTexture (const std::string& name) const;
    std::shared_ptr<Animation> getAnimation (const std::string& name) const;
};

#endif