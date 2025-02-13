#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H
#include <SFML/Graphics.hpp>
#include "Scene.h"

class Scene_Play: public Scene {
  sf::Color m_background;

  void init (const std::string& path) override;
  void sRender () override;
  Vec2 gridToMidPixel (float gridX, float gridY, std::shared_ptr<Entity> entity);

  public:
    Scene_Play(GameEngine* g, const std::string& path);
    
    void update () override;
    void doAction (const Action& action) override;
};

#endif