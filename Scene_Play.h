#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Scene.h"
#include "Entity.h"

struct PlayerConfig {
  float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
  std::string WEAPON;
};

class Scene_Play: public Scene {
  sf::Color               m_background;
  std::shared_ptr<Entity> m_player;
  PlayerConfig            m_playerConfig;

  void init (const std::string& path) override;
  void sMovement ();
  void sAnimation ();
  void sRender () override;
  Vec2 gridToMidPixel (float gridX, float gridY, std::shared_ptr<Entity> entity);

  public:
    Scene_Play(GameEngine* g, const std::string& path);
    
    void update () override;
    void doAction (const Action& action) override;
};

#endif