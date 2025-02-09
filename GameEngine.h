#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include <SFML/Graphics.hpp>
#include <map>
#include "Scene.h"

class GameEngine {
  sf::RenderWindow                               m_window;
  std::map<std::string, std::shared_ptr<Scene>>  m_scenes;
  std::string                                    m_currentScene;
  bool                                           m_running;

  void init ();
  void update ();  
  void sUserInput();

  public:
    GameEngine (const std::string& config);

    void run ();

    sf::RenderWindow&      window ();
    std::shared_ptr<Scene> currentScene ();
    void changeScene (const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);
};

#endif