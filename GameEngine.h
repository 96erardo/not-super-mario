#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include <SFML/Graphics.hpp>
#include <map>
#include "Scene.h"
#include "Assets.h"

class GameEngine {
  sf::RenderWindow                               m_window;
  std::map<std::string, std::shared_ptr<Scene>>  m_scenes;
  std::string                                    m_currentScene;
  Assets                                         m_assets;
  bool                                           m_running = true;

  void init ();
  void update ();  
  void loadAssets();
  void sUserInput(); 

  public:
    GameEngine (const std::string& config);

    void run();
    void quit();
    bool isRunning();

    sf::RenderWindow&      window ();
    std::shared_ptr<Scene> currentScene ();
    Assets&                assets ();

    void changeScene (const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);
};

#endif